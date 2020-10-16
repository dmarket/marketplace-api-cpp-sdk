#include <iostream>
#include <grpcpp/grpcpp.h>

#include "api.pb.h"
#include "api.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using marketplace::SignInRequest;
using marketplace::AuthResponse;
using marketplace::MarketAPI;

class ApiClient {
public:
    ApiClient(std::shared_ptr<Channel> channel) : stub_(MarketAPI::NewStub(channel)) {}

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    std::string Login(const std::string &email, const std::string &password) {
        SignInRequest request;
        request.set_email(email);
        request.set_password(password);

        // Container for the data we expect from the server.
        AuthResponse reply;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        ClientContext context;

        Status status = stub_->SignIn(&context, request, &reply);

        // Act upon its status.
        if (status.ok()) {
            return reply.authtoken();
        } else {
            std::cout << "Err: " << status.error_message() << std::endl;
            std::cout << "Err details: " << status.error_details() << std::endl;
            return "RPC error";
        }
    }

private:
    std::unique_ptr<MarketAPI::Stub> stub_;
};


int main() {
    ApiClient api(grpc::CreateChannel("v1.marketplace.grpc-demo.dmarket.com:443", grpc::InsecureChannelCredentials()));

    std::string email("alexx.2409+95@gmail.com");
    std::string password("QWERTYasdf1234zxcv");

    std::string auth_token = api.Login(email, password);
    std::cout << "Token received: " << auth_token << std::endl;

    return 0;
}
