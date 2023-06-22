#include "../benchmark.hpp"
#include "../function_wrapper.hpp"
#include <vector>
#include <iostream>
#include <random>
#include <string>
#include <omp.h>
#include <algorithm>
#include <fstream>

template<typename ElemType>
void mergeSort(std::vector<ElemType>& vec, size_t begin, size_t end)
{
    if (begin == end)
    {
        return;
    }
    if (begin + 1 == end)
    {
        if (vec[begin] > vec[end])
        {
            std::swap(vec[begin], vec[end]);
        }
        return;
    }
    mergeSort(vec, begin, (begin + end) / 2);
    mergeSort(vec, (begin + end) / 2 + 1, end);

    std::vector<ElemType> tmp_res(end - begin + 1);

    size_t i = begin, j = (begin + end) / 2 + 1;
    while(i <= (begin + end) / 2 || j <= end)
    { 
        size_t index = i - begin + j - (begin + end)/2 - 1; 
        if (i > (begin + end) / 2)
        {
            tmp_res[index] = vec[j];
            ++j;
        }
        else if (j > end)
        {
            tmp_res[index] = vec[i];
            ++i;
        }
        else if (vec[i] <= vec[j])
        {
            tmp_res[index] = vec[i];
            ++i;
        }
        else
        {
            tmp_res[index] = vec[j];
            ++j;
        }
    }

    for (size_t k = 0; k < end - begin + 1; ++k)
    {
        vec[begin + k] = tmp_res[k];
    }
}

template<typename ElemType>
std::vector<ElemType> mergeSortWrapper(std::vector<ElemType> vec)
{
    mergeSort(vec, 0, vec.size() - 1);
    return vec;
}

template<typename ElemType>
void mergeSortParallel(std::vector<ElemType>& vec, size_t begin, size_t end)
{
    if (begin == end)
    {
        return;
    }
    if (begin + 1 == end)
    {
        if (vec[begin] > vec[end])
        {
            std::swap(vec[begin], vec[end]);
        }
        return;
    }

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            mergeSort(vec, begin, (begin + end) / 2);
        }
        #pragma omp section
        {
            mergeSort(vec, (begin + end) / 2 + 1, end);
        }
    }

    std::vector<ElemType> tmp_res(end - begin + 1);

    size_t i = begin, j = (begin + end) / 2 + 1;
    while(i <= (begin + end) / 2 || j <= end)
    { 
        size_t index = i - begin + j - (begin + end)/2 - 1; 
        if (i > (begin + end) / 2)
        {
            tmp_res[index] = vec[j];
            ++j;
        }
        else if (j > end)
        {
            tmp_res[index] = vec[i];
            ++i;
        }
        else if (vec[i] <= vec[j])
        {
            tmp_res[index] = vec[i];
            ++i;
        }
        else
        {
            tmp_res[index] = vec[j];
            ++j;
        }
    }

    for (size_t k = 0; k < end - begin + 1; ++k)
    {
        vec[begin + k] = tmp_res[k];
    }
}

template<typename ElemType>
std::vector<ElemType> mergeSortParallelWrapper(std::vector<ElemType> vec)
{
    mergeSortParallel(vec, 0, vec.size() - 1);
    return vec;
}

template<typename ElemType>
std::ostream& operator<<(std::ostream& out, const std::vector<ElemType>& vec)
{
    for (auto& elem: vec)
    {
        out << elem << " ";
    }
    return out;
}

template<typename IntElemType>
std::vector<IntElemType> genVec(size_t size)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<IntElemType> vec(size);

    for (size_t i = 0; i < size; ++i)
    {
        vec[i] = std::uniform_int_distribution<IntElemType>(0, INT16_MAX)(gen);
    }

    return vec;
}


template<typename ElemType>
std::vector<ElemType> sortWrapper(std::vector<ElemType> vec)
{
    std::sort(vec.begin(), vec.end());
    return vec;
}

int main(int argc, char* argv[])
{
    size_t vec_size = std::stoull(argv[1]);
    std::vector<int> vec = genVec<int>(vec_size);

    std::ofstream out("res.txt");

    Benchmark::stat<std::vector<int>>(out,
    {
        {"mergeSort", function_wrapper<std::vector<int>>(mergeSortWrapper<int>, vec)},
        {"mergeSortParallel", function_wrapper<std::vector<int>>(mergeSortParallelWrapper<int>, vec)},
        {"CPPsort", function_wrapper<std::vector<int>>(sortWrapper<int>, vec)}
    });

    out << '\n' << "vec: \n" << vec;
}