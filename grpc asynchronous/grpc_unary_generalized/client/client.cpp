#include "client.h"

int main()
{
      std::cout << "Client starts" << std::endl;

      GeneralizedGRPCClient client(grpc::CreateChannel("127.0.0.1:50051",grpc::InsecureChannelCredentials()));

      PatientIndex p1;
      p1.set_index(1);

      ECGDataList ecgList;
      SpO2DataList spo2List;
      NIBPDataList nibpList;

      client.GetEcg(p1);
      client.GetSpO2(p1);
      client.GetNIBP(p1);

      return 0;
}
GeneralizedGRPCClient::GeneralizedGRPCClient(std::shared_ptr<Channel> channel) : stub{TrendData::NewStub(channel)}
{
      completionQueue = std::make_shared<CompletionQueue>();
}

template<typename ReadMessage, typename WriteMessage>
GeneralizedGRPCClient::CallData<ReadMessage, WriteMessage>::CallData(std::shared_ptr<TrendData::Stub> stub, std::shared_ptr<CompletionQueue> cq, WriteMessage writeMessage, std::function<std::unique_ptr<::grpc::ClientAsyncResponseReader<ReadMessage>> (::grpc::ClientContext *, const WriteMessage &, ::grpc::CompletionQueue *)> asyncGetReadMessage, std::function<void (ReadMessage)> handleReply) 
:
writeMessage_{writeMessage}, stub_{stub}, completionQueue_{cq}, asyncGetReadMessage_{asyncGetReadMessage}, handleReply_{handleReply}
{
      executeLogic();
}

template <typename ReadMessage, typename WriteMessage>
void GeneralizedGRPCClient::CallData<ReadMessage, WriteMessage>::executeLogic()
{
      ClientContext context;
      rpc_ = asyncGetReadMessage_(&context, writeMessage_, completionQueue_.get());
      rpc_->Finish(&readMessage_, &status_, &ok_);
      std::cout << "Wait on NEXT ReadMessage" << std::endl;
      completionQueue_->Next(&tag_, &ok_);
      if (status_.ok() )//&& tag_ == (void *)1)
      {
            handleReply_(readMessage_);
      }
      else
      {
            std::cout << "RPC failed " << status_.error_code() <<" "<<status_.error_message() << std::endl;
      }
}

void GeneralizedGRPCClient::GetEcg(PatientIndex pIndex)
{
      new CallData<ECGDataList, PatientIndex>(stub, completionQueue, pIndex,
      std::bind(&GeneralizedGRPC::TrendData::Stub::AsyncGetECG, stub, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), 
      std::bind(&GeneralizedGRPCClient::handleGetECGReply, this, std::placeholders::_1));
}

void GeneralizedGRPCClient::GetSpO2(PatientIndex pIndex)
{
      new CallData<SpO2DataList, PatientIndex>(stub, completionQueue, pIndex,
      std::bind(&GeneralizedGRPC::TrendData::Stub::AsyncGetSpO2, stub, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), 
      std::bind(&GeneralizedGRPCClient::handleGetSpO2Reply, this, std::placeholders::_1));
}

void GeneralizedGRPCClient::GetNIBP(PatientIndex pIndex)
{
      new CallData<NIBPDataList, PatientIndex>(stub, completionQueue, pIndex,
      std::bind(&GeneralizedGRPC::TrendData::Stub::AsyncGetNIBP, stub, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), 
      std::bind(&GeneralizedGRPCClient::handleGetNIBPReply, this, std::placeholders::_1));
}

void GeneralizedGRPCClient::handleGetECGReply(ECGDataList ecgList)
{
      for (int i = 0; i < ecgList.ecgs_size(); i++)
            std::cout << "ECG: " << ecgList.ecgs()[i].ecgvalue() << ", MESS: " << ecgList.ecgs()[i].ecgmessage() << std::endl;
}

void GeneralizedGRPCClient::handleGetSpO2Reply(SpO2DataList spo2List)
{
      for (int i = 0; i < spo2List.spo2s_size(); i++)
            std::cout << "SPO2: " << spo2List.spo2s()[i].spo2value() << spo2List.spo2s()[i].spo2message() << std::endl;
}

void GeneralizedGRPCClient::handleGetNIBPReply(NIBPDataList nibpList)
{
      for (int i = 0; i < nibpList.nibps_size(); i++)
            std::cout << "NIBP: " << nibpList.nibps()[i].nibpvalue() << nibpList.nibps()[i].nibpmessage() << std::endl;
}
