#include "gtest/gtest.h"
#include "plane.hpp"

#include "matrix4.hpp"
#include "material.hpp"
#include "transform.hpp"
#include "ray.hpp"

// Tests the default constructor
TEST(GraphicsPlane, DefaultConstructor)
{
const gfx::Plane plane{ };
const gfx::Matrix4 transform_expected{ gfx::createIdentityMatrix() };
const gfx::Material material_expected{ };

ASSERT_EQ(plane.getTransform(), transform_expected);
ASSERT_EQ(plane.getMaterial(), material_expected);
}

// Tests the standard constructor
TEST(GraphicsPlane, StandardConstructor)
{
const gfx::Color color_expected{ 0.5, 0.5, 0.5 };
const gfx::Material material_expected{ color_expected, 0.5, 0.5, 0.5, 50 };
const gfx::Plane plane{ gfx::createScalingMatrix(5), material_expected };
const gfx::Matrix4 transform_expected{ gfx::createScalingMatrix(5) };

ASSERT_EQ(plane.getTransform(), transform_expected);
ASSERT_EQ(plane.getMaterial(), material_expected);
}

// Tests the standard constructor (with default material)
TEST(GraphicsPlane, StandardConstructorDefaultMaterial)
{
const gfx::Plane plane{ gfx::createScalingMatrix(5) };
const gfx::Matrix4 transform_expected{ gfx::createScalingMatrix(5) };
const gfx::Material material_expected{ };

ASSERT_EQ(plane.getTransform(), transform_expected);
ASSERT_EQ(plane.getMaterial(), material_expected);
}

// Tests the standard constructor (with default transform)
TEST(GraphicsPlane, StandardConstructorDefaultTransform)
{
const gfx::Color color_expected{ 0.5, 0.5, 0.5 };
const gfx::Material material_expected{ color_expected, 0.5, 0.5, 0.5, 50 };
const gfx::Plane plane{ material_expected };
const gfx::Matrix4 transform_expected{ gfx::createIdentityMatrix() };

ASSERT_EQ(plane.getTransform(), transform_expected);
ASSERT_EQ(plane.getMaterial(), material_expected);
}

// Tests the copy constructor
TEST(GraphicsPlane, CopyConstructor)
{
const gfx::Matrix4 transform_expected{ gfx::createScalingMatrix(5) };
const gfx::Color color_expected{ 0.5, 0.5, 0.5 };
const gfx::Material material_expected{ color_expected, 0.5, 0.5, 0.5, 50 };
const gfx::Plane plane_src{ transform_expected, material_expected };
const gfx::Plane plane_cpy{ plane_src };

ASSERT_EQ(plane_cpy.getTransform(), transform_expected);
ASSERT_EQ(plane_cpy.getMaterial(), material_expected);
}

// Tests the assignment operator
TEST(GraphicsPlane, AssignmentOperator)
{

const gfx::Matrix4 transform_expected{ gfx::createScalingMatrix(5) };
const gfx::Color color_expected{ 0.5, 0.5, 0.5 };
const gfx::Material material_expected{ color_expected, 0.5, 0.5, 0.5, 50 };
const gfx::Plane plane_a{ transform_expected, material_expected };
gfx::Plane plane_b{ };

plane_b = plane_a;

ASSERT_EQ(plane_b.getTransform(), transform_expected);
ASSERT_EQ(plane_b.getMaterial(), material_expected);
}

// Tests the equality operator
TEST(GraphicsPlane, EqualityOperator)
{
const gfx::Plane plane_a{ gfx::createScalingMatrix(5) };
const gfx::Plane plane_b{ gfx::createScalingMatrix(5) };

ASSERT_TRUE(plane_a == plane_b);
}

// Tests the inequality operator
TEST(GraphicsPlane, InequalityOperator)
{
const gfx::Plane plane_a{ gfx::createScalingMatrix(5) };
const gfx::Plane plane_b{ };

ASSERT_TRUE(plane_a != plane_b);
}
