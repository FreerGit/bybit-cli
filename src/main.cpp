
#include <CLI/CLI.hpp>
#include <glaze/glaze.hpp>
#include <iostream>
#include <string>

#include "funding.cpp"

int main(int argc, char** argv) {
    CLI::App app{"Bybit cli"};

    // defaults to funding
    std::string watch{"funding"};
    app.add_option("-w,--watch", watch, "What to watch, defaults to funding");

    CLI11_PARSE(app, argc, argv);

    if (watch == "funding") {
        Funding funding{};
        funding.start();
    }
    return 0;
}
