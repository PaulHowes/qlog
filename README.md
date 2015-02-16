# QLOG

Quick Log for C++`

# Building

Quick Log is distributed only as a header. There is nothing to compile in this repository other
than tests which also illustrate how to use QLOG.

## Dependencies

* [CMake](http://www.cmake.org) 2.8.12 or newer
* C compiler. The following have been tested:
  * [llvm](http://llvm.org) 3.4 or newer
  * [gcc](http://gcc.gnu.org) 4.8.3 or newer
  * [Visual Studio](http://www.visualstudio.com)

## Instructions

I highly recommend building outside of the source tree so that build products do not pollute the
repository. The simplest way to accomplish this is to create a sub-directory named "build" as
follows:

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make

# Examples

## Send All Log Output to Standard Error

    qlog::logger my_log;

## Open or Append to a Log File

    std::ofstream log_file("error.log", std::ios::app);
    qlog::logger my_log(log_file);

## Open or Append Only Errors and Warnings to a Log File

    std::ofstream log_file("error.log", std::ios::app);
    qlog::logger my_log(log_file, qlog::warning);

## Emit an Error Message.

    my_log(qlog::error) << "This is an error message";

## Emit a Debug Message.

    my_log(qlog::debug) << "This is a debug message.";
    // Note that if the verbosity level is qlog::warning, as shown above, this
    // message will not be sent to the log.

# To Do

1.  Move all configuration to a separate class.
2.  Add log aging by size (e.g. 1 MB) or time span (e.g. daily, weekly, or monthly)
3.  When aging the log, maintain a maximum number of "old" log files.
4.  Add a dependency on the [ASF](http://github.com/PaulHowes/asf) library for formatting.

# Contributing

1.  Fork it
2.  Create a feature branch (`git checkout -b new-feature`)
3.  Commit changes (`git commit -am "Added new feature xyz"`)
4.  Push the branch (`git push origin new-feature`)
5.  Create a new pull request.

# Maintainers

* Paul Howes (http://github.com/PaulHowes/)

# License

Quick Log copyright 2014 Paul Howes and is licensed under the [Apache License](LICENSE).
