/**
   \file uniformed.hpp
   \brief uniformed distribution
   \author Junhua Gu
 */


#ifndef UNIFORMED_MODEL_H_
#define UNIFORMED_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>
#include <misc/optvec.hpp>

namespace opt_utilities
{
    template <typename T>
    class uniformed : public model<data<optvec<T>, optvec<T>>, optvec<T>, std::string>
    {
      private:
        uniformed *do_clone () const
        {
            return new uniformed<T> (*this);
        }

        const char *do_get_type_name () const
        {
            return "1d normed gaussian";
        }

      public:
        uniformed ()
        {
            this->push_param_info (param_info<optvec<T>> ("a", 0.));
            this->push_param_info (param_info<optvec<T>> ("b", 1.));
        }


      public:
        optvec<T> do_eval (const optvec<T> &x, const optvec<T> &param)
        {
            T a = get_element (param, 0);
            T b = get_element (param, 1);
            optvec<T> y;
            resize (y, get_size (x));
            for (int i = 0; i < get_size (x); ++i)
                {
                    T v = get_element (x, i);
                    set_element (y, i, (v - a) * (v - b) < 0 ? 1 / abs (b - a) : 0);
                }
            return y;
        }

      private:
        std::string do_get_information () const
        {
#ifdef WITH_OPT_DOC
#include <model_doc/uniformed.info>
#endif
            return "";
        }
    };
}


#endif
// EOF
