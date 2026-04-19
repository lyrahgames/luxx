#undef NDEBUG
#include <cassert>
import std;
import xstd;
import luxx;

static_assert(std::random_access_iterator<luxx::stack_iterator>);
static_assert(std::ranges::random_access_range<luxx::stack_range>);
static_assert(std::ranges::random_access_range<luxx::stack>);

void print(luxx::stack stack) {
  std::println("╭─────┬───────");
  for (auto value : stack | std::views::reverse)
    std::println("│ {:>3} │ {}", value.stack_index(), value);
  std::println("╞═════╪═══════");
  std::println("│ pos │ value");
  std::println("╰─────┴───────");
}

int main() {
  luxx::state lua{};
  luxx::stack stack{lua};
  stack.reserve(10);

  print(stack);
  assert(stack.empty());
  assert(stack.size() == 0);

  stack.push(luxx::nil);
  stack.push();
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

  stack.rotate(6, -1);
  stack[1] = "another string";
  stack[2] = 456;
  stack[4] = stack[3];
  stack[5] = "123456789";
  stack[6] = static_cast<luxx::integer>(stack[5]);
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
