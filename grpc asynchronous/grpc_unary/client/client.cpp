#include "client.h"

int main()
{
      std::cout << "Client starts" << std::endl;

      Client client(grpc::CreateChannel("127.0.0.1:50051", grpc::InsecureChannelCredentials()));

      PersonData p1;
      p1.set_user(User::admin);
      p1.set_name("Subin");
      p1.set_age(23);
      Greetings g1;

      PersonData p2;
      p2.set_user(User::client);
      p2.set_name("Midhun");
      p1.set_age(18);
      Greetings g2;

      client.WishUser(p1, &g1);
      client.WishUser(p2, &g2);

      return 0;
}
Client::Client(std::shared_ptr<Channel> channel) : stub_(GreetUser::NewStub(channel))
{
}

void Client::WishUser(PersonData personData, Greetings *msg)
{
      std::cout << "requesting server..." << std::endl;
      ClientContext context;

      std::unique_ptr<ClientAsyncResponseReader<Greetings>> rpc(stub_->AsyncWishUser(&context, personData, &completionQueue));

      rpc->Finish(msg, &status, (void *)1);

      std::cout << "Wait on NEXT" << std::endl;
      completionQueue.Next(&tag, &ok);
      std::cout << "Wait on temp" << std::endl;

      if (status.ok() && tag == (void *)1)
      {
            std::cout << msg->greetingsmessage() << std::endl;
      }
      else
      {
            std::cout << "RPC failed " << status.error_details() << std::endl;
      }
}