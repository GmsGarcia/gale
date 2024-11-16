#include "http.h"
#include "html.h"
#include "utils.h"
#include <cmath>
#include <cstdio>
#include <filesystem>
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

  parse_headers(stream);
}

void HttpRequest::parse_request_line(const std::string &line) {
  std::istringstream stream(line);
  if (!(stream >> reqline.method >> reqline.target >> reqline.version)) {
    // internal error [500] here... // couldn't parse the headers
    perror("Failed to parse request line");
  }
}

void HttpRequest::parse_headers(std::istringstream &stream) {
  std::string line;
  headers = Headers{};

  while (std::getline(stream, line) && !line.empty() && line != "\r") {
    if (line.back() == '\r') {
      line.pop_back();
    }

    std::size_t pos = line.find(": ");

    if (pos != std::string::npos) {
      std::string header_key = line.substr(0, pos);
      std::string header_value = line.substr(pos + 2);
      headers->insert({header_key, header_value});
    }
  }
}

void HttpResponse::generate(HttpRequest &req) {
  strline.version = "HTTP/1.1";
  length = 0;
  std::string path = "public" + req.reqline.target;
  bool found = false;

  headers = Headers{};

  // check if its a directory, and if its a directory and path doesnt end with "/" redirects...
  if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
    if (req.reqline.target.back() != '/') {
      strline.status = 301;
      strline.message = "Moved Permanently";
      headers->insert({"Location", req.reqline.target + "/"});
      return;
    }
    path += "index.html";
  } else if (!std::filesystem::exists(path)) {
    // check if appending ".html" makes it a valid path
    std::string html_path = path + ".html";
    if (std::filesystem::exists(html_path) && std::filesystem::is_regular_file(html_path)) {
      path = html_path;
    }
  }

  std::string html;

  // if the path exists and is a regular file, get file content
  if (std::filesystem::exists(path) && std::filesystem::is_regular_file(path)) {
    found = true;
    html = HTML::generate_from_file(path);
  }

  if (req.reqline.method == "GET" || req.reqline.method == "HEAD") {
    if (found) {
      strline.status = 200;
      strline.message = "OK";
      set_mime_type(path);
    } else {
      strline.status = 404;
      strline.message = "Not Found";
      html = HTML::generate_not_found();
    }
  } else {
    // OTHER METHODS SHOULD GO HERE
    strline.status = 501;
    strline.message = "Not Implemented";
    html = HTML::generate_not_implemented();
  }

  if (req.reqline.method == "GET") {
    body = html;
  }
  length = html.size();

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
    std::string mime = get_mime_from_ext(ext);

    if (!mime.empty()) {
      headers->insert({"Content-Type", mime});
    }
  }
}

std::string HttpResponse::as_string() {
  std::stringstream ss;
  ss << strline.version << " " << strline.status << " " << strline.message << "\r\n";

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
