#include <iostream>
#include <memory>
#include <string>

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>

#include "mx.grpc.pb.h"

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;

using GeneralizedGRPC::ECGData;
using GeneralizedGRPC::ECGDataList;
using GeneralizedGRPC::NIBPData;
using GeneralizedGRPC::NIBPDataList;
using GeneralizedGRPC::PatientIndex;
using GeneralizedGRPC::SpO2Data;
using GeneralizedGRPC::SpO2DataList;
using GeneralizedGRPC::TrendData;

class GeneralizedGRPCClient
{
public:
      GeneralizedGRPCClient(std::shared_ptr<grpc::Channel> channel);
      void GetEcg(PatientIndex); 
      void GetSpO2(PatientIndex); 
      void GetNIBP(PatientIndex);

      void handleGetECGReply(ECGDataList);
      void handleGetSpO2Reply(SpO2DataList);
      void handleGetNIBPReply(NIBPDataList);

      ~GeneralizedGRPCClient()
      {
            completionQueue.get()->Shutdown();
      }

private:
      template <typename ReadMessage, typename WriteMessage>
      class CallData
      {
      public:
            // construtor
            CallData(
                std::shared_ptr<TrendData::Stub>,
                std::shared_ptr<CompletionQueue>,
                WriteMessage writeMessage,
                std::function<std::unique_ptr<::grpc::ClientAsyncResponseReader<ReadMessage>>(::grpc::ClientContext *, const WriteMessage &, ::grpc::CompletionQueue *)>,
                std::function<void(ReadMessage)>);
            // logic function
            void executeLogic();

      private:
            std::shared_ptr<TrendData::Stub> stub_;
            std::shared_ptr<CompletionQueue> completionQueue_;
            std::unique_ptr< ::grpc::ClientAsyncResponseReader<ReadMessage>> rpc_;
            std::function<std::unique_ptr<::grpc::ClientAsyncResponseReader<ReadMessage>>(::grpc::ClientContext *, const WriteMessage &, ::grpc::CompletionQueue *)> asyncGetReadMessage_;
            std::function<void(ReadMessage)> handleReply_;

            ReadMessage readMessage_;
            WriteMessage writeMessage_;
            Status status_;
            void *tag_;
            bool ok_;

            enum CallStatus
            {
                  CREATE,
                  PROCESS,
                  FINISH
            };
      };

      std::shared_ptr<TrendData::Stub> stub;
      std::shared_ptr<CompletionQueue> completionQueue;
};
