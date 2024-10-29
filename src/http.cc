#include "http.h"
#include <cstdio>
#include <filesystem>
#include <fstream>
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
  if (!(stream >> reqline.method >> reqline.target >> reqline.version)) {
    perror("Failed to parse request line");
    return false;
  }

  return true;
}

bool HttpRequest::parseHeaders(std::istringstream &stream) { return true; }

void HttpResponse::generate(HttpRequest &req) {

  strline.version = "HTTP/1.1";
  headers = Headers{};

  // if is GET
  if (req.reqline.method == "GET") {
    strline.status = 200;
    strline.message = "OK";

    std::string path = req.reqline.target;

    
    if (path == "/") {
      path = "public/index.html";
    } else {
      path = "public" + path;
    }

    std::ifstream file(path);

    if (file.is_open()) {
      std::stringstream buf;
      buf << file.rdbuf();
      body = buf.str();
      file.close();

    } else {
      std::cout << "File not found." << std::endl;
      strline.status = 404;
      strline.message = "Not Found";

      body = "<html><body><h1>Not found :(</h1></body></html>";
    }
  } else {
    // OTHER METHODS SHOULD GO HERE
    std::cout << "Not a GET request." << std::endl;
    strline.status = 404;
    strline.message = "Not Found";

    body = "<html><body><h1>Not found :(</h1></body></html>";
  }

  headers->insert({"Content-Type", "text/html"});
  headers->insert({"Content-Length", std::to_string(body->size())});
  headers->insert({"Connection", "close"});
}

std::string HttpResponse::as_string() {
  std::stringstream ss;
  ss << strline.version << " " << strline.status << " " << strline.message
     << "\r\n";

  if (headers) {
    for (const auto &[key, val] : headers.value()) {
      ss << key << ": " << val << "\r\n";
    }
  }

  // separate line between headers and body
  ss << "\r\n";

  if (body) {
    ss << body.value();
  }

  return ss.str();
}
