#include "http.h"
#include <cstdio>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

void HttpRequest::parse(std::string raw) {
  std::istringstream stream(raw);
  std::string line;

  if (std::getline(stream, line)) {
    parseReqLine(line);
  }
}

bool HttpRequest::parseReqLine(const std::string &line) {
  std::istringstream stream(line);
  std::cout << line << std::endl;
  if (!(stream >> this->reqline.method >> this->reqline.target >>
        this->reqline.version)) {
    perror("Failed to parse request line");
    return false;
  }

  return true;
}

bool HttpRequest::parseHeaders(std::istringstream &stream) { return true; }
