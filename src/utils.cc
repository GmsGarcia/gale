#include "utils.h"
#include <chrono>
#include <cstdlib>
#include <iomanip>
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

std::string get_current_timestamp() {
  auto now = std::chrono::system_clock::now();
  std::time_t now_time = std::chrono::system_clock::to_time_t(now);

  std::tm local_tm = *std::localtime(&now_time);

  std::ostringstream oss;
  oss << std::put_time(&local_tm, "%Y/%m/%d %H:%M:%S");

  return oss.str();
}
