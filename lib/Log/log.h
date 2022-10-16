#ifndef LOG_H_
#define LOG_H_

#include <string>
#include <iostream>

class Log {
 public:
  Log(std::string = "", std::ostream& = std::cout);
  class Ephemera {
   public:
    Ephemera(std::ostream&);
    Ephemera(Ephemera&&);
    ~Ephemera();
    std::ostream& out() const { return out_; }
   private:
    std::ostream& out_;
    bool is_alive_;
  };
  const std::string& label() const { return label_; }
  std::ostream& out() const { return out_; }
  Ephemera Spawn() const;
 private:
  std::ostream& out_;
  std::string label_;
};

template <typename T>
Log::Ephemera operator<<(Log::Ephemera&& ephemera, const T& v) {
  ephemera.out() << v;
  return std::move(ephemera);
}

template <typename T>
Log::Ephemera operator<<(Log&& log, const T& v) {
  return log << v;
}

template <typename T>
Log::Ephemera operator<<(Log& log, const T& v) {
  if (!log.label().empty()) {
    log.out() << log.label() << ": ";
  }
  return std::move(log.Spawn() << v);
}

#endif  // LOG_H_
