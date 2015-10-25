#include <gtest/gtest.h>
#include "worldposition.h"

TEST(VectorGeomtryTest, SumVectors) {
    WorldPosition wp1(3.0, 7.0);
    WorldPosition wp2(2.0, 8.0);
    auto result = geom::sum(wp1, wp2);
    EXPECT_EQ(5.0, result.x);
    EXPECT_EQ(15.0, result.y);
}

TEST(VectorGeomtryTest, SubstructVectors) {
    WorldPosition wp1(3.0, 7.0);
    WorldPosition wp2(2.0, 8.0);
    auto result = geom::substruct(wp1, wp2);
    EXPECT_EQ(1.0, result.x);
    EXPECT_EQ(-1.0, result.y);
}

TEST(VectorGeomtryTest, ScaleVector) {
    WorldPosition wp(3.0, 7.0);
    auto result = geom::scale(wp, 2);
    EXPECT_EQ(6.0, result.x);
    EXPECT_EQ(14.0, result.y);
}

TEST(VectorGeomtryTest, GetAbsVector) {
    Point p(4, 7);
    EXPECT_EQ(8, p.abs());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
