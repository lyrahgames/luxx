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
import std;
import luxx;

void test(luxx::state_view lua) {
  std::println("ptr = {}", static_cast<void*>(lua.raw_state()));
}

template <typename type>
void breakpoint() = delete;

// static_assert(luxx::lua_allocator_like<luxx::default_allocator>);
// static_assert(luxx::allocator_like<luxx::default_allocator>);

// static_assert(luxx::stateless_lua_like_allocator<luxx::default_allocator>);

const auto custom_alloc =
    [](void* ptr, std::size_t osize, std::size_t nsize) -> void* {
  if (nsize == 0) {
    std::free(ptr);
    return nullptr;
  } else
    return std::realloc(ptr, nsize);
};

static_assert(
    luxx::allocator_interface<decltype(luxx::default_allocator::operator())>);
static_assert(not luxx::allocator_interface<decltype(custom_alloc)>);
static_assert(std::default_initializable<decltype(custom_alloc)>);

struct custom_allocator {
  std::size_t realloc = 0;
  std::size_t dealloc = 0;
  auto operator()(void* ptr, std::size_t osize, std::size_t nsize) noexcept
      -> void* {
    if (nsize == 0) {
      std::free(ptr);
      ++dealloc;
      return nullptr;
    }
    ++realloc;
    return std::realloc(ptr, nsize);
  }
};

// static_assert(
//     luxx::allocator_interface<decltype(custom_allocator::operator())>);

// static_assert(luxx::allocator_like<std::allocator<std::byte>>);
// static_assert(luxx::allocator_like<std::allocator<int>>);

static_assert(not luxx::stateless_allocator<std::allocator<std::byte>>);
static_assert(luxx::stateless_allocator<
              luxx::as_allocator_interface<std::allocator<std::byte>>>);

int main() {
  luxx::state lua{};
  lua.open_stdlibs();

  luxx::state lua2{std::allocator<int>{}};

  luxx::state log_lua{
      [](void* ptr, std::size_t osize, std::size_t nsize) noexcept -> void* {
        if (nsize == 0) {
          std::free(ptr);
          std::println("dealloc {} B", osize);
          return nullptr;
        }
        std::println("realloc {} B -> {} B", osize, nsize);
        return std::realloc(ptr, nsize);
      }};

  static_assert(not luxx::stateful_allocator<custom_allocator>);
  static_assert(luxx::stateful_allocator<std::shared_ptr<custom_allocator>>);
  static_assert(luxx::stateful_allocator<std::unique_ptr<custom_allocator>>);
  static_assert(
      luxx::stateful_allocator<luxx::allocator_reference<custom_allocator>>);
  {
    auto allocator = std::make_shared<custom_allocator>();
    {
      luxx::state stat_lua{allocator};
    }
    std::println("dealloc = {}", allocator->dealloc);
    std::println("realloc = {}", allocator->realloc);
  }
  {
    custom_allocator alloc{};
    {
      luxx::state lua{luxx::allocator_reference{alloc}};
    }
    std::println("dealloc = {}", alloc.dealloc);
    std::println("realloc = {}", alloc.realloc);
  }
  // breakpoint<decltype(lua)>();
  // breakpoint<decltype(luxx::default_allocator::operator())>();
  // breakpoint<decltype(lua2)>();
  // breakpoint<typename luxx::std_allocator<std::allocator<int>>::allocator>();
  // test(lua);
  // breakpoint<luxx::number>();
  // breakpoint<luxx::integer>();
  // test(luxx::state{});
  // test(std::move(lua));
}
