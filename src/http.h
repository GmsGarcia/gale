#ifndef HTTP_H
#define HTTP_H

#include <cstdint>
#include <cwchar>
#include <map>
#include <netinet/in.h>
#include <optional>
#include <sstream>
#include <string>
#include <sys/types.h>

struct RequestLine {
  std::string method;
  std::string target;
  std::string version;
};

struct StartLine {
  std::string version;
  uint16_t status;
  std::string message;
};

struct Client {
  sockaddr_in host;
  int port;
};

typedef std::map<std::string, std::string> Headers;

class HttpRequest {
public:
  RequestLine reqline;
  std::optional<Headers> headers;
  std::optional<std::string> body;
  Client client;
  void parse(std::string raw);

private:
  bool parse_request_line(const std::string &line);
  bool parse_headers(std::istringstream &stream);
  bool parse_body(std::istringstream &stream);
};

class HttpResponse {
public:
  StartLine strline;
  std::optional<Headers> headers;
  std::optional<std::string> body;
  void generate(HttpRequest &req);
  std::string as_string();
  uint length;

private:
  void set_mime_type(std::string ext);
};

#endif
