#include "chatclient.h"

ChatClient::ChatClient(std::shared_ptr<ChannelInterface> interface) : stub_(ChatService::NewStub(interface))
{
}

void ChatClient::chatToServer()
{
    ClientContext context;
    std::shared_ptr<ClientReaderWriter<ChatMessage, ChatMessage>> clientStream(
        stub_->communicate(&context));

    while (true)
    {
        std::unique_lock<std::mutex> lock(_lock);
        cv.wait_for(lock, std::chrono::seconds(2) ,[]{return false;});
        std::string input;
        std::cout << "Enter message to sent to server(" << std::this_thread::get_id() << "):" << std::endl;

        getline(std::cin, input);

        ChatMessage msg;
        msg.set_msg(input);

        clientStream->Write(msg);
        // clientStream->WritesDone();
        cv.notify_all();

        clientStream->Read(&msg);

        std::cout << "SERVER(" << std::this_thread::get_id() << "):" << msg.msg() << std::endl;
    }
}

int main()
{
    ChatClient client(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));

    std::thread t[3];

    for (int i = 0; i < 3; i++)
        t[i] = std::thread(&ChatClient::chatToServer, &client);

    for (int i = 0; i < 3; i++)
        t[i].join();

    return 0;
}
