#include "server.h"

int main()
{

      GreetServer server;
      server.start();
      return 0;
}

void GreetServer::start()
{
      std::string serverAddress = "127.0.0.1:50051";
      ServerBuilder builder;
      builder.AddListeningPort(serverAddress, grpc::InsecureServerCredentials());

      builder.RegisterService(&greetAsyncService);
      greetServerCompletionQueue = builder.AddCompletionQueue();

      greetServer = builder.BuildAndStart();

      std::cout << "Server listening on " << serverAddress << std::endl;

      handleRPCS();
}

void GreetServer::handleRPCS()
{
      std::cout << "Calldata instance created!" << std::endl;
      new CallData(&greetAsyncService, greetServerCompletionQueue.get());

      void *tag;
      bool status;

      while (true)
      {
            std::cout << "Waiting on NEXT()" << std::endl;
            greetServerCompletionQueue->Next(&tag, &status);
            std::cout << "Executing logic..." << std::endl;
            static_cast<CallData *>(tag)->executeLogic();
      }
}

GreetServer::CallData::CallData(GreetUser::AsyncService *service, ServerCompletionQueue *cq)
    : service_(service), serverCompletionQueue_(cq), serverAsyncResponseWriter_(&context_), callStatus_(CallStatus::CREATE)
{
      executeLogic();
}

void GreetServer::CallData::executeLogic()
{
      switch (callStatus_)
      {
      case CallStatus::CREATE:
            std::cout << "Enter CREATE" << std::endl;
            callStatus_ = CallStatus::PROCESS;
            service_->RequestWishUser(&context_, &pdataRequest_, &serverAsyncResponseWriter_, serverCompletionQueue_, serverCompletionQueue_, this);
            std::cout << "AFTER CREATE" << std::endl;
            break;
      case CallStatus::PROCESS:
            std::cout << "Enter PROCESS" << std::endl;
            callStatus_ = CallStatus::FINISH;
            new CallData(service_, serverCompletionQueue_); // try commenting this line

            if (pdataRequest_.user() == Greet::User::admin)
            {
                  std::string msg = "Hello ADMIN SIR!!";
                  greetingResponse_.set_greetingsmessage(msg);
            }
            else
            {
                  std::string msg = "Hello " + pdataRequest_.name() + ", you are turning " + std::to_string(pdataRequest_.age()) + " today!!";
                  greetingResponse_.set_greetingsmessage(msg);
            }

            serverAsyncResponseWriter_.Finish(greetingResponse_, Status::OK, this);
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
