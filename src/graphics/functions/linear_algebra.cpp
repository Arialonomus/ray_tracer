#include "linear_algebra.hpp"

#include <cmath>

namespace gfx{
    std::vector<double> getSubmatrix(std::span<const double> matrix_values,
                                    size_t row_to_remove,
                                    size_t col_to_remove)
    {
        const size_t matrix_dimension{ static_cast<size_t>(std::sqrt(matrix_values.size())) };
        const size_t submatrix_dimension{ matrix_dimension - 1 };
        std::vector<double> submatrix_values(submatrix_dimension * submatrix_dimension);

        for (int row = 0; row < submatrix_dimension; ++row)
            for (int col = 0; col < submatrix_dimension; ++col) {
                submatrix_values[row * submatrix_dimension + col] =
                        matrix_values[(row + (row >= row_to_remove ? 1 : 0)) * matrix_dimension +
                        (col + (col >= col_to_remove ? 1 : 0))];
            }
        return submatrix_values;
    }

    double calculateDeterminant(std::span<const double> matrix_values)
    {
        const size_t matrix_dimension{ static_cast<size_t>(std::sqrt(matrix_values.size())) };

        // Base Case: 2x2 Matrix
        if (matrix_dimension == 2)
            return matrix_values[0] * matrix_values[3] - matrix_values[1] * matrix_values[2];

        // Recursive Case: Calculate the determinant of the submatrix
        double determinant = 0;
        for (int col = 0; col < matrix_dimension; ++col) {
            const double minor = calculateDeterminant(getSubmatrix(matrix_values, 0, col));
            determinant += matrix_values[col] * (0 + col % 2 == 0 ? minor : -minor);
        }
        return determinant;
    }

    std::vector<double> calculateInverse(std::span<const double> matrix_values, const double determinant)
    {
        const size_t matrix_dimension{ static_cast<size_t>(std::sqrt(matrix_values.size())) };
        std::vector<double> inverse_matrix_values(matrix_values.size());

        for (int row = 0; row < matrix_dimension; ++row)
            for (int col = 0; col < matrix_dimension; ++col) {
                const double minor = calculateDeterminant(getSubmatrix(matrix_values, row, col));
                const double cofactor = (row + col) % 2 == 0 ? minor : -minor;
                inverse_matrix_values[col * matrix_dimension + row] = cofactor / determinant;
            }
        return inverse_matrix_values;
    }
}