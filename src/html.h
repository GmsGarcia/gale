#ifndef HTML_H
#define HTML_H

#include "http.h"
#include <string>

class HTML {
public:
  static std::string generate_from_file(std::string path);
  static std::string generate_not_found();
  static std::string generate_not_implemented();
};

#endif
