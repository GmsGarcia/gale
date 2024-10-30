#ifndef SERVER_H
#define SERVER_H

#include <cstdint>
#include <cstdio>
#include <string>

class HttpServer {
private:
  std::string _host;
  uint16_t _port;
  bool _running;
  int _sock_fd;
  void stop();
  void listen();
  void handle_connection(int client_fd);
  std::string generate_response(const std::string &req);
  int get_port() { return _port; }

public:
  void start(uint16_t port);
  bool is_running() { return _running; }
};

#endif
