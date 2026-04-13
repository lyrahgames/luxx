#undef NDEBUG
#include <cassert>
import std;
import luxx;

int main() {
  luxx::state lua{};
  lua.open_stdlibs();
  lua.eval("print(\"Hello, World!\\n\")");
  lua.eval("return 54,\"test\" ");
  // assert(n == 54);

  lua.stack_dump();
}
