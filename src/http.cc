#include "http.h"
#include <cmath>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <ios>
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
  length = 0;
  std::string path = "public" + req.reqline.target;
  bool found = false;

  // if the path exists and its a directory, adds '/index.html'
  if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
    path += "/index.html";
  }

  std::stringstream buf;

  // if the path exists and is a regular file, saves its content in a buffer
  if (std::filesystem::exists(path) && std::filesystem::is_regular_file(path)) {
    found = true;
    std::ifstream file(path);
    buf << file.rdbuf();
    file.close();
  } else {
    std::cout << "File not found." << std::endl;
  }

  // if is GET
  if (req.reqline.method == "GET" || req.reqline.method == "HEAD") {
    if (found) {
      strline.status = 200;
      strline.message = "OK";
      set_mime_type(path);

      length = buf.str().size();

      if (req.reqline.method == "GET") {
        body = buf.str();
      }
    } else {
      std::cout << "File not found." << std::endl;
      strline.status = 404;
      strline.message = "Not Found";

      std::string err_msg = "<html><body><h1>Not found :(</h1></body></html>";

      if (req.reqline.method == "GET") {
        body = err_msg;
      }

      length = err_msg.size();
    }
  } else {
    // OTHER METHODS SHOULD GO HERE
    std::cout << "Not a GET/HEAD request." << std::endl;
    strline.status = 501;
    strline.message = "Not Implemented";

    body = "<html><body><h1>501 - Not Implemented :(</h1></body></html>";
  }
  time_t t = time(nullptr);
  tm *gmt = gmtime(&t);
  std::stringstream datetime;
  datetime << std::put_time(gmt, "%a, %d %b %Y %H:%M:%S GMT");
  headers->insert({"Date", datetime.str()});
  headers->insert({"Content-Length", std::to_string(length)});
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
