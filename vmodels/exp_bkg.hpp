/**
   \file exp_bkg.hpp
   \brief power law model
   \author Junhua Gu
 */


#ifndef EXP_BKG_MODEL_H_
#define EXP_BKG_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>
#include <misc/optvec.hpp>

namespace opt_utilities
{
    template <typename T>
    class exp_bkg : public model<data<optvec<T>, optvec<T>>, optvec<T>, std::string>
    {
      private:
        exp_bkg *do_clone () const
        {
            return new exp_bkg<T> (*this);
        }

        const char *do_get_type_name () const
        {
            return "exp with bkg";
        }

      public:
        exp_bkg ()
        {
            this->push_param_info (param_info<optvec<T>> ("Ampl", 1));
            this->push_param_info (param_info<optvec<T>> ("scale", 1));
            this->push_param_info (param_info<optvec<T>> ("bkg", 1));
        }

        optvec<T> do_eval (const optvec<T> &x, const optvec<T> &param)
        {
            T A = get_element (param, 0);
            T scale = get_element (param, 1);
            T bkg = get_element (param, 2);
            return A * exp (-x / abs (scale)) + bkg;
        }

      private:
        std::string do_get_information () const
        {
#ifdef WITH_OPT_DOC
#include <model_doc/exp_bkg.info>
#endif
            return "";
        }
    };
}


#endif
// EOF
