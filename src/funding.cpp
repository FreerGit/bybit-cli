#include <cpr/cpr.h>
#include <openssl/core_dispatch.h>

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <ios>
#include <iostream>
#include <ranges>
#include <string>
#include <thread>
#include <vector>

#include "market.cpp"

constexpr std::string BASE_URL{"api.bybit.com"};

struct FundingInfo {
  std::string symbol;
  double funding_apr;
};

auto calculateFundingApr(const double rate, const int interval) -> double {
  assert(interval > 0);
  constexpr double minutes_per_year = 365 * 24 * 60;
  return rate * (minutes_per_year / interval) * 100;
};

class Funding {
 public:
  Funding() = default;

  auto start(const bool ascending) -> void {
    while (true) {
      this->fetchInfo();
      this->combineIntoFundingInfo();
      this->outputFundingInfo(ascending);
      perps_.clear();
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  }

 private:
  std::vector<FundingInfo> perps_{};
  Response<Symbol> symbols_{};
  Response<Instrument> instruments_{};

  auto fetchInfo() -> void {
    cpr::Response r =
        cpr::Get(cpr::Url(BASE_URL + "/v5/market/tickers"),
                 cpr::Parameters{{"category", "linear"}, {"limit", "1000"}});

    cpr::Response instr =
        cpr::Get(cpr::Url(BASE_URL + "/v5/market/instruments-info"),
                 cpr::Parameters{{"category", "linear"}, {"limit", "1000"}});

    symbols_.from_json(r.text);
    instruments_.from_json(instr.text);

    assert(symbols_.result.list.size() == instruments_.result.list.size());
  }

  auto combineIntoFundingInfo() -> void {
    for (size_t i = 0; i < symbols_.result.list.size(); i++) {
      const Symbol symbol = symbols_.result.list[i];
      const Instrument instrument = instruments_.result.list[i];
      assert(symbol.symbol == instrument.symbol);

      if (symbol.fundingRate.size() != 0) {
        const double apr = calculateFundingApr(std::stod(symbol.fundingRate),
                                               instrument.fundingInterval);
        FundingInfo info{symbol.symbol, apr};
        this->perps_.emplace_back(info);
      }
    }
  }

  auto outputFundingInfo(const bool ascending) -> void {
    std::sort(this->perps_.begin(), this->perps_.end(),
              [ascending](const FundingInfo& a, const FundingInfo& b) {
                return ascending ? (a.funding_apr < b.funding_apr)
                                 : (a.funding_apr > b.funding_apr);
              });

    std::cout << "\033[H\033[J";  // clear the terminal

    auto take_n = this->perps_ | std::views::take(15);
    for (const FundingInfo& info : take_n) {
      std::cout << std::left << std::setw(16) << info.symbol << std::setw(12)
                << info.funding_apr << std::endl;
    }
  }
};
