#include <iostream>
#include <string>
#include <functional>
#include <omp.h>
#include "../benchmark.hpp"
#include "../function_wrapper.hpp"

size_t sum(size_t num)
{
    size_t sum = 0;
    for (size_t i = 0; i < num; ++i)
    {
        sum += i;
    }
    return sum;
}

size_t sumAtomic(size_t num)
{
    size_t sum = 0;
    #pragma omp parallel for
    for (size_t i = 0; i < num; ++i)
    {
        #pragma omp atomic
        sum += i;
    }
    return sum;
}

size_t sumReduction(size_t num)
{
    size_t sum = 0;
    #pragma omp parallel for reduction(+ : sum)
    for (size_t i = 0; i < num; ++i)
    {
        sum += i;
    }
    return sum;
}

int main(int argc, char* argv[]) 
{
    size_t num = std::stoi(argv[1]);
    
    std::cout << "n = " << num << '\n';
    Benchmark::stat<size_t>({
        {"sum", function_wrapper<size_t, size_t>(sum, num)},
        {"sum_atomic", function_wrapper<size_t, size_t>(sumAtomic, num)},
        {"sum_reduction", function_wrapper<size_t, size_t>(sumReduction, num)}
        });
}
