#undef NDEBUG
#include <cassert>
import std;
import luxx;

int main() {
  luxx::state lua{};
  lua.open_stdlibs();
  {
    auto result = lua.eval("print(\"Hello, World!\\n\")");
    if (not result)
      std::println("{}", result.error().msg);
    else
      assert(result.value().empty());
  }

  {
    auto result = lua.eval("return 54,\"test\" ");
    if (not result)
      std::println("{}", result.error().msg);
    else {
      auto values = result.value();
      assert(values.size() == 2);
      assert(static_cast<luxx::integer>(values[0]) == 54);
      assert(values[0] == 54);
      assert(values[0] != "test");
      assert(values[0] != "54");
      assert(static_cast<luxx::string>(values[1]) == "test");
      assert(values[1] == "test");
      assert(values[1] != 54);
      // std::println("value[1] = {}", static_cast<const char*>(values[1]));
    }
  }
}
