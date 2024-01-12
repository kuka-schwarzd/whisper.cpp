#include "conv-agent/conv-agent-client.h"

ConvAgentServiceClient::ConvAgentServiceClient(std::shared_ptr<grpc::Channel> channel)
    : stub_(kuka::conv_agent::ConvAgentService::NewStub(channel)) {}

kuka::conv_agent::SendMessageResponse ConvAgentServiceClient::sendMessageInput(const std::string& text, float probability) {
    kuka::conv_agent::Message message;
    kuka::conv_agent::Input* input = message.mutable_input();
    input->mutable_content()->set_text(text);
    input->set_probability(probability);

    kuka::conv_agent::SendMessageRequest request;
    request.set_allocated_message(new kuka::conv_agent::Message(message));  // Allocate a new message

    kuka::conv_agent::SendMessageResponse reply;

    grpc::ClientContext context;

    grpc::Status status = stub_->SendMessage(&context, request, &reply);

    if (!status.ok()) {
        std::cout << status.error_code() << ": " << status.error_message() << std::endl;
    }

    // No need to manually delete the allocated message; set_allocated_message takes care of it.

    return reply;
}

kuka::conv_agent::SendMessageResponse ConvAgentServiceClient::sendMessageWakeword(const std::string& text, float probability, bool prompt) {
    kuka::conv_agent::Message message;
    kuka::conv_agent::Wakeword* ww = message.mutable_wakeword();
    ww->mutable_content()->set_text(text);
    ww->set_prompt(prompt);
    ww->set_probability(probability);

    kuka::conv_agent::SendMessageRequest request;
    request.set_allocated_message(new kuka::conv_agent::Message(message));  // Allocate a new message

    kuka::conv_agent::SendMessageResponse reply;

    grpc::ClientContext context;

    grpc::Status status = stub_->SendMessage(&context, request, &reply);

    if (!status.ok()) {
        std::cout << status.error_code() << ": " << status.error_message() << std::endl;
    }

    // No need to manually delete the allocated message; set_allocated_message takes care of it.

    return reply;
}

kuka::conv_agent::SendMessageResponse ConvAgentServiceClient::sendMessageOutput(const std::string& text) {
    kuka::conv_agent::Message message;
    kuka::conv_agent::Output* output = message.mutable_output();
    output->mutable_content()->set_text(text);

    kuka::conv_agent::SendMessageRequest request;
    request.set_allocated_message(new kuka::conv_agent::Message(message));  // Allocate a new message

    kuka::conv_agent::SendMessageResponse reply;

    grpc::ClientContext context;

    grpc::Status status = stub_->SendMessage(&context, request, &reply);

    if (!status.ok()) {
        std::cout << status.error_code() << ": " << status.error_message() << std::endl;
    }

    // No need to manually delete the allocated message; set_allocated_message takes care of it.

    return reply;
}

void sendToGrpcServerInput(const std::string& text, float probability) {
    std::string target_address("0.0.0.0:50051");
    ConvAgentServiceClient client(grpc::CreateChannel(target_address, grpc::InsecureChannelCredentials()));
    kuka::conv_agent::SendMessageResponse response = client.sendMessageInput(text, probability);
    std::cout << "Input: " << text << std::endl;
}

void sendToGrpcServerWakeword(const std::string& text, float probability, bool prompt) {
    std::string target_address("0.0.0.0:50051");
    ConvAgentServiceClient client(grpc::CreateChannel(target_address, grpc::InsecureChannelCredentials()));
    kuka::conv_agent::SendMessageResponse response = client.sendMessageWakeword(text, probability, prompt);
    std::cout << "Wakeword: " << text << ", Prompt: " << (prompt ? "true" : "false") << std::endl;
}

void sendToGrpcServerOutput(const std::string& text) {
    std::string target_address("0.0.0.0:50051");
    ConvAgentServiceClient client(grpc::CreateChannel(target_address, grpc::InsecureChannelCredentials()));
    kuka::conv_agent::SendMessageResponse response = client.sendMessageOutput(text);
    std::cout << "Output: " << text << std::endl;
}
