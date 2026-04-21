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
#include <lua.hpp>
import std;
import xstd;
import luxx;

int main() {
  luxx::state state{};
  state.open_stdlibs();

  std::string line;
  while (std::getline(std::cin, line)) {
    auto result = state.eval(line);
    if (result) {
      std::string str{};
      std::println("{}", result.value());  // output range directly
      luxx::stack_view{state}.pop(result.value().size());
    } else {
      std::println("ERROR: {}", result.error().msg);
      luxx::stack_view{state}.pop();
    }
  }
}
