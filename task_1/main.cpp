#include "../benchmark.hpp"
#include "../function_wrapper.hpp"
#include <omp.h>
#include <iostream>
#include <iomanip>
#include <string>

double pi(size_t num)
{
    double res = 0;
    for (size_t i = 0; i < num; ++i)
    {
        res += 4. / (num + ((i + 0.5) * (i + 0.5)) / (double)num);
    }
    return res;
}

double pi_atomic(size_t num)
{
    double res = 0;
    #pragma omp parallel for
    for (size_t i = 0; i < num; ++i)
    {
        #pragma omp atomic
        res += 4. / (num + ((i + 0.5) * (i + 0.5)) / (double)num);
    }
    return res;
}

double pi_reduction(size_t num)
{
    double res = 0;
    #pragma omp parallel for reduction(+ : res)
    for (size_t i = 0; i < num; ++i)
    {
        res += 4. / (num + ((i + 0.5) * (i + 0.5)) / (double)num);
    }
    return res;
}

int main(int argc, char* argv[])
{
    size_t num = std::stoull(argv[1]);

    std::cout << "n = " << num << '\n';
    std::cout << std::setprecision(10) << std::fixed;

    Benchmark::stat<double>({
        {"pi", function_wrapper<double, size_t>(pi, num)},
        {"pi_atomic", function_wrapper<double, size_t>(pi_atomic, num)},
        {"pi_reduction", function_wrapper<double, size_t>(pi_reduction, num)}
        });
}