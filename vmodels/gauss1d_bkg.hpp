/**
   \file gauss1d.hpp
   \brief Gauss model
   \author Junhua Gu
 */

#ifndef GAUSS_MODEL_BKG_H_
#define GAUSS_MODEL_BKG_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>
#include <misc/optvec.hpp>

namespace opt_utilities
{
    template <typename T>
    class gauss1d_bkg : public model<data<optvec<T>, optvec<T>>, optvec<T>, std::string>
    {
      private:
        gauss1d_bkg *do_clone () const
        {
            return new gauss1d_bkg<T> (*this);
        }

        const char *do_get_type_name () const
        {
            return "1d gaussian with bkg";
        }

      public:
        gauss1d_bkg ()
        {
            this->push_param_info (param_info<optvec<T>> ("N", 1));
            this->push_param_info (param_info<optvec<T>> ("x0", 0));
            this->push_param_info (param_info<optvec<T>> ("sigma", 1));
            this->push_param_info (param_info<optvec<T>> ("bkg", 0));
        }

      public:
        optvec<T> do_eval (const optvec<T> &x, const optvec<T> &param)
        {
            T N = get_element (param, 0);
            T x0 = get_element (param, 1);
            T sigma = get_element (param, 2);
            T bkg = get_element (param, 3);
            optvec<T> y = (x - x0) / sigma;
            return N * exp (-y * y / 2.) + bkg;
        }

      private:
        std::string do_get_information () const
        {
#ifdef WITH_OPT_DOC
#include <model_doc/gauss1d_bkg.info>
#endif
            return "";
        }
    };
}


#endif
// EOF
