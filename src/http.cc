#include "http.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

void HttpRequest::parse(std::string raw) {
  std::istringstream stream(raw);
  std::string line;

  if (std::getline(stream, line)) {
    parse_request_line(line);
  }
}

bool HttpRequest::parse_request_line(const std::string &line) {
  std::istringstream stream(line);
  if (!(stream >> reqline.method >> reqline.target >> reqline.version)) {
    perror("Failed to parse request line");
    return false;
  }
  return true;
}

void HttpResponse::generate(HttpRequest &req) {

  strline.version = "HTTP/1.1";
  headers = Headers{};

  // if is GET
  if (req.reqline.method == "GET") {
    strline.status = 200;
    strline.message = "OK";

    std::string path = "public" + req.reqline.target;

    if (path[path.size() - 1] == '/') {
      path = path + "/index.html";
    }

    std::ifstream file(path);

    // check if file exists, if it doesn't, checks if its a directory with an
    // index.html file inside
    if (file.is_open()) {
      std::stringstream buf;
      buf << file.rdbuf();
      body = buf.str();
      file.close();
      set_mime_type(path);
    } else {
      path = path + "/index.html";

      std::ifstream file(path);

      if (file.is_open()) {
        std::stringstream buf;
        buf << file.rdbuf();
        body = buf.str();
        file.close();
        set_mime_type(path);
      } else {
        std::cout << "File not found." << std::endl;
        strline.status = 404;
        strline.message = "Not Found";
        body = "<html><body><h1>Not found :(</h1></body></html>";
      }
    }
  } else {
    // OTHER METHODS SHOULD GO HERE
    std::cout << "Not a GET request." << std::endl;
    strline.status = 404;
    strline.message = "Not Found";

    body = "<html><body><h1>Not found :(</h1></body></html>";
  }

  headers->insert({"Content-Length", std::to_string(body->size())});
  headers->insert({"Connection", "close"});
}

void HttpResponse::set_mime_type(std::string target) {
  size_t dot_pos = target.find_last_of('.');

  if (dot_pos != std::string::npos && dot_pos + 1 < target.size()) {
    std::string ext = target.substr(dot_pos + 1);
    std::string mime = "";

    if (ext == "txt") {
      mime = "text/plain";
    } else if (ext == "css" || ext == "csv" || ext == "html") {
      mime = "text/" + ext;
    } else if (ext == "js" || ext == "mjs") {
      mime = "text/javascript";
    } else if (ext == "json") {
      mime = "application/" + ext;
    } else if (ext == "png" || ext == "jpeg" || ext == "gif" || ext == "bmp") {
      mime = "image/" + ext;
    } else if (ext == "jpg") {
      mime = "image/jpeg";
    } else {
      mime = "application/octet-stream";
    }

    if (!mime.empty()) {
      headers->insert({"Content-Type", mime});
    }
  }
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
