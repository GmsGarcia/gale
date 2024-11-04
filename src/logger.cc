#include "logger.h"
#include "http.h"
#include <arpa/inet.h>
#include <iostream>

void Logger::log_request(HttpRequest &req, HttpResponse &res) {
  std::cout << res.strline.status << " - "
            << inet_ntoa(req.client.host.sin_addr) << " requested "
            << req.reqline.target << std::endl;
}
