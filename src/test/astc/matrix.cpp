#include "test/catch.hpp"

#include "astc/matrix.hpp"
#include "astc/vector.hpp"

TEST_CASE("eigen_vector", "[matrix]")
{
  float eps = 0.06f;

  {
    const mat3x3f_t I(1,0,0, 0,1,0, 0,0,1);
    vec3f_t v(5, 6, 7);
    vec3f_t u = I * v;
    REQUIRE(u.x == Approx(v.x));
    REQUIRE(u.y == Approx(v.y));
    REQUIRE(u.z == Approx(v.z));
  }

  {
    mat3x3f_t m(2,0,1, 0,2,0, 1,0,2);
    vec3f_t eig;
    eigen_vector(m, eig);
    REQUIRE(eig.x == Approx(0.70711f).epsilon(eps));
    REQUIRE(eig.y == Approx(0.00000f).epsilon(eps));
    REQUIRE(eig.z == Approx(0.70711f).epsilon(eps));
  }

  {
    mat3x3f_t m(1,0,0, 0,2,0, 0,0,3);
    vec3f_t eig;
    eigen_vector(m, eig);
    REQUIRE(eig.x == Approx(0).epsilon(eps));
    REQUIRE(eig.y == Approx(0).epsilon(eps));
    REQUIRE(eig.z == Approx(1).epsilon(eps));
  }

  {
    mat3x3f_t m(10,14,5, 6,8,15, 5,5,10);
    vec3f_t eig;
    eigen_vector(m, eig);
    REQUIRE(eig.x == Approx(0.68322f));
    REQUIRE(eig.y == Approx(0.59861f));
    REQUIRE(eig.z == Approx(0.41817f));
  }
}
