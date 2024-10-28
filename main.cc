#include "src/server.h"

using namespace std;

int main() {
  HttpServer server = HttpServer();
  server.start();
}
