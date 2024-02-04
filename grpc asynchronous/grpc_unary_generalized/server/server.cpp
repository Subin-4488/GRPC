#include "server.h"

template <typename AsyncService, typename ReadMessage, typename WriteMessage>
GeneralizedGRPCServer::CallData<AsyncService, ReadMessage, WriteMessage>::CallData(AsyncService *asyncService, ServerCompletionQueue *cq, std::function<void(::grpc::ServerContext *, ReadMessage *readMessage, ::grpc::ServerAsyncResponseWriter<WriteMessage> *, ::grpc::CompletionQueue *, ::grpc::ServerCompletionQueue *, void *)> requestFN, std::function<WriteMessage(ReadMessage)> handleFN)
    : service_{asyncService}, serverCompletionQueue_{cq}, serverAsyncResponseWriter_(&context_), requestClientFN_{requestFN}, handleRequestFN_{handleFN}, callStatus_{CallStatus::CREATE}
{
    executeLogic();
}

template <typename AsyncService, typename ReadMessage, typename WriteMessage>
void GeneralizedGRPCServer::CallData<AsyncService, ReadMessage, WriteMessage>::executeLogic()
{
    WriteMessage writeMessage;
    switch (callStatus_)
    {
    case CallStatus::CREATE:
        std::cout << "Enter CREATE" << std::endl;
        callStatus_ = CallStatus::PROCESS;
        requestClientFN_(&context_, &readMessage_, &serverAsyncResponseWriter_, serverCompletionQueue_, serverCompletionQueue_, this);
        std::cout << "AFTER CREATE" << std::endl;
        break;
    case CallStatus::PROCESS:
        std::cout << "Enter PROCESS" << std::endl;
        callStatus_ = CallStatus::FINISH;

        new CallData<AsyncService, ReadMessage, WriteMessage>(service_, serverCompletionQueue_, requestClientFN_, handleRequestFN_);

        writeMessage = handleRequestFN_(readMessage_);

        serverAsyncResponseWriter_.Finish(writeMessage, Status::OK, this);
        
        std::cout << "AFTER PROCESS" << std::endl;
        break;
    
    case CallStatus::FINISH:
        std::cout << "Enter FINISH" << std::endl;
        delete this;
        std::cout << "Enter FINISH" << std::endl;
        break;
    default:
        break;
    }
}

void GeneralizedGRPCServer::start()
{
    std::string serverAddress = "127.0.0.1:50051";
    ServerBuilder builder;
    builder.AddListeningPort(serverAddress, grpc::InsecureServerCredentials());

    builder.RegisterService(&trendDataAsyncService);
    generalizedGRPCServerCompletionQueue = builder.AddCompletionQueue();

    generalizedGRPCServer = builder.BuildAndStart();

    std::cout << "Server listening on " << serverAddress << std::endl;

    handleRPCS();
}

void GeneralizedGRPCServer::handleRPCS()
{
    std::cout << "Calldata instance created! for ECG" << std::endl;
    new CallData<GeneralizedGRPC::TrendData::AsyncService, GeneralizedGRPC::PatientIndex, GeneralizedGRPC::ECGDataList>(
        &trendDataAsyncService,
        generalizedGRPCServerCompletionQueue.get(),
        std::bind(&TrendData::AsyncService::RequestGetECG, &trendDataAsyncService,
                  std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6),
        std::bind(&GeneralizedGRPCServer::handleECGRequest, this, std::placeholders::_1)
        );

    std::cout << "Calldata instance created! for spO2" << std::endl;
    new CallData<GeneralizedGRPC::TrendData::AsyncService, GeneralizedGRPC::PatientIndex, GeneralizedGRPC::SpO2DataList>(
        &trendDataAsyncService,
        generalizedGRPCServerCompletionQueue.get(),
        std::bind(&TrendData::AsyncService::RequestGetSpO2, &trendDataAsyncService,
                  std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6),
        std::bind(&GeneralizedGRPCServer::handleSpO2Request, this, std::placeholders::_1));

    std::cout << "Calldata instance created! for NIBP" << std::endl;
    new CallData<GeneralizedGRPC::TrendData::AsyncService, GeneralizedGRPC::PatientIndex, GeneralizedGRPC::NIBPDataList>(
        &trendDataAsyncService,
        generalizedGRPCServerCompletionQueue.get(),
        std::bind(&TrendData::AsyncService::RequestGetNIBP, &trendDataAsyncService,
                  std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6),
        std::bind(&GeneralizedGRPCServer::handleNIBPRequest, this, std::placeholders::_1));

    void *tag;
    bool status;

    while (true)
    {
        std::cout << "Waiting on NEXT()" << std::endl;
        generalizedGRPCServerCompletionQueue->Next(&tag, &status);
        std::cout << "Executing logic..." << std::endl;
        static_cast<CallDataInterface*>(tag)->executeLogic();
    }
}

ECGDataList GeneralizedGRPCServer::handleECGRequest(PatientIndex index)
{
    ECGDataList dataList;
    for (int i = 0; i < 2; i++)
    {
        ECGData *data = dataList.add_ecgs();
        data->set_ecgvalue(i + 1);
        std::string tmp = "ECG: "+std::to_string(i+1);
        data->set_ecgmessage(tmp);
    }

    return dataList;
}

SpO2DataList GeneralizedGRPCServer::handleSpO2Request(PatientIndex index)
{
    SpO2DataList dataList;
    for (int i = 0; i < 4; i++)
    {
        SpO2Data *data = dataList.add_spo2s();
        data->set_spo2value(i + 10);
        std::string tmp = "SpO2: "+std::to_string(i+11);
        data->set_spo2message(tmp);
    }

    return dataList;
}

NIBPDataList GeneralizedGRPCServer::handleNIBPRequest(PatientIndex index)
{
    NIBPDataList dataList;
    for (int i = 0; i < 6; i++)
    {
        NIBPData *data = dataList.add_nibps();
        data->set_nibpvalue(i + 100);
        std::string tmp = "NIBP: "+std::to_string(i+101);
        data->set_nibpmessage(tmp);
    }

    return dataList;
}

int main()
{
    GeneralizedGRPCServer server;
    server.start();
    return 0;
}
