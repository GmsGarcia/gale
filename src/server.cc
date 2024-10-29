#include "server.h"
#include "http.h"
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <netinet/in.h>
#include <ostream>
#include <string>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

void HttpServer::start(uint16_t port) {
  std::cout << "Starting TCP server...\n";
  _port = port;
  _running = true;

  _sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  // CREATE SOCKET
  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(_port);
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

  std::cout << "The TCP server is now running on port " << getPort() << ". Type 'help' for a list of all "
               "commands.\n";

  // TUI COMMANDS
  while (true) {
    std::string cmd;
    std::cin >> cmd;

    if (cmd == "quit" || cmd == "exit") {
      std::cout << "Stopping TCP server...\n";
      stop();
      exit(0);
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
  HttpRequest req;

  recv(client_fd, buf, sizeof(buf), 0);
  req.parse(buf);

  HttpResponse res;
  res.generate(req);
  std::string txt = res.as_string();

  send(client_fd, txt.c_str(), txt.size(), 0);
  close(client_fd);
}

void HttpServer::stop() {
  _running = false;
  if (_sock_fd >= 0) {
    close(_sock_fd);
    _sock_fd = -1;
  }
}
