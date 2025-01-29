#include "gtest/gtest.h"
#include "parse.hpp"

#include <cmath>

#include <nlohmann/json.hpp>

#include "matrix4.hpp"
#include "transform.hpp"
#include "vector4.hpp"

using json = nlohmann::json;

// Tests parsing and creating a translation matrix from JSON data
TEST(RayTracerParse, ParseTranslateMatrixData)
{
    // Test parsing a translation matrix
    const json translate_matrix_data{
            { "type",   "translate" },
            { "values", json::array({ 0, 0, 5 }) }
    };
    const gfx::Matrix4 translate_matrix_expected{ gfx::createTranslationMatrix(0, 0, 5) };
    const gfx::Matrix4 translate_matrix_actual{ data::parseTransformMatrixData(translate_matrix_data) };

    ASSERT_EQ(translate_matrix_actual, translate_matrix_expected);
}

// Tests parsing and creating scaling matrices from JSON data
TEST(RayTracerParse, ParseScalingMatrixData)
{
    // Test parsing a uniform scaling matrix
    const json uniform_scale_matrix_data{
            { "type",   "scale" },
            { "values", json::array({ 10 }) }
    };
    const gfx::Matrix4 uniform_scale_matrix_expected{ gfx::createScalingMatrix(10) };
    const gfx::Matrix4 uniform_scale_matrix_actual{
            data::parseTransformMatrixData(uniform_scale_matrix_data)
    };

    ASSERT_EQ(uniform_scale_matrix_actual, uniform_scale_matrix_expected);

    // Test parsing a non-uniform scaling matrix
    const json scale_matrix_data{
            { "type",   "scale" },
            { "values", json::array({ 2, 4, 6 }) }
    };
    const gfx::Matrix4 scale_matrix_expected{ gfx::createScalingMatrix(2, 4, 6) };
    const gfx::Matrix4 scale_matrix_actual{
            data::parseTransformMatrixData(scale_matrix_data)
    };

    ASSERT_EQ(scale_matrix_actual, scale_matrix_expected);
}

// Tests parsing and creating rotation matrices from JSON data
TEST(RayTracerParse, ParseRotationMatrixData)
{
    // Test parsing an x-axis rotation matrix
    const json x_rotation_matrix_data{
            {"type", "rotate_x"},
            {"values", json::array({M_PI_2f})}
    };
    const gfx::Matrix4 x_rotation_matrix_expected{ gfx::createXRotationMatrix(M_PI_2f) };
    const gfx::Matrix4 x_rotation_matrix_actual{
            data::parseTransformMatrixData(x_rotation_matrix_data)
    };

    ASSERT_EQ(x_rotation_matrix_actual, x_rotation_matrix_expected);

    // Test parsing a y-axis rotation matrix
    const json y_rotation_matrix_data{
            {"type", "rotate_y"},
            {"values", json::array({M_PI_2f})}
    };
    const gfx::Matrix4 y_rotation_matrix_expected{ gfx::createYRotationMatrix(M_PI_2f) };
    const gfx::Matrix4 y_rotation_matrix_actual{
            data::parseTransformMatrixData(y_rotation_matrix_data)
    };

    ASSERT_EQ(y_rotation_matrix_actual, y_rotation_matrix_expected);

    // Test parsing a z-axis rotation matrix
    const json z_rotation_matrix_data{
            {"type", "rotate_z"},
            {"values", json::array({M_PI_2f})}
    };
    const gfx::Matrix4 z_rotation_matrix_expected{ gfx::createZRotationMatrix(M_PI_2f) };
    const gfx::Matrix4 z_rotation_matrix_actual{
        data::parseTransformMatrixData(z_rotation_matrix_data)
    };

    ASSERT_EQ(z_rotation_matrix_actual, z_rotation_matrix_expected);
}

// Tests parsing and creating a skew matrix from JSON data
TEST(RayTracerParse, ParseSkewMatrixData)
{
    // Test parsing a translation matrix
    const json skew_matrix_data{
            { "type",   "skew" },
            { "values", json::array({ 0, 0, 0, 0, 0, 1 }) }
    };
    const gfx::Matrix4 skew_matrix_expected{ gfx::createSkewMatrix(0, 0, 0, 0, 0, 1) };
    const gfx::Matrix4 skew_matrix_actual{ data::parseTransformMatrixData(skew_matrix_data) };

    ASSERT_EQ(skew_matrix_actual, skew_matrix_expected);
}

// Tests that invalid JSON data causes an error
TEST(RayTracerParse, ParseMatrixInvalidJSON)
{
    // Test invalid transform matrix type
    const json error_rotation_matrix_data{
            {"type", "rotate"},
            {"values", json::array({M_PI_2f})}
    };

    EXPECT_THROW({
        const gfx::Matrix4 error_rotation_matrix_actual{
                data::parseTransformMatrixData(error_rotation_matrix_data) };
        }, std::invalid_argument);

    // Test invalid number of translate matrix values
    const json error_translate_matrix_data{
            { "type",   "translate" },
            { "values", json::array({ 0, 5 }) }
    };

    EXPECT_THROW({
         const gfx::Matrix4 error_translate_matrix_actual{
                 data::parseTransformMatrixData(error_translate_matrix_data) };
         }, std::invalid_argument);

    // Test invalid number of scale matrix values
    const json error_scale_matrix_data{
            { "type",   "scale" },
            { "values", json::array({ 0, 5 }) }
    };

    EXPECT_THROW({
         const gfx::Matrix4 error_scale_matrix_actual{
                 data::parseTransformMatrixData(error_scale_matrix_data) };
         }, std::invalid_argument);

    // Test invalid number of x-axis rotation matrix values
    const json error_x_rotation_matrix_data{
            { "type",   "rotate_x" },
            { "values", json::array({ 0, 5 }) }
    };

    EXPECT_THROW({
         const gfx::Matrix4 error_y_rotation_matrix_actual{
                 data::parseTransformMatrixData(error_x_rotation_matrix_data) };
         }, std::invalid_argument);

    // Test invalid number of x-axis rotation matrix values
    const json error_y_rotation_matrix_data{
            { "type",   "rotate_y" },
            { "values", json::array({ 0, 5 }) }
    };

    EXPECT_THROW({
         const gfx::Matrix4 error_y_rotation_matrix_actual{
                 data::parseTransformMatrixData(error_y_rotation_matrix_data) };
         }, std::invalid_argument);

    // Test invalid number of x-axis rotation matrix values
    const json error_z_rotation_matrix_data{
            { "type",   "rotate_z" },
            { "values", json::array({ 0, 5 }) }
    };

    EXPECT_THROW({
         const gfx::Matrix4 error_z_rotation_matrix_actual{
                 data::parseTransformMatrixData(error_z_rotation_matrix_data) };
         }, std::invalid_argument);

    // Test invalid number of x-axis rotation matrix values
    const json error_skew_matrix_data{
            { "type",   "skew" },
            { "values", json::array({ 0, 5 }) }
    };

    EXPECT_THROW({
         const gfx::Matrix4 error_skew_matrix_actual{
                 data::parseTransformMatrixData(error_skew_matrix_data) };
         }, std::invalid_argument);
}

// Test building a chained transformation matrix from parsed JSON data
TEST(RayTracerParse, BuildChainedTransformMatrix)
{
    const json transform_data_list{ json::array({
                {{ "type", "translate" }, { "values", json::array({ 10, 5, 7 }) }},
                {{ "type", "scale" }, { "values", json::array({ 5 }) }},
                {{ "type", "rotate_x" }, { "values", json::array({ M_PI_2f }) }}
        })
    };

    const gfx::Matrix4 transform_matrix_actual{ data::buildChainedTransformMatrix(transform_data_list) };
    const gfx::Vector4 point_initial{ gfx::createPoint(1, 0, 1) };
    const gfx::Vector4 point_expected{ gfx::createPoint(15, 0, 7) };

    const gfx::Vector4 point_actual{ transform_matrix_actual * point_initial };

    EXPECT_EQ(point_actual, point_expected);
}