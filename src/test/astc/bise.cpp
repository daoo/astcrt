#include "test/catch.hpp"

#include "astc/integer_sequence_encoding.hpp"
#include "astc/range.hpp"

#include <stdint.h>

TEST_CASE("integer_sequence_encode", "[bise]")
{
  uint8_t numbers[5] = { 1, 5, 4, 8, 9 };
  uint8_t output[5] = { 0 };
  integer_sequence_encode(numbers, 5, RANGE_10, output);
  REQUIRE(static_cast<int>(output[0]) == 81);
  REQUIRE(static_cast<int>(output[1]) == 114);
  REQUIRE(static_cast<int>(output[2]) == 0);
  REQUIRE(static_cast<int>(output[3]) == 0);
  REQUIRE(static_cast<int>(output[4]) == 0);
}

TEST_CASE("compute_ise_bitcount", "[bise]")
{
  REQUIRE(compute_ise_bitcount(1, RANGE_2) == 1);
  REQUIRE(compute_ise_bitcount(2, RANGE_2) == 2);
  REQUIRE(compute_ise_bitcount(3, RANGE_2) == 3);
  REQUIRE(compute_ise_bitcount(4, RANGE_32) == 20);
  REQUIRE(compute_ise_bitcount(4, RANGE_10) == 14);
  REQUIRE(compute_ise_bitcount(4, RANGE_12) == 15);
  REQUIRE(compute_ise_bitcount(4, RANGE_16) == 16);
  REQUIRE(compute_ise_bitcount(4, RANGE_20) == 18);
  REQUIRE(compute_ise_bitcount(4, RANGE_24) == 19);
}
