#include "test/catch.h"

#include "astc/endpoints_principal_components.h"
#include "astc/endpoints_min_max.h"
#include "astc/vector.h"

TEST_CASE("principal_component_analysis", "[pca]") {
  {
    unorm8_t colors[16] = {
        {{7, 18, 22, 255}},  {{7, 18, 22, 255}},  {{10, 16, 21, 255}},
        {{10, 16, 21, 255}}, {{7, 18, 22, 255}},  {{7, 18, 22, 255}},
        {{10, 16, 21, 255}}, {{10, 16, 21, 255}}, {{7, 18, 22, 255}},
        {{7, 18, 22, 255}},  {{10, 16, 21, 255}}, {{10, 16, 21, 255}},
        {{7, 18, 22, 255}},  {{7, 18, 22, 255}},  {{10, 16, 21, 255}},
        {{10, 16, 21, 255}}};

    vec3f_t k, m;
    principal_component_analysis_block(colors, k, m);

    REQUIRE(m.x == Approx(21.5));
    REQUIRE(m.y == Approx(17.0));
    REQUIRE(m.z == Approx(8.5));

    REQUIRE(k.x == Approx(0.26726));
    REQUIRE(k.y == Approx(0.53452));
    REQUIRE(k.z == Approx(-0.80178));

    vec3f_t e0, e1;
    find_min_max_block(colors, k, m, e0, e1);

    REQUIRE(e0.x == Approx(21));
    REQUIRE(e0.y == Approx(16));
    REQUIRE(e0.z == Approx(10));

    REQUIRE(e1.x == Approx(22));
    REQUIRE(e1.y == Approx(18));
    REQUIRE(e1.z == Approx(7));
  }

  {
    unorm8_t colors[16] = {
        {{253, 217, 0, 255}}, {{254, 218, 0, 255}}, {{253, 219, 0, 255}},
        {{254, 221, 0, 255}}, {{253, 219, 0, 255}}, {{252, 216, 0, 255}},
        {{253, 221, 0, 255}}, {{255, 222, 0, 255}}, {{254, 219, 0, 255}},
        {{254, 219, 0, 255}}, {{254, 225, 0, 255}}, {{254, 223, 0, 255}},
        {{245, 200, 0, 255}}, {{248, 210, 0, 255}}, {{251, 214, 0, 255}},
        {{247, 210, 0, 255}}};

    vec3f_t k, m;
    principal_component_analysis_block(colors, k, m);

    REQUIRE(m.x == Approx(0.0));
    REQUIRE(m.y == Approx(217.0625));
    REQUIRE(m.z == Approx(252.125));

    REQUIRE(k.x == Approx(0.0));
    REQUIRE(k.y == Approx(0.91038041));
    REQUIRE(k.z == Approx(0.4137723));

    vec3f_t e0, e1;
    find_min_max_block(colors, k, m, e0, e1);

    REQUIRE(e0.x == Approx(0.0));
    REQUIRE(e0.y == Approx(200.23731059));
    REQUIRE(e0.z == Approx(244.47787005));

    REQUIRE(e1.x == Approx(0.0));
    REQUIRE(e1.y == Approx(224.34733447));
    REQUIRE(e1.z == Approx(255.0f));
  }

  {
    unorm8_t colors[16] = {
        {{206, 216, 230, 255}}, {{206, 216, 230, 255}}, {{205, 217, 229, 255}},
        {{205, 217, 229, 255}}, {{206, 216, 230, 255}}, {{206, 216, 230, 255}},
        {{205, 217, 229, 255}}, {{205, 217, 229, 255}}, {{206, 216, 230, 255}},
        {{206, 216, 230, 255}}, {{205, 217, 229, 255}}, {{205, 217, 229, 255}},
        {{206, 216, 230, 255}}, {{206, 216, 230, 255}}, {{205, 217, 229, 255}},
        {{205, 217, 229, 255}}};

    vec3f_t k, m;
    principal_component_analysis_block(colors, k, m);

    REQUIRE(m.x == Approx(229.5));
    REQUIRE(m.y == Approx(216.5));
    REQUIRE(m.z == Approx(205.5));

    REQUIRE(k.x == Approx(0.57735027));
    REQUIRE(k.y == Approx(-0.57735027));
    REQUIRE(k.z == Approx(0.57735027));

    vec3f_t e0, e1;
    find_min_max_block(colors, k, m, e0, e1);

    REQUIRE(e0.x == Approx(229.0));
    REQUIRE(e0.y == Approx(217.0));
    REQUIRE(e0.z == Approx(205.0));

    REQUIRE(e1.x == Approx(230.0));
    REQUIRE(e1.y == Approx(216.0));
    REQUIRE(e1.z == Approx(206.0));
  }
}
