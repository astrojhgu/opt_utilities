/**
   \file num_diff.hpp
   \brief performing n-dim differential operation on func objects
   \author Junhua Gu
 */


#ifndef NUMDIFF_HPP
#define NUMDIFF_HPP
#define OPT_HEADER
#include <core/optimizer.hpp>
#include <core/opt_traits.hpp>
#include <algorithm>
#include <limits>
#include <cmath>

namespace opt_utilities
{
    /**
       calculate the numerical differential of a func_obj
    */
    template <typename rT, typename pT> class diff_func_obj : public func_obj<rT, pT>
    {
      private:
        virtual pT do_gradient (const pT &p) = 0;

      public:
        pT gradient (const pT &p)
        {
            return do_gradient (p);
        }
    };


    template <typename rT, typename pT> rT gradient (func_obj<rT, pT> &f, pT &p, size_t n)
    {
        rT ep = std::sqrt (std::numeric_limits<rT>::epsilon ());

        rT result;

        typename element_type_trait<pT>::element_type old_value = get_element (p, n);

        typename element_type_trait<pT>::element_type h = std::max (get_element (p, n), rT (1)) * ep;
        set_element (p, n, old_value + h);
        rT v2 = f (p);
        set_element (p, n, old_value - h);
        rT v1 = f (p);
        set_element (p, n, old_value);
        result = (v2 - v1) / h / 2;
        return result;
    }

    template <typename rT, typename pT> pT gradient (func_obj<rT, pT> &f, pT &p)
    {
        diff_func_obj<rT, pT> *pdfo = 0;
        if (pdfo = dynamic_cast<diff_func_obj<rT, pT> *> (&f))
            {
                return pdfo->gradient (p);
            }
        pT result;
        resize (result, get_size (p));
        for (int i = 0; i < get_size (p); ++i)
            {
                set_element (result, i, gradient (f, p, i));
            }
        return result;
    }


    template <typename rT, typename pT>
    rT hessian (func_obj<rT, pT> &f, const pT &p, size_t m, size_t n)
    {
        rT ep = std::sqrt (std::numeric_limits<rT>::epsilon ());
        typename element_type_trait<pT>::element_type hn = std::max (get_element (p, n), rT (1)) * ep;
        typename element_type_trait<pT>::element_type hm = std::max (get_element (p, m), rT (1)) * ep;
        pT p11;
        resize (p11, get_size (p));
        pT p00;
        resize (p00, get_size (p));
        pT p10;
        resize (p10, get_size (p));
        pT p01;
        resize (p01, get_size (p));

        for (size_t i = 0; i < get_size (p); ++i)
            {
                set_element (p11, i, get_element (p, i));
                set_element (p00, i, get_element (p, i));
                set_element (p01, i, get_element (p, i));
                set_element (p10, i, get_element (p, i));
            }

        set_element (p11, m, get_element (p11, m) + hm);
        set_element (p11, n, get_element (p11, n) + hn);
        set_element (p00, m, get_element (p00, m) - hm);
        set_element (p00, n, get_element (p00, n) - hn);
        set_element (p10, m, get_element (p10, m) + hm);
        set_element (p10, n, get_element (p10, n) - hn);
        set_element (p01, m, get_element (p01, m) - hm);
        set_element (p01, n, get_element (p01, n) + hn);

        rT result = (f (p11) + f (p00) - f (p01) - f (p10)) / (4 * hm * hn);
        return result;
    }


    template <typename rT, typename pT> rT div (func_obj<rT, pT> &f, const pT &p)
    {
        rT result = 0;
        for (int i = 0; i < get_size (p); ++i)
            {
                result += hessian (f, p, i, i);
            }
        return result;
    }
}

#endif
