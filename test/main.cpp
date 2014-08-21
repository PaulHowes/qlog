#include <qlog.hpp>

int main() {
  qlog::logger log;
  log << "This doesn't have a severity level." << std::endl;
  log << qlog::error << "This is an error message." << std::endl;
  log << qlog::warning << "This is a warning message." << std::endl;
  log << qlog::info << "This is an informational message." << std::endl;
  log << qlog::debug << "This is a debug message." << std::endl;
  log << "This is a continuation of the previous message." << std::endl;
  log << qlog::debug << "Specifying a severity level starts a new message." << std::endl;
  return 0;
}
