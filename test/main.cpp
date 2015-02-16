#include <qlog.hpp>

int main() {
  qlog::logger log;
  log(qlog::error) << "This is an error message.";
  log(qlog::warn) << "This is a warning message.";
  log(qlog::info) << "This is an informational message.";
  log(qlog::debug) << "This is a debug message.";
  return 0;
}
