/**
   \file poly1d.hpp
   \brief 1d polynomial
   \author Junhua Gu
 */


#ifndef POLY_MODEL_H_
#define POLY_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>
#include <sstream>
#include <cassert>

namespace opt_utilities
{
    template <typename T, int n>
    class poly1d : public model<data<optvec<T>, optvec<T>>, optvec<T>, std::string>
    {
        typedef optvec<T> Tv;

      private:
        poly1d<T, n> *do_clone () const
        {
            return new poly1d<T, n> (*this);
        }

        const char *do_get_type_name () const
        {
            return "polynomial";
        }

      public:
        poly1d ()
        {
            assert (n >= 0);
            for (int i = 0; i <= n; ++i)
                {
                    std::ostringstream ostr;
                    ostr << "a" << i;
                    this->push_param_info (param_info<Tv> (ostr.str ().c_str (), 1));
                }
        }

      public:
        Tv do_eval (const Tv &x, const Tv &param)
        {
            //      return x*get_element(param,0)+get_element(param,1);
            Tv result (x.size ());
            for (int m = 0; m < result.size (); ++m)
                {
                    result[m] = 0;
                    for (int i = 0; i <= n; ++i)
                        {
                            typename Tv::value_type xn (1);
                            for (int j = 0; j < i; ++j)
                                {
                                    xn *= x[m];
                                }
                            result[m] += get_element (param, i) * xn;
                        }
                }
            return result;
        }

      private:
        std::string do_get_information () const
        {
#ifdef WITH_OPT_DOC
#include <model_doc/poly1d.info>
#endif
            return "";
        }
    };
}


#endif
// EOF
