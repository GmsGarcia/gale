#include "http.h"

class Logger {
private:
  bool _active;

public:
  void enable() { _active = true; };
  void disable() { _active = false; };
  bool is_active() { return _active; };
  void log_request(HttpRequest &req, HttpResponse &res);
};
