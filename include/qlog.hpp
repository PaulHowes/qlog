/** @file qlog.hpp */

#pragma once
#include <iostream>
#include <vector>
#include <sys/time.h>

/** @namespace qlog */
namespace qlog {

  /**
   * @brief Used to define log severity levels.
   *
   * Each log level has a name and a number. The number is used when comparing the current 
   * verbosity level with the severity level of a message. If the message being emitted is lower
   * than the verbosity level of the log, then it will not be visible.
   */
  struct severity_t {
    /**
     * @brief The severity level that is used in comparisons.
     */
    unsigned long level;

    /**
     * @brief The name of the severity.
     */
    char const* name;

    /**
     * @brief Initializes a new qlog::severity_t instance with a level and name.
     * @param[in] l Level of the new severity.
     * @param[in] n Name of the new severity.
     */
    severity_t(unsigned long l, char const* n) : level(l), name(n) { }
  };

  /**
   * @brief The qlog::none severity is intended to be used when setting the verbosity of the log to
   *        prevent any messages from being emitted.
   */
  const severity_t none (  0, "NONE");

  /**
   * @brief The message is for a catastrophic event that caused the program to terminate
   *        unexpectedly.
   */
  const severity_t fatal(100, "FATAL");

  /**
   * @brief The message is for an unexpected event that did not cause the program to terminate,
   *        but should be investigated.
   */
  const severity_t error(200, "ERROR");

  /**
   * @brief The message is for an event that may have been expected, but is not desired.
   */
  const severity_t warn (300, "WARN");

  /**
   * @brief The message is for informational purposes only.
   */
  const severity_t info (400, "INFO");

  /**
   * @brief The message is for debugging the program, and can otherwise be ignored.
   */
  const severity_t debug(500, "DEBUG");

  /**
   * @brief The qlog::all severity is intended to be used when setting the verbosity of the log to
   *        include all messages, including custom severity levels that may be defined external
   *        to this module.
   */
  const severity_t all  (999, "ALL");

  /**
   * @brief Simple logging class.
   */
  class logger {
    public:
      /**
       * @brief Initializes a new qlog::logger instance with a verbosity level.
       */
      logger(severity_t const& v) :
          _output(&std::cerr), _severity(all.level), _verbosity(v.level) { }

      /**
       * @brief Initializes a new qlog::logger instance with an output stream and verbosity level.
       * @param[in] o Stream to which logging output is sent.
       * @param[in] v Default verbosity level of the log.
       */
      logger(std::ostream& o = std::cerr, severity_t const& v = all)
        : _output(&o), _severity(all.level), _verbosity(v.level) {
      }

      /**
       * @brief Destructor.
       */
      ~logger() {
        // Adds a newline and flushes the stream.
        (*_output) << std::endl;
      }

      /**
       * @brief Generates the timestamp and log level at the beginning of the line.
       * @param[in] l Severity level of the entry.
       *
       * This is used to generate the timestamp and emit the severity level of the message that
       * follows. For example:
       *
       *     auto log = qlog::logger;
       *     log(qlog::debug) << "This is a debug message";
       */
      logger& operator()(severity_t const& l) {
        set_severity(l);
        if(_severity <= _verbosity) {
          (*_output) << std::endl << timestamp() << " [" << l.name << "] ";
        }
        return *this;
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
       * @brief Specialized insertion operator that accepts @c std::ostream manipulators.
       * @param[in] p Pointer to the @c std::ostream manipulator function.
       * @returns a reference to the @c logger object for chaining.
       */
      logger& operator<<(std::ostream& (*p)(std::ostream&)) {
        p(*_output);
        return *this;
      }

      /**
       * @brief Sets the new logging level.
       * @param[in] l The new logging level.
       * @returns a reference to the @c logger object for chaining.
       */
      logger& set_severity(const severity_t l) {
        _severity = l.level;
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

    private:

      /** @brief Stream that receives log messages. */
      std::ostream* _output;

      /** @brief Current message severity level. */
      unsigned long _severity;

      /** @brief Current log verbosity level. */
      unsigned long _verbosity;
  };
}

