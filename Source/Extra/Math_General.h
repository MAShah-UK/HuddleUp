#pragma once

#include <initializer_list>
#include <cmath>

using std::initializer_list;
using std::pow;

/*
 * Consider:
 * - Universal references.
 * - Constexpr functions.
*/

namespace Math
{

// Stateless:

template <typename T, typename U, typename V>
T clamp(const T& actual, const U& min, const V& max)
{
    if      (actual < min) return min;
    else if (actual > max) return max;
    else                   return actual;
}

template <typename T, typename U>
T min(const T& val1, const U& val2)
{
    return (val1 < val2) ? val1 : val2;
}

template <typename T, typename U>
T max(const T& val1, const U& val2)
{
    return (val1 > val2) ? val1 : val2;
}

template <typename T>
T abs(const T& val)
{
    return val > 0 ? val : -val;
}

template <typename T, typename U, typename V>
bool tolerance(const T& val1, const U& val2, const V& tol)
{
    return abs(val1 - val2) <= abs(tol) ? true : false;
}

template <typename T>
T sum(const T& val1)
{
    return val1;
}

template <typename T, typename ... Types>
T sum(const T& val1, const Types& ... Params)
{
    return val1 + sum(Params...);
}

template <typename T, typename ... Types>
T average(const T& val1, const Types& ... Params)
{
    return sum(val1, Params...) / (sizeof...(Params) + 1);
}

template <typename T>
T isPositive(const T& val1)
{
    return val1 > 0 ? true : false;
}

template <typename T>
T vectorLength(initializer_list<T> comp)
{
    T sum = 0;

    for (const T& val : comp)
       sum += pow(val, 2);

    return pow(sum, 0.5);
}

// With state:

class Interpolate
{
public:

    // Interpolation key points.
    double initialX   = 0;
    double finalX     = 0;
    double initialY   = 0;
    double finalY     = 0;

    double currentX   = 0;
    double incrementX = 0;

    enum interpType {IT_linear, IT_sinusoidal, IT_smooth, IT_exponential};

    Interpolate();

    double interpolatedValue(bool applyIncrement = true, interpType iT = IT_smooth);
    bool outOfXRange();
};

}

