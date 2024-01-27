#include "client.h"

int main()
{
    std::cout << "Client starts" << std::endl;

    Client client(grpc::CreateChannel("127.0.0.1:50051", grpc::InsecureChannelCredentials()));

    EvenFlag flag;
    flag.set_flag(true);
    client.GenerateRandomNumber(flag);
    
    flag.set_flag(false);
    client.GenerateRandomNumber(flag);

    return 0;
}
Client::Client(std::shared_ptr<ChannelInterface> channel) : stub_(RandomGenerator::NewStub(channel))
{
}

void Client::GenerateRandomNumber(EvenFlag &flag)
{
    ClientContext context;
    
    RandomNumber result;
    std::unique_ptr<ClientReader<RandomNumber>> reader(stub_->GenerateRandomNumber(&context, flag));

    while (reader->Read(&result))
    {
        std::cout << "From server: " << result.rnumber() << std::endl;
    }
    Status status = reader->Finish();
    if (status.ok()){
        std::cout <<"RPC Success\n";
    }
    else{
        std::cout <<"RPC Fails\n";
    }
}
