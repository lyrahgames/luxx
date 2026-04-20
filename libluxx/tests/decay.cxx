import std;
import luxx;

using luxx::nil;

static_assert(std::same_as<luxx::nil_t, decltype(luxx::decay(nil))>);

static_assert(std::same_as<luxx::boolean, decltype(luxx::decay(bool{}))>);

static_assert(std::same_as<luxx::number, decltype(luxx::decay(float{}))>);
static_assert(std::same_as<luxx::number, decltype(luxx::decay(double{}))>);

static_assert(
    std::same_as<luxx::integer, decltype(luxx::decay(std::int8_t{}))>);
static_assert(
    std::same_as<luxx::integer, decltype(luxx::decay(std::int16_t{}))>);
static_assert(
    std::same_as<luxx::integer, decltype(luxx::decay(std::int32_t{}))>);
static_assert(
    std::same_as<luxx::integer, decltype(luxx::decay(std::int64_t{}))>);

static_assert(
    std::same_as<luxx::integer, decltype(luxx::decay(std::uint8_t{}))>);
static_assert(
    std::same_as<luxx::integer, decltype(luxx::decay(std::uint16_t{}))>);
static_assert(
    std::same_as<luxx::integer, decltype(luxx::decay(std::uint32_t{}))>);
static_assert(
    std::same_as<luxx::integer, decltype(luxx::decay(std::uint64_t{}))>);

static_assert(std::same_as<luxx::string, decltype(luxx::decay(""))>);
static_assert(
    std::same_as<luxx::string, decltype(luxx::decay(std::string_view{""}))>);
static_assert(std::same_as<luxx::string, decltype(luxx::decay(std::string{}))>);

int main() {}
