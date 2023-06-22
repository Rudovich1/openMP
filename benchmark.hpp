#pragma once

#include <chrono>
#include <functional>
#include <iostream>
#include <string>
#include <iomanip>

#define CHRONO std::chrono

class Benchmark
{
    CHRONO::steady_clock::time_point start_time_;
    CHRONO::duration<double>& res_;

public:

    Benchmark(CHRONO::duration<double>& res): start_time_(CHRONO::steady_clock::now()), res_(res) {}
    ~Benchmark() {res_ = CHRONO::steady_clock::now() - start_time_;}

    template<typename ReturnType>
    static CHRONO::duration<double> bench(std::function<ReturnType()> fun, ReturnType& res)
    {
        CHRONO::steady_clock::time_point start = CHRONO::steady_clock::now();
        res = fun();
        return CHRONO::steady_clock::now() - start;
    }

    template<class ReturnType>
    static void stat(std::ostream& out, std::vector<std::pair<std::string, std::function<ReturnType()>>> funs)
    {
        ReturnType res;
        for (std::pair<std::string, std::function<ReturnType()>>& fun: funs)
        {
            auto work_time = CHRONO::duration_cast<CHRONO::milliseconds>(bench(fun.second, res)).count();
            out << std::setw(15) << std::left << fun.first << ": " << work_time << "ms.\n";
        }
    }
};