#include <CryptChat/CryptChatApp.h>

// External includes
#include <iostream>

#include <asio.hpp>
#include <asio/write.hpp>
#include <External/imgui/imgui.h>

using asio::ip::tcp;

CryptChatApp::CryptChatApp()
{
	try 
    {
        asio::io_context io_context;

        // Create an acceptor to listen on port 12345
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 12345));
        std::cout << "Server is listening on port 12345...\n";

        // Wait for a client connection
        tcp::socket socket(io_context);
        acceptor.accept(socket);
        std::cout << "Client connected!\n";

        // Read a message from the client
        char data[1024];
        size_t length = socket.read_some(asio::buffer(data));
        std::cout << "Received: " << std::string(data, length) << "\n";

        // Send a response
        std::string response = "Hello from server!";
        asio::write(socket, asio::buffer(response));

    }
	catch (std::exception& e) 
    {
        std::cerr << "Error: " << e.what() << "\n";
    }
}

CryptChatApp::~CryptChatApp() {}

void CryptChatApp::Update(float inDeltaTime)
{
	ImGui::Begin("CryptChat");

	ImGui::End();
}
