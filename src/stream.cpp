#include <ixwebsocket/IXWebSocket.h>

#include <iostream>

const std::string sub =
    R"({"op": "subscribe","args": ["allLiquidation.VIRTUALUSDT"]})";

class Stream {
 public:
  Stream() = default;

  auto start() -> void {
    ix::WebSocket webSocket;
    webSocket.setUrl("wss://stream.bybit.com/v5/public/linear");

    webSocket.setOnMessageCallback([&webSocket](
                                       const ix::WebSocketMessagePtr& msg) {
      if (msg->type == ix::WebSocketMessageType::Message) {
        std::cout << "received message: " << msg->str << std::endl;
      } else if (msg->type == ix::WebSocketMessageType::Open) {
        std::cout << "Connection established" << std::endl;
        webSocket.send(sub);
      } else if (msg->type == ix::WebSocketMessageType::Error) {
        std::cout << "Connection error: " << msg->errorInfo.reason << std::endl;
      }
    });

    webSocket.start();

    std::condition_variable cv;
    std::mutex mtx;
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock);  // wait indefinitely
  }
};