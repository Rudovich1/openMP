#include "../benchmark.hpp"
#include "../function_wrapper.hpp"
#include <vector>
#include <random>
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>

using Matrix = std::vector<std::vector<double>>;

class MatrixException: public std::exception
{
    std::string msg_;
    static inline const std::string pref_ = "Matrix exception";
public:

    MatrixException() {}
    MatrixException(const std::string& msg): msg_(msg) {}
    MatrixException(std::string&& msg): msg_(msg) {}
    MatrixException(const char* msg): msg_(msg) {}

    const char* what() const noexcept override 
    {
        return (pref_ + "(" + msg_ + ")").c_str();
    }
};

Matrix genMatrix(size_t num_rows, size_t num_columns)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(-10., 10.);

    Matrix matrix(num_rows, std::vector<double>(num_columns));

    for (size_t i = 0; i < num_rows; ++i)
    {
        for (size_t j = 0; j < num_columns; ++j)
        {
            matrix[i][j] = dist(gen);
        }
    }

    return matrix;
}

Matrix prod(const Matrix& matrix1, const Matrix& matrix2)
{
    if (matrix1.empty() || matrix1[0].empty() || matrix2.empty() || matrix2[0].empty() || matrix1[0].size() != matrix2.size())
    {
        throw MatrixException();
    }

    size_t num_rows = matrix1.size();
    size_t num_columns = matrix2[0].size();
    size_t prod_size = matrix1[0].size();

    Matrix res_matrix(num_rows, std::vector<double>(num_columns));

    for (size_t i = 0; i < num_rows; ++i)
    {
        for (size_t j = 0; j < num_columns; ++j)
        {
            for (size_t k = 0; k < prod_size; ++k)
            {
                res_matrix[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    return res_matrix;
}

Matrix prodParallel(const Matrix& matrix1, const Matrix& matrix2)
{
    if (matrix1.empty() || matrix1[0].empty() || matrix2.empty() || matrix2[0].empty() || matrix1[0].size() != matrix2.size())
    {
        throw MatrixException();
    }

    size_t num_rows = matrix1.size();
    size_t num_columns = matrix2[0].size();
    size_t prod_size = matrix1[0].size();

    Matrix res_matrix(num_rows, std::vector<double>(num_columns));

    #pragma omp parallel for 
    for (size_t i = 0; i < num_rows; ++i)
    {
        #pragma omp parallel for
        for (size_t j = 0; j < num_columns; ++j)
        {
            for (size_t k = 0; k < prod_size; ++k)
            {
                res_matrix[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    return res_matrix;
}

void printMatrix(const Matrix& matrix)
{
    std::cout << std::setprecision(6) << std::fixed;
    for (size_t i = 0; i < matrix.size(); ++i)
    {
        std::cout << "[ ";
        for (size_t j = 0; j < matrix[0].size(); ++j)
        {
            std::cout << std::setw(10) << std::left << matrix[i][j] << " "; 
        }
        std::cout << "]\n";
    }
}

std::ostream& operator<<(std::ostream& out, const Matrix& matrix)
{
    for (auto& i: matrix)
    {
        for (auto& j: i)
        {
            out << std::setw(10) << j << " "; 
        }
        out << '\n';
    }
    return out;
}

int main(int argc, char* argv[])
{
    size_t matrix1_num_rows = std::stoull(argv[1]);
    size_t matrix1_num_columns = std::stoull(argv[2]);
    size_t matrix2_num_columns = std::stoull(argv[3]);

    Matrix matrix1 = genMatrix(matrix1_num_rows, matrix1_num_columns);
    Matrix matrix2 = genMatrix(matrix1_num_columns, matrix2_num_columns);

    std::ofstream out("res.txt");

    Benchmark::stat<Matrix>(out, 
    {
        {"prod", function_wrapper<Matrix, Matrix, Matrix>(prod, matrix1, matrix2)},
        {"prod parallel", function_wrapper<Matrix, Matrix, Matrix>(prodParallel, matrix1, matrix2)}
    });

    out << '\n';
    out << "matrix 1: \n" << matrix1 << '\n' << "matrix 2: \n" << matrix2;
}