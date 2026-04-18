#include <lua.hpp>
#undef NDEBUG
#include <cassert>
import std;
import xstd;
import luxx;

void print(luxx::stack stack) {
  for (auto slot : stack) {
    visit(slot, xstd::match{
                    [&](auto value) {
                      std::println("{:>3}: {:<10}", slot.stack_index(),
                                   slot.type_name());
                    },
                    [&](xstd::one_of<luxx::number, luxx::string> auto value) {
                      std::println("{:>3}: {:<10} = {}", slot.stack_index(),
                                   slot.type_name(), value);
                    },
                    [&](luxx::integer value) {
                      std::println("{:>3}: {:<10} = {}", slot.stack_index(),
                                   "integer", value);
                    },
                });
  }
}

int main() {
  luxx::state lua{};

  // luxx::stack_view stack{lua};
  // stack.stack_dump();

  // stack.push();
  // stack.push(true);
  // stack.push(3.14);
  // stack.push(-123);
  // stack.push("Hello, World!");
  // {
  //   std::string str{"Dynamic String"};
  //   stack.push(str);
  // }

  // stack.reserve(10);
  // for (int i = 0; i < 10; ++i) stack.push(i);

  // stack.rotate(6, -1);

  // luxx::stack_value_view value{stack.native_handle(), -1};
  // assert(not value.is<bool>());
  // assert(not value.is<float>());
  // assert(value.is<std::string_view>());

  // if (auto v = value.as<std::string_view>())
  //   std::println("stack value = string({})", v.value());

  // lua_setglobal(lua, "test");
  // lua_getglobal(lua, "test2");
  // lua_getglobal(lua, "test");

  // stack[5] = stack[1];
  // auto n   = static_cast<int>(stack[4]);

  luxx::stack stack{lua};

  print(stack);

  stack.push(luxx::nil);
  stack.push();
  stack.push(-123, 3.14);
  stack.push("a string");

  print(stack);

  // stack[1] = "another string";
  // stack[2] = 456;
  // stack[5] = "actually a new value...";

  // const auto x   = static_cast<int>(stack[2]);
  // const auto str = static_cast<std::string_view>(stack[1]);
}
