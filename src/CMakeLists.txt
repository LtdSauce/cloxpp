find_package(fmt CONFIG)
find_package(spdlog CONFIG)
find_package(docopt CONFIG)

# Generic test that uses conan libs
add_executable(cloxpp-interpreter main.cpp)
target_link_libraries(
  cloxpp-interpreter
  PRIVATE project_options
          project_warnings
          docopt::docopt
          fmt::fmt
          spdlog::spdlog)

target_include_directories(cloxpp-interpreter PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")

