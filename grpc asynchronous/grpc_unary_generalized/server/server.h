#include <grpc/grpc.h>
#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>

#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "calldata_interface.h"
#include "mx.grpc.pb.h"

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;

using GeneralizedGRPC::ECGData;
using GeneralizedGRPC::ECGDataList;
using GeneralizedGRPC::NIBPData;
using GeneralizedGRPC::NIBPDataList;
using GeneralizedGRPC::PatientIndex;
using GeneralizedGRPC::SpO2Data;
using GeneralizedGRPC::SpO2DataList;
using GeneralizedGRPC::TrendData;

class GeneralizedGRPCServer
{
public:
      void start();
      ~GeneralizedGRPCServer()
      {
            generalizedGRPCServer->Shutdown();
            generalizedGRPCServerCompletionQueue->Shutdown();
      }

private:
      // 1. Statemachine
      template <typename AsyncService, typename ReadMessage, typename WriteMessage>
      class CallData : CallDataInterface
      {
      public:
            // construtor
            CallData(
                AsyncService *,
                ServerCompletionQueue *,
                std::function<void(::grpc::ServerContext *, ReadMessage*, ::grpc::ServerAsyncResponseWriter<WriteMessage> *,
                  ::grpc::CompletionQueue *, ::grpc::ServerCompletionQueue *,void *tag)>,
                std::function<WriteMessage(ReadMessage)>);
            // logic function
            void executeLogic() override;

      private:
            AsyncService *service_;
            ServerCompletionQueue *serverCompletionQueue_;
            std::function<void(::grpc::ServerContext *, 
            ReadMessage*, ::grpc::ServerAsyncResponseWriter<WriteMessage> *,
                  ::grpc::CompletionQueue *, ::grpc::ServerCompletionQueue *,void *tag)> requestClientFN_;
            std::function<WriteMessage(ReadMessage)> handleRequestFN_;

            enum CallStatus
            {
                  CREATE,
                  PROCESS,
                  FINISH
            };
            CallStatus callStatus_;
            ReadMessage readMessage_;
            WriteMessage writeMessage_;
            ServerAsyncResponseWriter<WriteMessage> serverAsyncResponseWriter_;
            ServerContext context_;
      };

      // 2
      void handleRPCS();
      // 3
      std::unique_ptr<ServerCompletionQueue> generalizedGRPCServerCompletionQueue;

      GeneralizedGRPC::TrendData::AsyncService trendDataAsyncService;
      std::unique_ptr<Server> generalizedGRPCServer;

      // GeneralizedGRPC::PatientIndex pindex;
      // GeneralizedGRPC::ECGData ecgData;
      // GeneralizedGRPC::ECGDataList ecgDataList;
      // GeneralizedGRPC::SpO2Data spO2Data;
      // GeneralizedGRPC::SpO2DataList spO2DataList;
      // GeneralizedGRPC::NIBPData nibpData;
      // GeneralizedGRPC::NIBPDataList nibpDataList;

      ECGDataList handleECGRequest(PatientIndex);
      SpO2DataList handleSpO2Request(PatientIndex);
      NIBPDataList handleNIBPRequest(PatientIndex);
};
