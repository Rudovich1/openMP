#include "../benchmark.hpp"
#include "../function_wrapper.hpp"
#include <omp.h>
#include <iostream>
#include <iomanip>
#include <string>

double pi_reduction(size_t num, size_t num_threads_)
{
    double res = 0;
    #pragma omp parallel for reduction(+ : res) num_threads(num_threads_)
    for (size_t i = 0; i < num; ++i)
    {
        res += 4. / (num + ((i + 0.5) * (i + 0.5)) / (double)num);
    }
    return res;
}

int main()
{

    std::cout << std::setprecision(10) << std::fixed;

    Benchmark::stat<double>({
        {"[N=100|T=1]", function_wrapper<double, size_t, size_t>(pi_reduction, 100, 1)},
        {"[N=1000000|T=1]", function_wrapper<double, size_t, size_t>(pi_reduction, 1000000, 1)},
        {"[N=100|T=2]", function_wrapper<double, size_t, size_t>(pi_reduction, 100, 2)},
        {"[N=1000000|T=2]", function_wrapper<double, size_t, size_t>(pi_reduction, 1000000, 2)},
        {"[N=100|T=4]", function_wrapper<double, size_t, size_t>(pi_reduction, 100, 4)},
        {"[N=1000000|T=4]", function_wrapper<double, size_t, size_t>(pi_reduction, 1000000, 4)},
        {"[N=100|T=8]", function_wrapper<double, size_t, size_t>(pi_reduction, 100, 8)},
        {"[N=1000000|T=8]", function_wrapper<double, size_t, size_t>(pi_reduction, 1000000, 8)},
        {"[N=100|T=10]", function_wrapper<double, size_t, size_t>(pi_reduction, 100, 10)},
        {"[N=1000000|T=10]", function_wrapper<double, size_t, size_t>(pi_reduction, 1000000, 10)},
        {"[N=100|T=12]", function_wrapper<double, size_t, size_t>(pi_reduction, 100, 12)},
        {"[N=1000000|T=12]", function_wrapper<double, size_t, size_t>(pi_reduction, 1000000, 12)},

        });
}