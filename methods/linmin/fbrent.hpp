#ifndef BRENT_HPP
#define BRENT_HPP
#define OPT_HEADER
#include <iostream>
#include "bas_util.hpp"
//#include "optimizer.hpp"
namespace opt_utilities
{
    // functional style
    template <typename T, typename fT> T fbrent (T ax, T bx, T cx, const fT &f, T tol, T &xmin)
    {
        const int ITMAX = 100;
        const T CGOLD = 0.3819660;
        const T ZEPS = std::numeric_limits<T>::epsilon () * 1.e-3;

        int iter;
        T a = 0, b = 0, d (0), etemp = 0, fu = 0, fv = 0, fw = 0, fx = 0, p = 0, q = 0, r = 0,
          tol1 = 0, tol2 = 0, u = 0, v = 0, w = 0, x = 0, xm = 0;
        T e = 0.;
        a = (ax < cx ? ax : cx);
        b = (ax > cx ? ax : cx);
        x = w = v = bx;
        fw = fv = fx = f (x);
        for (iter = 0; iter < ITMAX; ++iter)
            {
                xm = .5 * (a + b);
                tol2 = 2. * (tol1 = tol * tabs (x) + ZEPS);
                if (tabs (T (x - xm)) <= (tol2 - .5 * (b - a)))
                    {
                        xmin = x;
                        return fx;
                    }
                if (tabs (e) > tol1)
                    {
                        r = (x - w) * (fx - fv);
                        q = (x - v) * (fx - fw);
                        p = (x - v) * q - (x - w) * r;
                        q = 2. * (q - r);
                        if (q > 0.)
                            {
                                p = -p;
                            }
                        q = tabs (q);
                        etemp = e;
                        e = d;
                        if (tabs (p) >= tabs (T (T (.5) * p * etemp)) || p <= q * (a - x) || p >= q * (b - x))
                            {
                                d = CGOLD * (e = (x >= xm ? a - x : b - x));
                            }
                        else
                            {
                                d = p / q;
                                u = x + d;
                                if (u - a < tol2 || b - u < tol2)
                                    {
                                        d = sign (tol1, T (xm - x));
                                    }
                            }
                    }
                else
                    {
                        d = CGOLD * (e = (x >= xm ? a - x : b - x));
                    }
                u = (tabs (d) >= tol1 ? x + d : x + sign (tol1, d));
                fu = f (u);
                if (fu <= fx)
                    {
                        if (u >= x)
                            {
                                a = x;
                            }
                        else
                            {
                                b = x;
                            }
                        shft3 (v, w, x, u);
                        shft3 (fv, fw, fx, fu);
                    }
                else
                    {
                        if (u < x)
                            {
                                a = u;
                            }
                        else
                            {
                                b = u;
                            }
                        if (fu <= fw || w == x)
                            {
                                v = w;
                                w = u;
                                fv = fw;
                                fw = fu;
                            }
                        else if (fu <= fv || v == x || v == w)
                            {
                                v = u;
                                fv = fu;
                            }
                    }
            }
        std::cerr << "Too many iterations in brent" << std::endl;
        xmin = x;
        return fx;
    }
}

#endif
