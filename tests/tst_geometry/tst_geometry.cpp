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

TEST(BaseRectTest, contains) {
    BasePoint<int32_t> point1(10, 10);
    BasePoint<int32_t> point2(100, 100);
    BaseRect<int32_t, BasePoint<int32_t>> rect(1, 1, 20, 20);

    EXPECT_TRUE(rect.contains(point1));
    EXPECT_FALSE(rect.contains(point2));
}

TEST(BaseRectTest, is_inside) {
    BaseRect<int32_t, BasePoint<int32_t>> rect1(1, 1, 10, 10);
    BaseRect<int32_t, BasePoint<int32_t>> rect2(1, 1, 5, 100);
    BaseRect<int32_t, BasePoint<int32_t>> rect3(3, 4, 2, 3);

    EXPECT_FALSE(rect2.is_inside(rect1));
    EXPECT_TRUE(rect3.is_inside(rect1));
}

TEST(BaseRectTest, relative_intersection) {
    BaseRect<int32_t, BasePoint<int32_t>> rect1(5, 6, 18, 28);
    BaseRect<int32_t, BasePoint<int32_t>> rect2(3, 7, 11, 28);
    auto newRect = geom::rect::relative_intersection(rect1, rect2);

    EXPECT_EQ(0, newRect.x);
    EXPECT_EQ(1, newRect.y);
    EXPECT_EQ(9, newRect.width);
    EXPECT_EQ(28, newRect.height);
}

TEST(BaseRectTest, move) {
    BaseRect<int32_t, BasePoint<int32_t>> rect(1, 2, 10, 10);
    BasePoint<int32_t> delta(5, 7);
    auto newRect = geom::rect::move_by(rect, delta);

    EXPECT_EQ(6, newRect.x);
    EXPECT_EQ(9, newRect.y);
    EXPECT_EQ(10, newRect.width);
    EXPECT_EQ(10, newRect.height);
}

TEST(BaseRectTest, move_inside) {
    BaseRect<int32_t, BasePoint<int32_t>> rect1(-5, -6, 20, 30);
    BaseRect<int32_t, BasePoint<int32_t>> rect2(190, 180, 20, 30);
    BaseRect<int32_t, BasePoint<int32_t>> big_rect(0, 0, 200, 200);

    auto newRect1 = geom::rect::move_inside(rect1, big_rect);

    EXPECT_EQ(0, newRect1.x);
    EXPECT_EQ(0, newRect1.y);
    EXPECT_EQ(20, newRect1.width);
    EXPECT_EQ(30, newRect1.height);

    auto newRect2 = geom::rect::move_inside(rect2, big_rect);

    EXPECT_EQ(180, newRect2.x);
    EXPECT_EQ(170, newRect2.y);
    EXPECT_EQ(20, newRect2.width);
    EXPECT_EQ(30, newRect2.height);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
