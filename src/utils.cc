#include "utils.h"
#include <cstdlib>
#include <string>

std::string get_mime_from_ext(std::string ext) {
  if (ext == "txt") {
    return "text/plain";
  } else if (ext == "css" || ext == "csv" || ext == "html") {
    return "text/" + ext;
  } else if (ext == "js" || ext == "mjs") {
    return "text/javascript";
  } else if (ext == "json") {
    return "application/" + ext;
  } else if (ext == "png" || ext == "jpeg" || ext == "gif" || ext == "bmp") {
    return "image/" + ext;
  } else if (ext == "jpg") {
    return "image/jpeg";
  } else {
    return "application/octet-stream";
  }
}
