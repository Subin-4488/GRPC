#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "chat.grpc.pb.h"

using grpc::Channel;
using grpc::ChannelInterface;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;

using chatproto::ChatMessage;
using chatproto::ChatService;

class ChatClient
{
public:
    explicit ChatClient(std::shared_ptr<ChannelInterface> interface);
    void chatToServer();

private:
    std::unique_ptr<ChatService::Stub> stub_;
    std::mutex _lock;
    std::condition_variable cv;
};
