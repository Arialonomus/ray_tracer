#include "gtest/gtest.h"
#include "world.hpp"

#include <vector>

#include "light.hpp"
#include "sphere.hpp"
#include "ray.hpp"
#include "transform.hpp"
#include "intersection.hpp"

// Tests the default constructor
TEST(GraphicsWorld, DefaultConstructor)
{
    const gfx::PointLight light_source_expected{ gfx::Color{ 1, 1, 1 },
                                    gfx::createPoint(-10, 10, -10) };
    const gfx::World world{ };

    ASSERT_EQ(world.getLightSource().intensity, light_source_expected.intensity);
    ASSERT_EQ(world.getLightSource().position, light_source_expected.position);
    ASSERT_TRUE(world.isEmpty());
}

// Tests the point light constructor
TEST(GraphicsWorld, PointLightConstructor)
{
    const gfx::PointLight light_source_expected{ gfx::Color{ 0.5, 0.5, 0.5 },
                                           gfx::createPoint(-5, 5, -5) };
    const gfx::World world{ light_source_expected };

    ASSERT_EQ(world.getLightSource().intensity, light_source_expected.intensity);
    ASSERT_EQ(world.getLightSource().position, light_source_expected.position);
    ASSERT_TRUE(world.isEmpty());
}

// Tests the object list constructor (single object)
TEST(GraphicsWorld, ObjectListConstructorSingleObject)
{
    const gfx::PointLight light_source_expected{ gfx::Color{ 1, 1, 1 },
                                           gfx::createPoint(-10, 10, -10) };
    gfx::Sphere sphere_a{ };
    const gfx::World world{ sphere_a };

    ASSERT_EQ(world.getLightSource().intensity, light_source_expected.intensity);
    ASSERT_EQ(world.getLightSource().position, light_source_expected.position);
    ASSERT_EQ(world.getObjectCount(), 1);
    ASSERT_EQ(dynamic_cast<const gfx::Sphere&>(world.getObjectAt(0)), sphere_a);
}

// Tests the object list constructor (multiple objects)
TEST(GraphicsWorld, ObjectListConstructorMultipleObjects)
{
    const gfx::PointLight light_source_expected{ gfx::Color{ 1, 1, 1 },
                                                 gfx::createPoint(-10, 10, -10) };
    gfx::Sphere sphere_a{ };
    gfx::Sphere sphere_b{ };
    const gfx::World world{ sphere_a, sphere_b };

    ASSERT_EQ(world.getLightSource().intensity, light_source_expected.intensity);
    ASSERT_EQ(world.getLightSource().position, light_source_expected.position);
    ASSERT_EQ(world.getObjectCount(), 2);
    ASSERT_EQ(dynamic_cast<const gfx::Sphere&>(world.getObjectAt(0)), sphere_a);
    ASSERT_EQ(dynamic_cast<const gfx::Sphere&>(world.getObjectAt(1)), sphere_b);
}

// Tests the standard constructor
TEST(GraphicsWorld, StandardConstructor)
{
    const gfx::PointLight light_source_expected{ gfx::Color{ 0.5, 0.5, 0.5 },
                                                 gfx::createPoint(-5, 5, -5) };
    gfx::Sphere sphere_a{ };
    gfx::Sphere sphere_b{ };
    const gfx::World world{ light_source_expected, sphere_a, sphere_b };

    ASSERT_EQ(world.getLightSource().intensity, light_source_expected.intensity);
    ASSERT_EQ(world.getLightSource().position, light_source_expected.position);
    ASSERT_EQ(world.getObjectCount(), 2);
    ASSERT_EQ(dynamic_cast<const gfx::Sphere&>(world.getObjectAt(0)), sphere_a);
    ASSERT_EQ(dynamic_cast<const gfx::Sphere&>(world.getObjectAt(1)), sphere_b);
}

// Tests getting the number of objects in the world
TEST(GraphicsWorld, GetObjectCount)
{
    // Test the object count of an empty world
    const gfx::World world_a{ };

    ASSERT_EQ(world_a.getObjectCount(), 0);
    ASSERT_TRUE(world_a.isEmpty());

    // Test the object count of a populated world
    const gfx::Sphere sphere_a{ };
    const gfx::Sphere sphere_b{ };
    const gfx::Sphere sphere_c{ };
    const gfx::World world_b{ sphere_a, sphere_b, sphere_c };

    ASSERT_EQ(world_b.getObjectCount(), 3);
}

// Tests adding an object to the world
TEST(GraphicsWorld, AddObject)
{
    gfx::World world{ };
    const gfx::Sphere sphere_a{ };

    world.addObject(sphere_a);

    ASSERT_EQ(world.getObjectCount(), 1);
    ASSERT_EQ(dynamic_cast<const gfx::Sphere&>(world.getObjectAt(0)), sphere_a);
}

// Tests calculating world intersections
TEST(GraphicsWorld, WorldIntersections)
{
    gfx::Sphere sphere_a{ };
    gfx::Sphere sphere_b{ gfx::createScalingMatrix(0.5) };
    const gfx::World world{ sphere_a, sphere_b };
    const gfx::Ray ray{ 0, 0, -5,
                        0, 0, 1 };

    const std::vector<gfx::Intersection> world_intersections{ world.getIntersections(ray) };

    EXPECT_EQ(world_intersections.size(), 4);
    EXPECT_FLOAT_EQ(world_intersections.at(0).getT(), 4);
    EXPECT_FLOAT_EQ(world_intersections.at(1).getT(), 4.5);
    EXPECT_FLOAT_EQ(world_intersections.at(2).getT(), 5.5);
    EXPECT_FLOAT_EQ(world_intersections.at(3).getT(), 6);
}

// Tests calculating whether various points are in shadow
TEST(GraphicsWorld, PointIsShadowed)
{
    gfx::Material material{ };
    material.setColor(0.8, 1.0, 0.6);
    material.setDiffuse(0.7);
    material.setSpecular(0.2);

    const gfx::Sphere sphere_a{ material };
    const gfx::Sphere sphere_b{ gfx::createScalingMatrix(0.5) };

    const gfx::World world{ sphere_a, sphere_b };

    // Test a point where nothing is collinear with point and light source
    ASSERT_FALSE(world.isShadowed(gfx::createPoint(0, 10, 0)));

    // Test a point where an object is between the point and the light
    ASSERT_TRUE(world.isShadowed(gfx::createPoint(10, -10, 10)));

    // Test a point where the light source is between the point and an object
    ASSERT_FALSE(world.isShadowed(gfx::createPoint(-20, 20, -20)));

    // Test a point where the point is between an object and the light source
    ASSERT_FALSE(world.isShadowed(gfx::createPoint(-2, 2, -2)));
}

// Test shading a color when a ray misses all objects in a world
TEST(GraphicsWorld, CalculatePixelColorMiss)
{
    gfx::Material material{ };
    material.setColor(0.8, 1.0, 0.6);
    material.setDiffuse(0.7);
    material.setSpecular(0.2);

    gfx::Sphere sphere_a{ material };
    gfx::Sphere sphere_b{ gfx::createScalingMatrix(0.5) };

    const gfx::World world{ sphere_a, sphere_b };
    const gfx::Ray ray{ 0, 0, -5,
                        0, 1, 0 };

    const gfx::Color pixel_color_expected{ 0, 0, 0 };
    const gfx::Color pixel_color_actual{ world.calculatePixelColor(ray) };

    EXPECT_EQ(pixel_color_actual, pixel_color_expected);
}

// Test shading a color when a ray hits an object in world from the outside
TEST(GraphicsWorld, CalculatePixelColorHitOutside)
{
    gfx::Material material{ };
    material.setColor(0.8, 1.0, 0.6);
    material.setDiffuse(0.7);
    material.setSpecular(0.2);

    gfx::Sphere sphere_a{ material };
    gfx::Sphere sphere_b{ gfx::createScalingMatrix(0.5) };

    const gfx::World world{ sphere_a, sphere_b };
    const gfx::Ray ray{ 0, 0, -5,
                        0, 0, 1 };

    const gfx::Color pixel_color_expected{ 0.380661, 0.475827, 0.285496  };
    const gfx::Color pixel_color_actual{ world.calculatePixelColor(ray) };

    EXPECT_EQ(pixel_color_actual, pixel_color_expected);
}

// Test shading a color when a ray hits an object in world from the inside
TEST(GraphicsWorld, CalculatePixelColorHitInside)
{
    gfx::Material material{ };
    material.setColor(0.8, 1.0, 0.6);
    material.setDiffuse(0.7);
    material.setSpecular(0.2);

    gfx::Sphere sphere_a{ material };
    gfx::Sphere sphere_b{ gfx::createScalingMatrix(0.5) };
    const gfx::PointLight light_source{ gfx::Color{ 1, 1, 1 },
                                        gfx::createPoint( 0, 0.25, 0 )};
    const gfx::World world{ light_source, sphere_a, sphere_b };
    const gfx::Ray ray{ 0, 0, 0,
                        0, 0, 1 };

    const gfx::Color pixel_color_expected{ 0.904984, 0.904984, 0.904984  };
    const gfx::Color pixel_color_actual{ world.calculatePixelColor(ray) };

    EXPECT_EQ(pixel_color_actual, pixel_color_expected);
}

// Test shading a color when a ray intersection is behind the ray origin
TEST(GraphicsWorld, CalculatePixelColorHitBehind)
{
    gfx::Material sphere_a_material{ };
    sphere_a_material.setAmbient(1);
    sphere_a_material.setColor(0.8, 1.0, 0.6);
    sphere_a_material.setDiffuse(0.7);
    sphere_a_material.setSpecular(0.2);

    gfx::Material sphere_b_material{ };
    sphere_b_material.setAmbient(1);

    gfx::Sphere sphere_a{ sphere_a_material };
    gfx::Sphere sphere_b{ gfx::createScalingMatrix(0.5), sphere_b_material };
    const gfx::PointLight light_source{ gfx::Color{ 1, 1, 1 },
                                        gfx::createPoint( 0, 0.25, 0 )};
    const gfx::World world{ light_source, sphere_a, sphere_b };
    const gfx::Ray ray{ 0, 0, 0.75,
                        0, 0, -1 };

    const gfx::Color pixel_color_expected{ sphere_b.getMaterial().getColor() };
    const gfx::Color pixel_color_actual{ world.calculatePixelColor(ray) };

    EXPECT_EQ(pixel_color_actual, pixel_color_expected);
}

// Test shading a color when a ray intersection point is in shadow
TEST(GraphicsWorld, CalculatePixelColorInShadow)
{
    gfx::Sphere sphere_a{ };
    gfx::Sphere sphere_b{ gfx::createTranslationMatrix(0, 0, 10) };
    const gfx::PointLight light_source{ gfx::Color{ 1, 1, 1 },
                                        gfx::createPoint( 0, 0, -10 )};
    const gfx::World world{ light_source, sphere_a, sphere_b };
    const gfx::Ray ray{ 0, 0, 5,
                        0, 0, 1 };

    // Validate state is correctly initialized before repeating this calculation in calculatePixelColor()
    const std::vector<gfx::Intersection> world_intersections{ world.getIntersections(ray) };
    auto possible_hit{ getHit(world_intersections) };

    ASSERT_TRUE(possible_hit);
    const gfx::Intersection intersection_expected{ 4, &world.getObjectAt(1) };
    const gfx::Intersection& intersection_actual{ possible_hit.value() };

    EXPECT_EQ(intersection_actual, intersection_expected);

    const gfx::Color pixel_color_expected{ 0.1, 0.1, 0.1 };
    const gfx::Color pixel_color_actual{ world.calculatePixelColor(ray) };

    EXPECT_EQ(pixel_color_actual, pixel_color_expected);
}