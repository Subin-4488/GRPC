#include "server.h"

RandomGeneratorServer::RandomGeneratorServer() {}

Status RandomGeneratorServer::GenerateRandomNumber(ServerContext *context,
const EvenFlag * flag,
::grpc::ServerWriter<RandomNumber> *writer)
{
      std::cout << "Client connected!" <<std::endl;
    for (int i=0; i<100; i++){
        if (flag->flag() && i%2==0){
            RandomNumber r;
            r.set_rnumber(i);
            writer->Write(r);
        }
        else if (!flag->flag() && i%2!=0){
            RandomNumber r;
            r.set_rnumber(i);
            writer->Write(r);
        }
    }
    return Status::OK;
}



int main()
{
      std::cout << "Server starts" << std::endl;
      std::string server_address("127.0.0.1:50051");
      RandomGeneratorServer greetService;

      ServerBuilder builder;
      builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
      builder.RegisterService(&greetService);
      std::unique_ptr<Server> server(builder.BuildAndStart());
      std::cout << "Server listening on " << server_address << std::endl;
      server->Wait();

      return 0;
}
