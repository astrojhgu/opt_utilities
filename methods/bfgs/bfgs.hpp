/**
   \file bfgs.hpp
   \brief BFGS optimization method
   \author Junhua Gu
 */

#ifndef BFGS_METHOD
#define BFGS_METHOD
#define OPT_HEADER
#include <core/optimizer.hpp>
//#include <blitz/array.h>
#include <limits>
#include <cstdlib>
#include <core/opt_traits.hpp>
#include "../linmin/linmin.hpp"
#include <math/num_diff.hpp>
#include <cassert>
#include <cmath>
#include <ctime>
#include <vector>
#include <algorithm>
/*
 *
 */
#include <iostream>
using std::cerr;
using std::endl;

namespace opt_utilities
{

    template <typename rT, typename pT> class bfgs_method : public opt_method<rT, pT>
    {
      public:
        pT start_point;
        rT threshold;
        func_obj<rT, pT> *p_fo;
        optimizer<rT, pT> *p_optimizer;
        typedef typename element_type_trait<pT>::element_type element_type;
        element_type *mem_pool;
        element_type **invBk;
        bool bstop;

      private:
        rT func (const pT &x)
        {
            assert (p_fo != 0);
            return p_fo->eval (x);
        }

        const char *do_get_type_name () const
        {
            return "bfgs";
        }

      public:
        bfgs_method () : threshold (1e-5), p_fo (0), p_optimizer (0), mem_pool (0), invBk (0)
        {
        }

        virtual ~bfgs_method ()
        {
            destroy_workspace ();
        };

        bfgs_method (const bfgs_method<rT, pT> &rhs)
        : p_fo (rhs.p_fo), p_optimizer (rhs.p_optimizer), threshold (rhs.threshold), mem_pool (0),
          invBk (0)
        {
        }

        bfgs_method<rT, pT> &operator= (const bfgs_method<rT, pT> &rhs)
        {
            p_fo = rhs.p_fo;
            p_optimizer = rhs.p_optimizer;
            threshold = rhs.threshold;
        }

        opt_method<rT, pT> *do_clone () const
        {
            return new bfgs_method<rT, pT> (*this);
        }

        void init_workspace (int n)
        {
            destroy_workspace ();
            mem_pool = new element_type[n * n];
            invBk = new element_type *[n];

            for (size_t i = 0; i != n; ++i)
                {
                    invBk[i] = mem_pool + i * n;
                }
            for (size_t i = 0; i != n; ++i)
                {
                    for (size_t j = 0; j != n; ++j)
                        {
                            invBk[i][j] = (i == j ? 1 : 0);
                        }
                }
        }

        void destroy_workspace ()
        {
            delete[] mem_pool;
            delete[] invBk;
        }

      public:
        void do_set_start_point (const pT &p)
        {
            start_point = p;
            init_workspace (get_size (p));
        }

        pT do_get_start_point () const
        {
            return start_point;
        }

        void do_set_precision (rT t)
        {
            threshold = t >= 0 ? t : -t;
        }

        rT do_get_precision () const
        {
            return threshold;
        }

        void do_set_optimizer (optimizer<rT, pT> &o)
        {
            p_optimizer = &o;
            p_fo = p_optimizer->ptr_func_obj ();
        }

        pT do_optimize ()
        {
            pT s;
            pT &p = start_point;
            resize (s, get_size (start_point));
            pT old_grad;
            pT y;
            resize (old_grad, get_size (start_point));
            resize (y, get_size (start_point));
            for (;;)
                {
                    for (size_t i = 0; i != get_size (p); ++i)
                        {
                            set_element (old_grad, i, gradient (*p_fo, start_point, i));
                            set_element (s, i, 0);
                            for (size_t j = 0; j != get_size (p); ++j)
                                {
                                    s[i] += invBk[i][j] * old_grad[j];
                                }
                        }
                    double fret;
                    linmin (start_point, s, fret, *p_fo);

                    for (size_t i = 0; i != get_size (p); ++i)
                        {
                            set_element (y, i, gradient (*p_fo, start_point, i) - get_element (old_grad, i));
                        }

                    rT sy = 0;
                    pT invBy;
                    pT yinvB;
                    resize (invBy, get_size (p));
                    resize (yinvB, get_size (p));
                    for (size_t i = 0; i != get_size (p); ++i)
                        {
                            sy += s[i] * y[i];
                            for (size_t j = 0; j != get_size (p); ++j)
                                {
                                    invBy[i] += invBk[i][j] * y[j];
                                    yinvB[i] += y[j] * invBk[j][i];
                                }
                        }
                    if (sy < threshold && sy > -threshold)
                        {
                            return start_point;
                        }
                    rT yinvBy = 0;
                    for (size_t i = 0; i != get_size (p); ++i)
                        {
                            yinvBy += invBy[i] * y[i];
                        }

                    for (size_t i = 0; i < get_size (p); ++i)
                        {
                            for (size_t j = 0; j < get_size (p); ++j)
                                {
                                    invBk[i][j] += ((sy + yinvBy) * s[i] * s[j] / (sy * sy) -
                                                    (invBy[i] * s[j] + s[i] * yinvB[j]) / (sy));
                                }
                        }
                }
            return start_point;
        }

        void do_stop ()
        {
            bstop = true;
        }
    };
}


#endif
// EOF
