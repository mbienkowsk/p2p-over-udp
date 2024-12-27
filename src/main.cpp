#include "spdlog/spdlog.h"
#include <boost/asio/io_context.hpp>
#include <boost/asio/post.hpp>

int main() {
  boost::asio::io_context io;

  post(io, []() { spdlog::info("boost::asio::post called"); });
  io.run();

  spdlog::info("Boost.asio is done");
}
