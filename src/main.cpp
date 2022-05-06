#include <fstream>
#include <functional>
#include <iostream>

#include <docopt/docopt.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

// This file will be generated automatically when you run the CMake configuration step.
// It creates a namespace called `cloxpp`.
// You can modify the source template at `configured_files/config.hpp.in`.
#include <internal_use_only/config.hpp>

static constexpr auto USAGE =
  R"(Lox C++ Interpreter.

    Usage:
          cloxpp-interpreter [<script>]
          cloxpp-interpreter (-h | --help)
          cloxpp-interpreter --version
 Options:
          -h --help     Show this screen.
          --version     Show version.
)";

namespace cloxpp {

void run(std::string_view script);

void runPrompt()
{
  spdlog::info("Running interpreter interactively");
  while (true) {
    std::string line;
    fmt::print(">>> ");
    if (!std::getline(std::cin, line)) { break; }
    run(line);
  }
  fmt::print("/>>>");
}
void runFile(std::string_view file)
{
  spdlog::info("Interpreting {}", file);
  std::ifstream file_stream((std::string(file)));
  if (file_stream.is_open()) {
    run(std::string((std::istreambuf_iterator<char>(file_stream)), std::istreambuf_iterator<char>()));
  } else {
    spdlog::error("Could not open file");
    std::terminate();
  }
}
void run(std::string_view script) { std::cout << script << std::endl; }

}// namespace cloxpp

int main(int argc, const char **argv)
{
  static constexpr auto log_file = "cloxpp.log";
  auto file_logger = spdlog::basic_logger_mt("file_logger", log_file, true);// do not truncate!
  spdlog::set_default_logger(file_logger);

  try {
    std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
      { std::next(argv), std::next(argv, argc) },
      true,// show help if requested
      fmt::format("{} {}",
        cloxpp::cmake::project_name,
        cloxpp::cmake::project_version));// version string, acquired from config.hpp via CMake

    fmt::print("Log file at: {}\n", log_file);

    spdlog::info("Running with args: {}", [&args](std::stringstream sstream) {
      for (auto const &arg : args) { sstream << arg.first << "=" << arg.second << '\n'; }
      return sstream.str();
    }({}));

    const auto script = args["<script>"];
    if (script.isString()) {
      cloxpp::runFile(script.asString());
    } else {
      cloxpp::runPrompt();
    }

    spdlog::info("Parsed Arguments");

  } catch (const std::exception &e) {
    fmt::print("Unhandled exception in main: {}", e.what());
  }
}
