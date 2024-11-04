#include "logger.h"
#include "http.h"
#include "utils.h"
#include <arpa/inet.h>
#include <cmath>
#include <iostream>
#include <string>

void Logger::log_request(HttpRequest &req, HttpResponse &res) {
  // if the logger is disabled, returns
  if (!_active) {
    return;
  }

  int status = res.strline.status;
  std::string code = "";

  // format code text with some fancy colors :P
  if (status >= 200 && status < 300) {
    code = "\033[102m\033[1m\033[30m" + std::to_string(status) + "\033[0m";
  } else if (status >= 400 && status < 500) {
    code = "\033[103m\033[1m\033[30m" + std::to_string(status) + "\033[0m";
  } else if (status >= 500) {
    code = "\033[101m\033[1m\033[30m" + std::to_string(status) + "\033[0m";
  }

  // join everything
  std::string msg = code + " | " + inet_ntoa(req.client.host.sin_addr) + " | " +
                    req.reqline.method + " " + req.reqline.target;
  // then: voilá
  log(msg);
}

void Logger::log(std::string txt) {
  std::string timestamp = get_current_timestamp();
  std::cout << "[GALE] " << timestamp << " | " << txt << std::endl;
}
