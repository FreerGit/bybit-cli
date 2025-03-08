#include <glaze/glaze.hpp>
#include <iostream>
#include <string>

// Define a struct for Bybit order
struct BybitOrder {
    std::string symbol;
    double price;
    double qty;
    std::string side;
};

// Register the struct with glaze
template <>
struct glz::meta<BybitOrder> {
    using T = BybitOrder;
    static constexpr auto value =
        glz::object("symbol", &T::symbol, "price", &T::price, "qty", &T::qty,
                    "side", &T::side);
};

int main() {
    // Create an order
    BybitOrder order = {"BTCUSDT", 45000.5, 0.1, "Buy"};

    // Serialize to JSON
    std::string buffer = glz::write_json(order).value_or("error");
    std::cout << buffer << '\n';
    // Deserialize from JSON
    // BybitOrder new_order;
    std::string json_str =
        R"({"symbol":"btc","price":3.14,"qty":4.44,"side":"sell"})";

    auto s = glz::read_json<BybitOrder>(json_str);

    if (s) {
        const auto new_order = s.value();
        std::cout << "Parsed Struct: " << new_order.symbol << ", "
                  << new_order.price << std::endl;
    }

    return 0;
}
