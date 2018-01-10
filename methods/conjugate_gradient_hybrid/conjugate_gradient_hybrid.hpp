/**
   \file conjugate_gradient.hpp
   \brief powerll optimization method
   \author Junhua Gu
 */

#ifndef CONJUGATE_GRADIENT_HYBRID
#define CONJUGATE_GRADIENT_HYBRID
#define OPT_HEADER
#include <core/optimizer.hpp>
//#include <blitz/array.h>
#include <limits>
#include <cassert>
#include <cmath>
#include "../linmin/linmin.hpp"
#include <math/num_diff.hpp>
#include <algorithm>
#include <iostream>

namespace opt_utilities
{
    /**
       \brief Impliment of an optimization method
       \tparam rT return type of the object function
       \tparam pT parameter type of the object function
     */
    template <typename rT, typename pT> class conjugate_gradient_hybrid : public opt_method<rT, pT>
    {
      public:
        typedef pT array1d_type;
        typedef rT T;

      private:
        func_obj<rT, pT> *p_fo;
        optimizer<rT, pT> *p_optimizer;
        volatile bool bstop;
        // typedef blitz::Array<rT,2> array2d_type;

        const char *do_get_type_name () const
        {
            return "conjugate gradient method";
        }

      private:
        array1d_type start_point;
        array1d_type end_point;

      private:
        rT threshold;
        array1d_type g;
        array1d_type h;
        array1d_type xi;

      private:
        rT func (const pT &x)
        {
            assert (p_fo != 0);
            return p_fo->eval (x);
        }


      private:
        void clear_xi ()
        {
        }

        void init_xi (int n)
        {
            clear_xi ();
            g = array1d_type (n);
            h = array1d_type (n);
            xi = array1d_type (n);
        }


        void cg (array1d_type &p, const T ftol, int &iter, T &fret)
        {
            int converge_count_cg = 0;
            int count_no_cg = 0;
            bool cg_on = true;
            const int ITMAX = 200;
            const T EPS = std::numeric_limits<T>::epsilon ();

            int j, its;
            int n = p.size ();
            T gg, gam, fp, dgg;
            fp = func (p);
            xi = gradient (*p_fo, p);
            for (j = 0; j < n; ++j)
                {
                    g[j] = -xi[j];
                    xi[j] = h[j] = g[j];
                }
            for (its = 1; its <= ITMAX; ++its)
                {
                    std::cerr << "CG:" << cg_on << std::endl;

                    iter = its;
                    linmin (p, xi, fret, *p_fo);
                    // std::cerr<<"######:"<<its<<"\t"<<abs(fret-fp)/(abs(fret)+fabs(fp)+EPS)<<std::endl;
                    if (!cg_on)
                        {
                            ++count_no_cg;
                        }
                    else
                        {
                            count_no_cg = 0;
                        }
                    if (!cg_on && count_no_cg > 10)
                        {
                            cg_on = true;
                            // init_xi((int)get_size(start_point));
                            for (j = 0; j < n; ++j)
                                {
                                    g[j] = -xi[j];
                                    xi[j] = h[j] = g[j];
                                }
                        }
                    std::cerr << 2.0 * abs (fret - fp) << "\t"
                              << ftol * (abs (fret) + fabs (fp) + EPS) << std::endl;
                    if (2.0 * abs (fret - fp) <= ftol * (abs (fret) + fabs (fp) + EPS))
                        {
                            if (cg_on)
                                {
                                    cg_on = false;
                                    converge_count_cg++;
                                    std::cerr << "converge_count=" << converge_count_cg << std::endl;
                                }
                            if (converge_count_cg > 1)
                                {
                                    std::cerr << "exit" << std::endl;
                                    return;
                                }
                        }
                    else
                        {
                            converge_count_cg = 0;
                        }
                    std::cerr << "converge_count=" << converge_count_cg << std::endl;
                    fp = func (p);
                    xi = gradient (*p_fo, p);
                    if (cg_on)
                        {
                            dgg = gg = 0;
                            for (j = 0; j < n; ++j)
                                {
                                    gg += g[j] * g[j];
                                    // dgg+=(xi[j]+g[j])*xi[j];
                                    dgg += xi[j] * xi[j];
                                }
                            // std::cerr<<its<<"\t"<<gg<<std::endl;
                            if (gg == 0.0)
                                {
                                    std::cerr << "exit2" << std::endl;
                                    return;
                                }
                            gam = dgg / gg;
                            for (j = 0; j < n; ++j)
                                {
                                    g[j] = -xi[j];
                                    xi[j] = h[j] = g[j] + gam * h[j];
                                }
                        }
                }
            // std::cerr<<"Too many iterations in cg"<<std::endl;
        }


      public:
        conjugate_gradient_hybrid () : threshold (1e-4), g (0), h (0), xi (0)
        {
        }

        virtual ~conjugate_gradient_hybrid ()
        {
            clear_xi ();
        };

        conjugate_gradient_hybrid (const conjugate_gradient_hybrid<rT, pT> &rhs)
        : opt_method<rT, pT> (rhs), p_fo (rhs.p_fo), p_optimizer (rhs.p_optimizer),
          start_point (rhs.start_point), end_point (rhs.end_point), threshold (rhs.threshold),
          g (0), h (0), xi (0)
        {
        }

        conjugate_gradient_hybrid<rT, pT> &operator= (const conjugate_gradient_hybrid<rT, pT> &rhs)
        {
            threshold = rhs.threshold;
            p_fo = rhs.p_fo;
            p_optimizer = rhs.p_optimizer;
            start_point = rhs.start_point;
            end_point = rhs.end_point;
            threshold = rhs.threshold;
        }

        opt_method<rT, pT> *do_clone () const
        {
            return new conjugate_gradient_hybrid<rT, pT> (*this);
        }

        void do_set_start_point (const array1d_type &p)
        {
            resize (start_point, get_size (p));
            opt_assign (start_point, p);
        }

        array1d_type do_get_start_point () const
        {
            return start_point;
        }

        void do_set_lower_limit (const array1d_type &p)
        {
        }

        void do_set_upper_limit (const array1d_type &p)
        {
        }

        void do_set_precision (rT t)
        {
            threshold = t;
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
            bstop = false;
            init_xi ((int)get_size (start_point));

            int iter = 100;
            opt_assign (end_point, start_point);
            rT fret;
#if 0
      for(int i=0;i<get_size(start_point);++i)
	{
	  array1d_type direction(start_point.size());
	  direction[i]=1;
	  linmin(end_point,direction,fret,(*p_fo));
	}
#endif
            cg (end_point, threshold, iter, fret);
            return end_point;
        }

        void do_stop ()
        {
            bstop = true;
        }
    };
}


#endif
// EOF
