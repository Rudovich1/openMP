#pragma once

#include <functional>

template<class Type>
struct Tag
{
    using type = Type; 
    constexpr Tag(){}
};

template<typename ResultType, typename ... Args>
std::function<ResultType()> function_wrapper(typename Tag<std::function<ResultType(Args ...)>>::type fun, Args ... args)
{
    return [fun, args ...](){return fun(args ...);};
}