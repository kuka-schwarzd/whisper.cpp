
#include <grpcpp/grpcpp.h>
#include <grpc++/grpc++.h>
#include "conv_agent_input_service.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using kuka::conv_agent::MessageResponse;
using kuka::conv_agent::MessageRequest;
using kuka::conv_agent::ConvAgentService;


class ConvAgentServiceClient {
 public:
  ConvAgentServiceClient(std::shared_ptr<Channel> channel)
      : stub_(kuka::conv_agent::ConvAgentService::NewStub(channel)) {}

  // Assembles client payload, sends it to the server, and returns its response
  std::string sendRequest(const std::string& a) {
    // Data to be sent to server
    kuka::conv_agent::MessageRequest request;
    request.set_message(a);

    // Container for server response
    kuka::conv_agent::MessageResponse reply;

    // Context can be used to send metadata to the server or modify RPC behavior
    ClientContext context;

    // Actual Remote Procedure Call
    Status status = stub_->SendMessage(&context, request, &reply);

    // Returns results based on RPC status
    if (status.ok()) {
      return reply.message();
    } else {
      std::cout << status.error_code() << ": " << status.error_message() << std::endl;
      return "RPC Failed";
    }
  }

 private:
  std::unique_ptr<kuka::conv_agent::ConvAgentService::Stub> stub_;
};

// Function to send text to the gRPC server
void sendToGrpcServer(const std::string& text) {
  // Replace "0.0.0.0:50051" with your actual gRPC server address
  std::string target_address("0.0.0.0:50051");

  // Instantiates the client
  ConvAgentServiceClient client(
      // Channel from which RPCs are made - endpoint is the target_address
      grpc::CreateChannel(target_address,
                          // Indicate when channel is not authenticated
                          grpc::InsecureChannelCredentials()));

  // RPC is created and response is stored
  std::string response = client.sendRequest(text);

  // Prints results
  std::cout << "Original string: " << text << std::endl;
  std::cout << "Received string: " << response << std::endl;
}
