#ifndef SERVER_H
#define SERVER_H

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
  std::string generate_response(const std::string& req);
  int getPort() { return _port; }
public:
  HttpServer();
  void start();
  bool isRunning() { return _running; }
};

#endif
