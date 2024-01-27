#include <grpc/grpc.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include <iostream>
#include <string>

#include "mx.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;

using ServerStreamEX::EvenFlag;
using ServerStreamEX::RandomGenerator;
using ServerStreamEX::RandomNumber;

class RandomGeneratorServer final : public RandomGenerator::Service
{

public:
    explicit RandomGeneratorServer();
    ~RandomGeneratorServer() override {}
    ::grpc::Status GenerateRandomNumber(::grpc::ServerContext*, const ::ServerStreamEX::EvenFlag*, ::grpc::ServerWriter< ::ServerStreamEX::RandomNumber>*) override;

private:
};