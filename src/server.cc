#include "server.h"
#include <cstdio>
#include <iostream>
#include <netinet/in.h>
#include <numeric>
#include <ostream>
#include <string>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

// HttpServer 's constructor
HttpServer::HttpServer () {}

void HttpServer::start() {
  std::cout << "Starting TCP server...\n";
  _running = true;

  _sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  // CREATE SOCKET
  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(8000);
  serverAddress.sin_addr.s_addr = INADDR_ANY;

  if (bind(_sock_fd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) <
      0) {
    perror("Failed to bind socket.");
    close(_sock_fd);
    return;
  }

  if (::listen(_sock_fd, 5) < 0) {
    perror("Failed to listen to socket.");
    close(_sock_fd);
    return;
  }

  // [NEW THREAD] HANDLE CONNECTIONS
  std::thread listener(&HttpServer ::listen, this);
  listener.detach();

  std::cout << "The TCP server is now running. Type 'help' for a list of all "
               "commands.\n";

  // TUI COMMANDS
  while (true) {
    std::string cmd;
    std::cin >> cmd;

    if (cmd == "quit" || cmd == "exit") {
      std::cout << "Stopping TCP server...\n";
      stop();
      std::cout << "The server stopped successfully.\n";
    } else if (cmd == "help") {
      std::cout << "COMMANDS LIST:\n";
      std::cout << "quit / stop / exit - Stops the server.\n";
    } else {
      std::cout << "Command '" << cmd << "' not found.\n";
    }
  }
}

void HttpServer::listen() {
  while (_running) {
    sockaddr_in clientAddress;
    socklen_t clientLen = sizeof(clientAddress);
    int client_fd =
        accept(_sock_fd, (struct sockaddr *)&clientAddress, &clientLen);

    if (client_fd >= 0) {
      std::cout << "Client connected.\n";
      handle_connection(client_fd);
      close(client_fd);
    } else {
      if (_running) {
        perror("Failed to accept connection.");
      }
    }

    std::this_thread::sleep_for(
        std::chrono::milliseconds(1000)); // sleep 1s every iteration
  }
}

void HttpServer::handle_connection(int client_fd) {
  char buf[1024] = {0};

  recv(client_fd, buf, sizeof(buf), 0);
  std::cout << "Requested: " << buf << std::endl;

  std::string res = generate_response(buf);
  send(client_fd, res.c_str(), res.length(), 0);
  close(client_fd);
}

std::string HttpServer::generate_response(const std::string& req) {
  std::string res;
  std::string body = "<html><body><h1>Hello, world!</h1></body></html>";
  std::string e404 = "<html><body><h1>Not found :(</h1></body></html>";

  // if is GET
  if (req.substr(0,3) == "GET") {
    std::cout << "GET";
    res = "HTTP/1.1 200 OK\r\n"
          "Content-Type: text/html\r\n"
          "Content-Length: " + std::to_string(body.size()) + "\r\n"
          "Connection: close\r\n\r\n" +
          body;
  } else {
    std::cout << "ELSE";
    res = "HTTP/1.1 404 Not Found\r\n"
          "Content-Length: 0\r\n"
          "Connection: close\r\n\r\n" +
          e404;
  }
  return res;
}

void HttpServer::stop() { _running = false; }
