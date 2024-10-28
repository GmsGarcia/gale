#ifndef HTTP_H
#define HTTP_H

#include <map>
#include <string>

class HttpRequest {
public:
  std::string method;
  std::string path;
  std::map<std::string, std::string> headers;
  void parseRequest();
};

class HttpResponse {

};

#endif
