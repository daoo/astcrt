#include "test/catch.hpp"

#include "astc/bitmanip.hpp"

#include <stddef.h>
#include <stdint.h>

TEST_CASE("getbit", "[bitmanip]")
{
  {
    for (size_t i = 0; i < 8; ++i) {
      REQUIRE_FALSE(getbit(0x00, i));
    }
  }

  {
    for (size_t i = 0; i < 8; ++i) {
      REQUIRE(getbit(0xFF, i));
    }
  }

  REQUIRE(getbit(0x10, 4));
  REQUIRE(getbit(0x01, 0));
  REQUIRE_FALSE(getbit(0x01, 2));
}

TEST_CASE("getbits", "[bitmanip]")
{
  uint8_t number = 0xBA;
  REQUIRE(getbits(number, 3, 0) == 0xA);
  REQUIRE(getbits(number, 7, 4) == 0xB);
}

TEST_CASE("orbits8_ptr", "[bitmanip]")
{
  {
    uint32_t number = 0;
    uint8_t* data = reinterpret_cast<uint8_t*>(&number);

    orbits8_ptr(data, 0, 1, 1);
    REQUIRE(number == 0x00000001);
    orbits8_ptr(data, 1, 1, 1);
    REQUIRE(number == 0x00000003);
    orbits8_ptr(data, 2, 0, 2);
    REQUIRE(number == 0x00000003);
    orbits8_ptr(data, 4, 0xF, 4);
    REQUIRE(number == 0x000000F3);
  }

  {
    uint32_t number = 0;
    uint8_t* data = reinterpret_cast<uint8_t*>(&number);
    orbits8_ptr(data, 0, 0xF, 4);
    REQUIRE(number == 0x0000000F);
    orbits8_ptr(data, 8, 0xAA, 8);
    REQUIRE(number == 0x0000AA0F);
    orbits8_ptr(data, 16, 0xBB, 8);
    REQUIRE(number == 0x00BBAA0F);
  }
}

TEST_CASE("orbits16_ptr", "[bitmanip]")
{
  {
    uint32_t number = 0;
    uint8_t* data = reinterpret_cast<uint8_t*>(&number);

    orbits16_ptr(data, 0, 0xFFFF, 16);
    REQUIRE(number == 0x0000FFFF);
    orbits16_ptr(data, 16, 0xFFFF, 16);
    REQUIRE(number == 0xFFFFFFFF);
  }

  {
    uint32_t number = 0;
    uint8_t* data = reinterpret_cast<uint8_t*>(&number);

    orbits16_ptr(data, 0, 0x0001, 16);
    REQUIRE(number == 0x00000001);
    orbits16_ptr(data, 1, 0x0001, 16);
    REQUIRE(number == 0x00000003);
    orbits16_ptr(data, 2, 0x0001, 16);
    REQUIRE(number == 0x00000007);
    orbits16_ptr(data, 3, 0x0001, 16);
    REQUIRE(number == 0x0000000F);
    orbits16_ptr(data, 4, 0x000F, 16);
    REQUIRE(number == 0x000000FF);
    orbits16_ptr(data, 16, 0x00FF, 16);
    REQUIRE(number == 0x00FF00FF);
  }
}

TEST_CASE("getbytes2", "[bitmanip]")
{
  uint32_t number = 0x12ABCDEF;
  uint8_t* data = reinterpret_cast<uint8_t*>(&number);
  REQUIRE(getbytes2(data, 0) == 0xCDEF);
  REQUIRE(getbytes2(data, 1) == 0xABCD);
  REQUIRE(getbytes2(data, 2) == 0x12AB);
}

TEST_CASE("setbytes2", "[bitmanip]")
{
  uint32_t number = 0;
  uint8_t* data = reinterpret_cast<uint8_t*>(&number);

  uint16_t a = 0xAABB;

  setbytes2(data, 0, a);

  REQUIRE(data[0] == 0xBB);
  REQUIRE(data[1] == 0xAA);
  REQUIRE(data[2] == 0x00);
  REQUIRE(data[3] == 0x00);

  setbytes2(data, 1, a);

  REQUIRE(data[0] == 0xBB);
  REQUIRE(data[1] == 0xBB);
  REQUIRE(data[2] == 0xAA);
  REQUIRE(data[3] == 0x00);
}

TEST_CASE("split_high_low", "[bitmanip]")
{
  {
    uint8_t number = 0xAB;
    uint8_t high, low;
    split_high_low(number, 4, high, low);
    REQUIRE(high == 0xA);
    REQUIRE(low == 0xB);
  }
  {
    uint8_t number = 0xFF;
    uint8_t high, low;
    split_high_low(number, 4, high, low);
    REQUIRE(high == 0xF);
    REQUIRE(low == 0xF);
  }
  {
    uint8_t number = 0x00;
    uint8_t high, low;
    split_high_low(number, 4, high, low);
    REQUIRE(high == 0x0);
    REQUIRE(low == 0x0);
  }
}

TEST_CASE("bitwriter", "[bitmanip]")
{
  uint32_t number = 0;
  bitwriter w(reinterpret_cast<uint8_t*>(&number));

  w.write8(0x0F, 4);
  REQUIRE(number == 0x0000000F);
  w.write8(0x0F, 4);
  REQUIRE(number == 0x000000FF);
  w.write8(0x02, 2);
  REQUIRE(number == 0x000002FF);
  w.write8(0x02, 2);
  REQUIRE(number == 0x00000AFF);
  w.write8(0x00, 4);
  REQUIRE(number == 0x00000AFF);
  w.write8(0xFF, 8);
  REQUIRE(number == 0x00FF0AFF);
}

TEST_CASE("reverse_byte", "[bitmanip]")
{
  for (size_t i = 0; i < 256; ++i) {
    REQUIRE(reverse_byte(reverse_byte(static_cast<uint8_t>(i))) == i);
  }
}

TEST_CASE("reverse_bytes", "[bitmanip]")
{
  {
    uint32_t a = 0xF0F0F0F0;
    uint32_t b = 0;
    reverse_bytes(
        reinterpret_cast<uint8_t*>(&a),
        4,
        reinterpret_cast<uint8_t*>(&b)+3);

    REQUIRE(b == 0x0F0F0F0F);
  }

  {
    uint32_t a = 0x0000000F;
    uint32_t b = 0;
    reverse_bytes(
        reinterpret_cast<uint8_t*>(&a),
        1,
        reinterpret_cast<uint8_t*>(&b)+3);

    REQUIRE(b == 0xF0000000);
  }

  {
    uint32_t a = 0x000ED76D;
    uint32_t b = 0;
    reverse_bytes(
        reinterpret_cast<uint8_t*>(&a),
        3,
        reinterpret_cast<uint8_t*>(&b)+3);

    REQUIRE(b == 0xB6EB7000);
  }

  {
    // Check that existing bytes are not overwritten.
    uint32_t a = 0x000FFFFF;
    uint32_t b = 0x00000FFF;
    reverse_bytes(
        reinterpret_cast<uint8_t*>(&a),
        3,
        reinterpret_cast<uint8_t*>(&b)+3);
    REQUIRE(b == 0xFFFFFFFF);
  }
}
