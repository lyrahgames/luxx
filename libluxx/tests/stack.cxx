#undef NDEBUG
#include <cassert>
import std;
import xstd;
import luxx;

using namespace std::literals;

// assignable_from
// convertible_to
// constructible_from

static_assert(not std::default_initializable<luxx::stack_view>);
static_assert(std::destructible<luxx::stack_view>);
static_assert(not std::movable<luxx::stack_view>);
static_assert(not std::copyable<luxx::stack_view>);
static_assert(std::equality_comparable<luxx::stack_view>);
static_assert(std::ranges::random_access_range<luxx::stack_view>);

static_assert(std::random_access_iterator<luxx::stack_iterator>);

static_assert(not std::default_initializable<luxx::stack_reference>);
static_assert(std::destructible<luxx::stack_reference>);
static_assert(std::move_constructible<luxx::stack_reference>);
static_assert(std::copy_constructible<luxx::stack_reference>);
static_assert(std::movable<luxx::stack_reference>);
static_assert(std::copyable<luxx::stack_reference>);

static_assert(std::ranges::random_access_range<luxx::stack_range>);

static_assert(std::ranges::random_access_range<luxx::stack_array<1>>);
static_assert(std::ranges::random_access_range<luxx::stack_array<2>>);
static_assert(std::ranges::random_access_range<luxx::stack_array<3>>);
static_assert(xstd::tuple_like<luxx::stack_array<1>>);
static_assert(xstd::tuple_like<luxx::stack_array<2>>);
static_assert(xstd::tuple_like<luxx::stack_array<3>>);

void print(luxx::stack_view stack) {
  std::println("╭─────┬───────");
  for (auto value : stack | std::views::reverse)
    std::println("│ {:>3} │ {}", value.stack_index(), value);
  std::println("╞═════╪═══════");
  std::println("│ pos │ value");
  std::println("╰─────┴───────");
}

int main() {
  luxx::state lua{};
  luxx::stack_view stack{lua};
  stack.reserve(10);

  print(stack);
  assert(stack.empty());
  assert(stack.size() == 0);

  stack.push(luxx::nil);
  stack.push(-123, 3.14, 1.0);
  stack.push("a string");
  stack.push(true, false);
  {
    std::string str{"Dynamic String"};
    stack.push(str);
  }

  print(stack);
  assert(not stack.empty());
  assert(stack.size() == 8);
  assert(stack[1] == luxx::nil);
  assert(stack[2] == -123);
  assert(stack[3] == 3.14);
  assert(stack[4] == 1.0);
  assert(stack[5] == "a string");
  assert(stack[6] == true);
  assert(stack[7] == false);
  assert(stack[8] == "Dynamic String");

  assert(stack[2] != -123.);
  assert(stack[4] != 1);
  assert(stack[4] == 1.);
  assert(!(stack[1] < 1) && !(stack[1] == 1) && !(stack[1] > 1));
  assert(stack[2] < -100);
  assert(stack[2] > -125);
  assert(3.0 < stack[3]);
  assert(1.0 >= stack[4]);
  assert(stack[5] != "b string");
  assert(stack[8] != "dynamic string");
  assert("a czstring"sv <= "a string");
  assert("a czstring" <= stack[5]);
  assert(stack[5] <= "a zstring");

  assert(not safe_cast<luxx::boolean>(stack[1]));
  assert(not safe_cast<luxx::integer>(stack[1]));
  assert(not safe_cast<luxx::number>(stack[1]));
  assert(not safe_cast<luxx::string>(stack[1]));

  assert(not safe_cast<luxx::boolean>(stack[2]));
  assert(safe_cast<luxx::integer>(stack[2]).value() == -123);
  assert(not safe_cast<luxx::number>(stack[2]));
  assert(not safe_cast<luxx::string>(stack[2]));

  assert(not safe_cast<luxx::boolean>(stack[3]));
  assert(not safe_cast<luxx::integer>(stack[3]));
  assert(safe_cast<luxx::number>(stack[3]).value() == 3.14);
  assert(safe_cast<float>(stack[3]).value() == 3.14f);
  assert(not safe_cast<luxx::string>(stack[3]));

  assert(not safe_cast<luxx::boolean>(stack[5]));
  assert(not safe_cast<luxx::integer>(stack[5]));
  assert(not safe_cast<luxx::number>(stack[5]));
  assert(safe_cast<luxx::string>(stack[5]).value() == "a string");

  assert(safe_cast<luxx::boolean>(stack[6]).value() == true);
  assert(not safe_cast<luxx::integer>(stack[6]));
  assert(not safe_cast<luxx::number>(stack[6]));
  assert(not safe_cast<luxx::string>(stack[6]));

  stack.rotate(6, -1);
  stack[1] = "another string";
  stack[2] = 456;
  stack[4] = stack[3];
  stack[5] = "123456789";
  stack[6] = lua_cast<luxx::integer>(stack[5]).value();
  stack.push(stack[2]);

  print(stack);
  assert(not stack.empty());
  assert(stack.size() == 9);
  assert(stack[1] == "another string");
  assert(stack[2] == 456);
  assert(stack[3] == 3.14);
  assert(stack[4] == 3.14);
  assert(stack[5] == "123456789");
  assert(stack[6] == 123456789);
  assert(stack[7] == "Dynamic String");
  assert(stack[8] == true);
  assert(stack[9] == 456);
}
