#include "conv-agent-client.h"

ConvAgentServiceClient::ConvAgentServiceClient(std::shared_ptr<grpc::Channel> channel)
    : stub_(kuka::conv_agent::ConvAgentService::NewStub(channel)) {}

std::string ConvAgentServiceClient::sendRequest(const std::string& a) {
  kuka::conv_agent::MessageRequest request;
  request.set_message(a);

  kuka::conv_agent::MessageResponse reply;

  grpc::ClientContext context;

  grpc::Status status = stub_->SendMessage(&context, request, &reply);

  if (status.ok()) {
    return reply.message();
  } else {
    std::cout << status.error_code() << ": " << status.error_message() << std::endl;
    return "RPC Failed";
  }
}

void sendToGrpcServer(const std::string& text) {
  std::string target_address("0.0.0.0:50051");
  ConvAgentServiceClient client(grpc::CreateChannel(target_address, grpc::InsecureChannelCredentials()));
  std::string response = client.sendRequest(text);
  std::cout << "Original string: " << text << std::endl;
  std::cout << "Received string: " << response << std::endl;
}
