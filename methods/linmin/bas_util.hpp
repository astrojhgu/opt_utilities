#ifndef BAS_UTIL
#define BAS_UTIL
#define OPT_HEADER
#include <core/opt_traits.hpp>
#include <algorithm>
namespace opt_utilities
{
    template <typename T> T tabs (T x)
    {
        return T (x) < T (0) ? T (-x) : T (x);
    }

    template <typename T> T sqr (T x)
    {
        return x * x;
    }


    template <typename T> void shft3 (T &a, T &b, T &c, T d)
    {
        opt_assign (a, b);
        opt_assign (b, c);
        opt_assign (c, d);
    }

    template <typename T> void shft (T &a, T &b, T &c, T d)
    {
        opt_assign (a, b);
        opt_assign (b, c);
        opt_assign (c, d);
    }
    //  template <typename T>
    //  void swap(T& ax,T& bx)
    //{
    //  swap(ax,bx);
    //    T temp;
    // opt_assign(temp,ax);
    // opt_assign(ax,bx);
    // opt_assign(bx,temp);
    //}

    template <typename T> T sign (const T &a, const T &b)
    {
        return b >= 0 ? T (a >= 0 ? T (a) : T (-a)) : T (a >= 0 ? T (-a) : T (a));
    }

    template <typename T> T tmax (T a, T b)
    {
        return b > a ? T (b) : T (a);
    }

    template <typename T> void mov3 (T &a, T &b, T &c, T &d, T &e, T &f)
    {
        opt_assign (a, d);
        opt_assign (b, e);
        opt_assign (c, f);
    }
}

#endif
