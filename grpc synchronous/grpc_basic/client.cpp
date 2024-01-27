#include "client.h"

int main()
{
      std::cout << "CLient..." << std::endl;
      Client client(grpc::CreateChannel("127.0.0.1:50051", grpc::InsecureChannelCredentials()));

      std::cout << "Channel created..." << std::endl;

      PersonData person1;
      person1.set_user(User::admin);
      person1.set_name("Subin David");
      person1.set_age(23);

      PersonData person2;
      person2.set_user(User::client);
      person2.set_name("Midhun David");
      person2.set_age(17);

      Greetings greetings;

      client.WishUser(person1, &greetings);
      client.WishUser(person2, &greetings);

      return 0;
}

Client::Client(std::shared_ptr<grpc::ChannelInterface> channel) : stub_(GreetUser::NewStub(channel))
{
}

void Client::WishUser(PersonData &personData, Greetings *msg)
{
      ClientContext context;
      Status status = stub_->WishUser(&context, personData, msg);

      if (!status.ok())
      {
            std::cout << "GetFeature rpc failed." << std::endl;
            //     return;
      }
      std::cout << status.error_message() << status.error_code() << std::endl;

      std::cout << msg->greetingsmessage() << std::endl;
}
