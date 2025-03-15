#include <cpr/cpr.h>
#include <ixwebsocket/IXWebSocket.h>

#include <iostream>
#include <string>

#include "market.cpp"
#include "stream.cpp"

class Stream {
 public:
  Stream() = default;

  auto start() -> void {
    const Subscribe sub = buildSubscribe();

    ix::WebSocket webSocket;
    webSocket.setUrl("wss://stream.bybit.com/v5/public/linear");

    webSocket.setOnMessageCallback([&webSocket,
                                    sub](const ix::WebSocketMessagePtr& msg) {
      if (msg->type == ix::WebSocketMessageType::Message) {
        Message<Liqudation> m{};
        m.from_json(msg->str);

        if (m.topic.size() > 0) {
          for (const Liqudation& liq : m.data) {
            const double size = std::stod(liq.v) * std::stod(liq.p);
            if (size > 1000) {
              std::cout << std::left << std::setw(16) << m.topic.substr(15)
                        << std::setw(5) << liq.S
                        << std::stod(liq.v) * std::stod(liq.p) << std::endl;
            }
          }
        }
      } else if (msg->type == ix::WebSocketMessageType::Open) {
        std::cout << "Connection established" << std::endl;
        std::string json =
            glz::write_json(sub).value_or("Error serializing JSON");
        webSocket.send(json);
      } else if (msg->type == ix::WebSocketMessageType::Error) {
        std::cout << "Connection error: " << msg->errorInfo.reason << std::endl;
      }
    });

    webSocket.start();

    while (true) {
      webSocket.send(R"({"op": "ping"})");
      std::this_thread::sleep_for(std::chrono::seconds(10));
    }
  }

 private:
  Response<Symbol> symbols_{};

  auto buildSubscribe() -> Subscribe {
    cpr::Response r =
        cpr::Get(cpr::Url(BASE_URL + "/v5/market/tickers"),
                 cpr::Parameters{{"category", "linear"}, {"limit", "1000"}});

    symbols_.from_json(r.text);

    Subscribe sub{};

    for (const auto& s : symbols_.result.list) {
      sub.args.push_back("allLiquidation." + s.symbol);
    }
    return sub;
  }
};