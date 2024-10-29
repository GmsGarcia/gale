#ifndef HTTP_H
#define HTTP_H

#include <cstdint>
#include <map>
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

typedef std::map<std::string, std::string> Headers;

class HttpRequest {
public:
  RequestLine reqline;
  std::optional<Headers> headers;
  std::optional<std::string> body;
  void parse(std::string raw);

private:
  bool parseReqLine(const std::string &line);
  bool parseHeaders(std::istringstream &stream);
  bool parseBody(std::istringstream &stream);
};

class HttpResponse {
public:
  StartLine strline;
  std::optional<Headers> headers;
  std::optional<std::string> body;
  void generate(HttpRequest &req);
  std::string as_string();
  uint length();
};

#endif
