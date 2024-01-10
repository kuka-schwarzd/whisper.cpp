#pragma once

#include <vector>
#include <string>

#include <grpcpp/grpcpp.h>
#include <grpc++/grpc++.h>
#include "conv_agent_input_service.grpc.pb.h"

class ConvAgentServiceClient {
 public:
  ConvAgentServiceClient(std::shared_ptr<grpc::Channel> channel);
  std::string sendRequest(const std::string& a);

 private:
  std::unique_ptr<kuka::conv_agent::ConvAgentService::Stub> stub_;
};

void sendToGrpcServer(const std::string& text);
