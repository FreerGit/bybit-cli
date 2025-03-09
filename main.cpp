#include <cpr/cpr.h>

#include <CLI/CLI.hpp>
#include <glaze/glaze.hpp>
#include <iostream>
#include <string>

#include "glaze/json/json_t.hpp"

struct Symbol {
    std::string symbol;
};

struct Symbols {
    std::string category;
    std::vector<Symbol> list;
};

template <typename T>
struct Response {
    int retCode;
    std::string retMsg;
    T result;

    glz::error_ctx from_json(const std::string& buffer) {
        return glz::read<glz::opts{.error_on_unknown_keys = false}>(*this,
                                                                    buffer);
    }

    std::string to_json() {
        std::string json =
            glz::write_json(*this).value_or("Error serializing JSON");
        return glz::prettify_json(json);
    }
};

constexpr std::string BASE_URL{"api.bybit.com"};

int main(int argc, char** argv) {
    CLI::App app{"Bybit cli"};

    // defaults to funding
    std::string watch{"funding"};
    app.add_option("-w,--watch", watch, "What to watch, defaults to funding");

    CLI11_PARSE(app, argc, argv);

    assert(watch == "funding");

    cpr::Response r = cpr::Get(
        cpr::Url(BASE_URL + "/v5/market/tickers"),
        cpr::Parameters{{"category", "linear"}, {"symbol", "BTCUSDT"}});

    std::cout << r.status_code << r.text << std::endl;

    Response<Symbols> symbols{};
    symbols.from_json(r.text);

    std::cout << symbols.to_json() << std::endl;
    return 0;
}
