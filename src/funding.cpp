#include <cpr/cpr.h>

#include <iostream>
#include <string>

#include "market.cpp"

constexpr std::string BASE_URL{"api.bybit.com"};

class Funding {
   public:
    Funding() {}
    void start() const {
        cpr::Response r = cpr::Get(
            cpr::Url(BASE_URL + "/v5/market/tickers"),
            cpr::Parameters{{"category", "linear"}, {"limit", "1000"}});

        cpr::Response instr = cpr::Get(
            cpr::Url(BASE_URL + "/v5/market/instruments-info"),
            cpr::Parameters{{"category", "linear"}, {"limit", "1000"}});

        // std::cout << r.status_code << r.text << std::endl;
        // std::cout << instr.status_code << r.text << std::endl;

        Response<Symbol> symbols{};
        Response<Instrument> instruments{};
        symbols.from_json(r.text);
        instruments.from_json(instr.text);

        // for (const auto& s : symbols.result.list) {
        //     std::cout << s.symbol << " " << s.fundingRate << std::endl;
        // }
        std::cout << "Length of symbols " << symbols.result.list.size()
                  << std::endl;

        std::cout << "Length of instruments " << instruments.result.list.size()
                  << std::endl;
    }
};
