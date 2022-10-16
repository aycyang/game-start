#include "log.h"

int main(int argc, char** argv) {
  Log() << "hello " << "world!";
  Log("hello") << "world!";
  {
    Log log("test");
    log << "hello";
    log << 1 << ", " << 2;
  }
  {
    Log log;
    log << "hello " << "world!";
  }
  {
    Log err("MyLabel", std::cerr);
    err << "error";
  }
}
