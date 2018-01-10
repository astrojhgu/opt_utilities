/**
   \file gsl_simplex.hpp
   \brief a simple wrapper for the GSL_SIMPLEX method
   \author Junhua Gu
 */

#ifndef GSL_SIMPLEX_METHOD
#define GSL_SIMPLEX_METHOD
#define OPT_HEADER
#include <core/optimizer.hpp>
//#include <blitz/array.h>
#include <vector>
#include <limits>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <gsl/gsl_multimin.h>
#include <iostream>


namespace opt_utilities
{

    /**
       \brief object function of the gsl simplex function
     */
    template <typename rT, typename pT> double gsl_func_adapter (const gsl_vector *v, void *params)
    {
        pT temp;
        temp.resize (v->size);
        for (size_t i = 0; i < get_size (temp); ++i)
            {
                set_element (temp, i, gsl_vector_get (v, i));
            }
        return ((func_obj<rT, pT> *)params)->eval (temp);
    }


    /**
       \brief wrapper for the gsl simplex optimization method
       \tparam return type of the object function
       \tparam param type of the object function
    */
    template <typename rT, typename pT> class gsl_simplex : public opt_method<rT, pT>
    {
      public:
        typedef pT array1d_type;
        typedef rT T;

      private:
        func_obj<rT, pT> *p_fo;
        optimizer<rT, pT> *p_optimizer;

        // typedef blitz::Array<rT,2> array2d_type;


      private:
        array1d_type start_point;
        array1d_type end_point;

      private:
        rT threshold;

      private:
        rT func (const pT &x)
        {
            assert (p_fo != 0);
            return p_fo->eval (x);
        }

        const char *do_get_type_name () const
        {
            return "gsl simplex";
        }

      public:
        gsl_simplex () : threshold (1e-4)
        {
        }

        virtual ~gsl_simplex (){};

        gsl_simplex (const gsl_simplex<rT, pT> &rhs)
        : p_fo (rhs.p_fo), p_optimizer (rhs.p_optimizer), start_point (rhs.start_point),
          end_point (rhs.end_point), threshold (rhs.threshold)
        {
        }

        gsl_simplex<rT, pT> &operator= (const gsl_simplex<rT, pT> &rhs)
        {
            threshold = rhs.threshold;
            p_fo = rhs.p_fo;
            p_optimizer = rhs.p_optimizer;
            opt_assign (start_point, rhs.start_point);
            opt_assign (end_point, rhs.end_point);
        }

        opt_method<rT, pT> *do_clone () const
        {
            return new gsl_simplex<rT, pT> (*this);
        }

        void do_set_start_point (const array1d_type &p)
        {
            start_point.resize (get_size (p));
            opt_assign (start_point, p);
        }

        array1d_type do_get_start_point () const
        {
            return start_point;
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
            const gsl_multimin_fminimizer_type *T = gsl_multimin_fminimizer_nmsimplex;
            gsl_multimin_fminimizer *s = NULL;
            gsl_vector *ss, *x;
            gsl_multimin_function minex_func;

            size_t iter = 0;
            int status;
            double size;

            /* Starting point */
            x = gsl_vector_alloc (get_size (start_point));
            //      gsl_vector_set (x, 0, 5.0);
            // gsl_vector_set (x, 1, 7.0);
            for (size_t i = 0; i != get_size (start_point); ++i)
                {
                    gsl_vector_set (x, i, get_element (start_point, i));
                }


            /* Set initial step sizes to 1 */
            ss = gsl_vector_alloc (get_size (start_point));
            gsl_vector_set_all (ss, 1.0);


            // foo f;
            /* Initialize method and iterate */
            minex_func.n = get_size (start_point);
            minex_func.f = &gsl_func_adapter<double, std::vector<double>>;
            minex_func.params = (void *)p_fo;

            s = gsl_multimin_fminimizer_alloc (T, get_size (start_point));
            gsl_multimin_fminimizer_set (s, &minex_func, x, ss);

            do
                {
                    iter++;
                    status = gsl_multimin_fminimizer_iterate (s);

                    if (status)
                        {
                            break;
                        }
                    // std::cerr<<"threshold="<<threshold<<std::endl;
                    size = gsl_multimin_fminimizer_size (s);
                    status = gsl_multimin_test_size (size, threshold);

                    if (status == GSL_SUCCESS)
                        {
                            // printf ("converged to minimum at\n");
                        }

                    // printf ("%5d %10.3e %10.3ef f() = %7.3f size = %.3f\n",
                    // iter,
                    // gsl_vector_get (s->x, 0),
                    // gsl_vector_get (s->x, 1),
                    //  s->fval, size);
                }
            while (status == GSL_CONTINUE);

            /*
          foo f;
          gsl_vector_set (x, 0, 0.0);
          gsl_vector_set (x, 1, 0.0);
          cout<<"fdsa ";
          cout<<gsl_func_adapter<double,vector<double> >(x,(void*)&f)<<endl;;

            */

            end_point.resize (get_size (start_point));
            for (size_t i = 0; i < get_size (start_point); ++i)
                {
                    set_element (end_point, i, gsl_vector_get (s->x, i));
                }

            gsl_vector_free (x);
            gsl_vector_free (ss);
            gsl_multimin_fminimizer_free (s);


            return end_point;
        }
    };
}


#endif
// EOF
