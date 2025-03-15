
#include <glaze/glaze.hpp>
#include <iostream>
#include <string>
#include <vector>

struct Subscribe {
  std::string op = "subscribe";
  std::vector<std::string> args;
};

struct Liqudation {
  std::string s;
  std::string S;
  std::string v;
  std::string p;
};

template <typename T>
struct Message {
  std::string topic;
  std::string type;
  std::vector<T> data;

  void from_json(const std::string& buffer) {
    const glz::error_ctx e =
        glz::read<glz::opts{.error_on_unknown_keys = false}>(*this, buffer);
    if (e.ec != glz::error_code::none) {
      std::cerr << "JSON: " << glz::format_error(e) << std::endl;
      assert(false);
    }
  }

  std::string to_json() {
    std::string json =
        glz::write_json(*this).value_or("Error serializing JSON");
    return glz::prettify_json(json);
  }
};