#ifndef SERVER_H
#define SERVER_H

#include "logger.h"
#include <cstdint>
#include <cstdio>
#include <netinet/in.h>
#include <string>

class HttpServer {
private:
  std::string _host;
  uint16_t _port;
  bool _running;
  int _sock_fd;
  Logger logger;
  void stop();
  void listen();
  void handle_connection(int client_fd, sockaddr_in client_addr, socklen_t client_len);
  std::string generate_response(const std::string &req);
  int get_port() { return _port; }

public:
  void start(uint16_t port);
  bool is_running() { return _running; }
};

#endif
