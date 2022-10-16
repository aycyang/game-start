#include "log.h"

#include <string>
#include <iostream>

Log::Log(std::string label, std::ostream& out) : out_(out), label_(label) {}

Log::Ephemera Log::Spawn() const {
  return Log::Ephemera(out_);
}

Log::Ephemera::Ephemera(std::ostream& out) : out_(out), is_alive_(true) {}

Log::Ephemera::Ephemera(Log::Ephemera&& rhs) : out_(rhs.out_), is_alive_(true) {
  rhs.is_alive_ = false;
}
Log::Ephemera::~Ephemera() {
  if (is_alive_) {
    out_ << std::endl;
  }
}
