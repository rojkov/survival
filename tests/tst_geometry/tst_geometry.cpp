#include <gtest/gtest.h>
#include <SDL.h>
#include "geometry.h"

TEST(GeometryTest, CanCalculateSomething) {
    Rect rect {1, 1, 1,1};
    rect.as_sdl_rect();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
