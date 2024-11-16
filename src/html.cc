#include "html.h"
#include <fstream>
#include <sstream>
#include <string>

std::string HTML::generate_from_file(std::string path) {
  std::stringstream buf;

  std::ifstream file(path);
  buf << file.rdbuf();
  file.close();

  return buf.str();
}

std::string HTML::generate_not_found() {
  return "<html><body><h1>Not found</h1><p>The server cannot find the requested resource.</p></body></html>";
}

std::string HTML::generate_not_implemented() {
  return "<html><body><h1>Not implemented</h1><p>The request method is not supported by the server and cannot be handled.</p></body></html>";
}
