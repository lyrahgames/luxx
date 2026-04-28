// Copyright © 2026 Markus Pawellek
//
// This file is part of `luxx`.
//
// `luxx` is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License,
// or (at your option) any later version.
//
// `luxx` is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with `luxx`. If not, see <https://www.gnu.org/licenses/>.
//
#undef NDEBUG
#include <cassert>
import std;
import xstd;
import luxx;

using namespace std::literals;

// assignable_from
// convertible_to
// constructible_from

static_assert(sizeof(luxx::stack_view) == sizeof(void*));
static_assert(alignof(luxx::stack_view) == alignof(void*));
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

  // assert(not stack[0]); // zero is strange
  assert(not stack[1]);
  assert(not stack[2]);
  assert(not stack[3]);
  assert(not stack[4]);

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
  stack.pop(luxx::global{"global_str"});

  print(stack);
  assert(not stack.empty());
  assert(stack.size() == 7);
  // assert(not stack[0]); // zero is strange
  assert(stack[1]);
  assert(stack[2]);
  assert(stack[3]);
  assert(stack[4]);
  assert(stack[1] == luxx::nil);
  assert(stack[2] == -123);
  assert(stack[3] == 3.14);
  assert(stack[4] == 1.0);
  assert(stack[5] == "a string");
  assert(stack[6] == true);
  assert(stack[7] == false);

  stack.push(luxx::global{"global_str"});

  assert(stack.size() == 8);
  assert(stack[8] == "Dynamic String");

  stack.pop();
  auto var = lua["global_str"] = "global string";
  stack.push(var);

  // std::println("lua['global_str'] = {}", var);

  assert(stack.size() == 8);
  assert(stack[8] == "global string");

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
  assert(stack[7] == "global string");
  assert(stack[8] == true);
  assert(stack[9] == 456);
}
