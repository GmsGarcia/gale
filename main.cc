#include "src/server.h"
#include <cstdint>

using namespace std;

int main() {
  uint16_t port = 8080;
  HttpServer server;
  server.start(port);
}
