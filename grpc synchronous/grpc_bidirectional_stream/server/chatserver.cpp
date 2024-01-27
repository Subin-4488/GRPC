#include "chatserver.h"

ChatServer::ChatServer() {}

ChatServer::~ChatServer() {}

Status ChatServer::communicate(ServerContext *context, ::grpc::ServerReaderWriter<ChatMessage, ChatMessage> *stream)
{
    ChatMessage msg;

    while (stream->Read(&msg)){
        std::cout <<"From Client: "<< msg.msg() <<std::endl;

        //lockguard here

        //
        std::string input;
        std::cout << "Enter message to be sent: \n";
        getline(std::cin, input);
        msg.set_msg(input);
        stream->Write(msg);
    }

    return Status::OK;
}

int main()
{
    std::string server_address("127.0.0.1:50051");
    ChatServer service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
    return 0;
}
