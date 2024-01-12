#pragma once

#include <vector>
#include <string>

#include <grpcpp/grpcpp.h>
#include <grpc++/grpc++.h>
#include "conv-agent/conv_agent_input_service.grpc.pb.h"

class ConvAgentServiceClient {
 public:
  ConvAgentServiceClient(std::shared_ptr<grpc::Channel> channel);
  kuka::conv_agent::SendMessageResponse sendMessageInput(const std::string& a, float probability);
  kuka::conv_agent::SendMessageResponse sendMessageWakeword(const std::string& a, float probability, bool prompt);
  kuka::conv_agent::SendMessageResponse sendMessageOutput(const std::string& a);

 private:
  std::unique_ptr<kuka::conv_agent::ConvAgentService::Stub> stub_;
};

void sendToGrpcServerInput(const std::string& text, float probability);
void sendToGrpcServerWakeword(const std::string& text, float probability, bool prompt);
void sendToGrpcServerOutput(const std::string& text);
