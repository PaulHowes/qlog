/** @file qlog.hpp */

#pragma once
#include <iostream>
#include <sys/time.h>

/** @namespace qlog */
namespace qlog {

  /**
   * @brief Log message severity levels.
   */
  enum class severity {
    /** @brief Error level. */
    ERROR,
    /** @brief Warning level. */
    WARNING,
    /** @brief Informational level. */
    INFO,
    /** @brief Debug level. */
    DEBUG
  };

  /**
   * @brief Simple logging class.
   */
  class logger {
    public:
      /**
       * @brief Constructs a new instance of @c logger.
       * @param[in] o Stream to which logging output is sent.
       * @param[in] v Default verbosity level of the log.
       */
      logger(std::ostream& o = std::cerr,
             severity v = severity::DEBUG)
        : _output(&o), _severity(severity::DEBUG), _verbosity(v) {
      }

      /**
       * @brief Destructor.
       */
      ~logger() {
        // Adds a newline and flushes the stream.
        (*_output) << std::endl;
      }

      /**
       * @brief Insertion operator that accepts any constant value.
       * @param[in] o Constant value to emit.
       * @returns a reference to the @c logger object for chaining.
       */
      template<typename T>
      logger& operator<<(const T& o) {
        if(_severity <= _verbosity) {
          (*_output) << o;
        }
        return *this;
      }

      /**
       * @brief Insertion operator that accepts any object value.
       * @param[in] o Value to emit.
       * @returns a reference to the @c logger object for chaining.
       */
      template<typename T>
      logger& operator<<(T& o) {
        if(_severity <= _verbosity) {
          std::cout << o;
        }
        return *this;
      }

      /**
       * @brief Specialized insertion operator that accepts log manipulators.
       * @param[in] p Pointer to the log manipulator function.
       * @returns a reference to the @c logger object for chaining.
       */
      logger& operator<<(logger& (*p)(logger&)) {
        return p(*this);
      }

      /**
       * @brief Specialized insertion operator that accepts @c std::ostream manipulators.
       * @param[in] p Pointer to the @c std::ostream manipulator function.
       * @returns a reference to the @c logger object for chaining.
       */
      logger& operator<<(std::ostream& (*p)(std::ostream&)) {
        p(*_output);
        return *this;
      }

    private:

      /**
       * @brief Sets the new logging level.
       * @param[in] l The new logging level.
       * @returns a reference to the @c logger object for chaining.
       */
      logger& set_severity(const severity l) {
        _severity = l;
        return *this;
      }

      /**
       * @brief Returns a timestamp.
       * @returns A @c NULL terminated character array that contains a timestamp.
       */
      const std::string timestamp() {
        // Retrieve the time with microsecond accuracy.
        struct timeval t;
        struct timezone tz;
        gettimeofday(&t, &tz);

        // Converts `t.tv_sec`, which is the time in seconds since the epoch to a `struct tm`
        struct tm time_info;
        gmtime_r(&t.tv_sec, &time_info);

        // Format as a string.
        const size_t maxsize = 80;
        char time_str[maxsize];
        strftime(time_str, maxsize, "%Y-%m-%dT%H:%M:%S", &time_info);

        // Append the milliseconds and the `Z`
        sprintf(time_str, "%s.%dZ", time_str, t.tv_usec / 1000);

        // Done.
        return time_str;
      }

      /** @brief Stream that receives log messages. */
      std::ostream* _output;

      /** @brief Current message severity level. */
      severity _severity;

      /** @brief Current log verbosity level. */
      severity _verbosity;

      /*
       * The log manipulators need access to logger::timestamp() and logger::set_severity()
       */
      friend logger& error(logger&);
      friend logger& warning(logger&);
      friend logger& info(logger&);
      friend logger& debug(logger&);
  };

  /**
   * @brief Log manipulator that changes output to the @c ERROR severity level.
   * @param[in] l Instance of @c logger that invoked the manipulator.
   * @returns a reference to the @c logger object for chaining.
   */
  logger& error(logger& l) {
    l.set_severity(severity::ERROR);
    l << l.timestamp() << " [ERROR] ";
    return l;
  }

  /**
   * @brief Log manipulator that changes output to the @c WARNING severity level.
   * @param[in] l Instance of @c logger that invoked the manipulator.
   * @returns a reference to the @c logger object for chaining.
   */
  logger& warning(logger& l) {
    l.set_severity(severity::WARNING);
    l << l.timestamp() << " [WARNING] ";
    return l;
  }

  /**
   * @brief Log manipulator that changes output to the @c INFO severity level.
   * @param[in] l Instance of @c logger that invoked the manipulator.
   * @returns a reference to the @c logger object for chaining.
   */
  logger& info(logger& l) {
    l.set_severity(severity::INFO);
    l << l.timestamp() << " [INFO] ";
    return l;
  }

  /**
   * @brief Log manipulator that changes output to the @c DEBUG severity level.
   * @param[in] l Instance of @c logger that invoked the manipulator.
   * @returns a reference to the @c logger object for chaining.
   */
  logger& debug(logger& l) {
    l.set_severity(severity::DEBUG);
    l << l.timestamp() << " [DEBUG] ";
    return l;
  }
}

