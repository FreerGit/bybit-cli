#include <glaze/glaze.hpp>
#include <string>

struct Symbol {
    std::string symbol;
    std::string fundingRate;
};

struct Instrument {
    std::string symbol;
    int fundingInterval;  // in minutes
};

template <typename T>
struct Market {
    std::string category;
    std::vector<T> list;
};

template <typename T>
struct Response {
    int retCode;
    std::string retMsg;
    Market<T> result;

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