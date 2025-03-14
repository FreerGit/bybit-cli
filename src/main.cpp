
#include <CLI/CLI.hpp>
#include <glaze/glaze.hpp>
#include <string>

#include "funding.cpp"
#include "stream.cpp"

auto main(int argc, char** argv) -> int {
  CLI::App app{"Bybit cli"};

  // Defaults to watching funding
  std::string watch{"funding"};
  app.add_option("-w,--watch", watch, "What to watch, defaults to funding");

  std::string sort_order{"asc"};
  app.add_option("--sort", sort_order, "Sort order (asc or desc)")
      ->check(CLI::IsMember({"asc", "desc"}));

  CLI11_PARSE(app, argc, argv);

  if (watch == "funding") {
    bool ascending = sort_order == "asc";
    Funding funding{};
    funding.start(ascending);
  }

  if (watch == "liqs") {
    Stream stream{};
    stream.start();
  }

  return 0;
}
