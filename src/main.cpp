#include <functional>
#include <iostream>

#include <docopt/docopt.h>
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

// ToDo implement as in 4.1
void runPrompt() { spdlog::info("Running interpreter interactively"); }
void runFile(std::string_view file) { spdlog::info("Interpreting {}", file); };

}// namespace cloxpp

int main(int argc, const char **argv)
{
  try {
    std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
      { std::next(argv), std::next(argv, argc) },
      true,// show help if requested
      fmt::format("{} {}",
        cloxpp::cmake::project_name,
        cloxpp::cmake::project_version));// version string, acquired from config.hpp via CMake
    for (auto const &arg : args) { std::cout << arg.first << "=" << arg.second << '\n'; }
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
