#include "server.h"

GreetServer::GreetServer() {}

::grpc::Status GreetServer::WishUser(::grpc::ServerContext *context, const ::Greet::PersonData *personData, ::Greet::Greetings *greetings)
{
      std::cout << "Client connected..." << std::endl;
      if (personData->user() == User::admin)
      {
            greetings->set_greetingsmessage("Hello admin...");
            return Status::OK;
      }

      std::string msg = "Hello " + personData->name() + ", you are turning " + std::to_string(personData->age()) + " today!!";
      greetings->set_greetingsmessage(msg);
      return Status::OK;
}

GreetServer::~GreetServer() {}

int main()
{
      std::cout << "Server starts" << std::endl;
      std::string server_address("127.0.0.1:50051");
      GreetServer greetService;

      ServerBuilder builder;
      builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
      builder.RegisterService(&greetService);
      std::unique_ptr<Server> server(builder.BuildAndStart());
      std::cout << "Server listening on " << server_address << std::endl;
      server->Wait();

      return 0;
}