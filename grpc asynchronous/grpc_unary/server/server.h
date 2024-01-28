#include <grpc/grpc.h>
#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>

#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "mx.grpc.pb.h"

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;

using Greet::Greetings;
using Greet::GreetUser;
using Greet::PersonData;
using Greet::User;

class GreetServer final
{
public:
    void start();
    ~GreetServer(){
        greetServer->Shutdown();
        greetServerCompletionQueue->Shutdown();
    }
private:
    // 1
    class CallData
    {
    public:
        CallData(GreetUser::AsyncService *, ServerCompletionQueue *);
        void executeLogic();

    private:
        GreetUser::AsyncService *service_;
        ServerCompletionQueue *serverCompletionQueue_;

        enum CallStatus
        {
            CREATE,
            PROCESS,
            FINISH
        };
        CallStatus callStatus_;
        PersonData pdataRequest_;
        Greetings greetingResponse_;
        ServerAsyncResponseWriter<Greetings> serverAsyncResponseWriter_;
        ServerContext context_;
    };
    // 2
    void handleRPCS();
    // 3
    std::unique_ptr<ServerCompletionQueue> greetServerCompletionQueue;
    GreetUser::AsyncService greetAsyncService;
    std::unique_ptr<Server> greetServer;
};
