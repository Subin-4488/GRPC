#include "server.h"

GreetServer::GreetServer()
{
      //_service = std::shared_ptr<GreetServer::Service>();
}

::grpc::Status GreetServer::WishUser(::grpc::ServerContext *context, const ::Greet::PersonData *personData, ::Greet::Greetings *greetings)
{
      if (personData->user() == Greet::User::admin)
      {
            greetings->set_greetingsmessage("Hey admin");
            return Status::OK;
      }
      std::cout << "CLient connected " << std::endl;
      std::string msg = "Hello " + personData->name() + ", you are turning " + std::to_string(personData->age()) + " today!!";
      greetings->set_greetingsmessage(msg);
      return Status::OK;
}

// std::shared_ptr<GreetUser::Service> GreetServer::getService()
// {
//       return _service;
// }

int main()
{
      std::cout << "Server started..." << std::endl;
      std::string server_address("0.0.0.0:50051");
      // RouteGuideImpl service(db_path);
      GreetServer greetService;

      ServerBuilder builder;
      builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
      builder.RegisterService(&greetService);
      std::unique_ptr<Server> server(builder.BuildAndStart());
      std::cout << "Server listening on " << server_address << std::endl;
      server->Wait();

      return 0;
}