#include <gtest/gtest.h>
#include "worldposition.h"

TEST(VectorGeomtryTest, SumVectors) {
    WorldPosition wp1(3.0, 7.0);
    WorldPosition wp2(2.0, 8.0);
    auto result = geom::sum(wp1, wp2);
    EXPECT_EQ(5.0, result.x);
    EXPECT_EQ(15.0, result.y);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
