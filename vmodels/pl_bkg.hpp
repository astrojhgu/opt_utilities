/**
   \file pl_bkg.hpp
   \brief power law model
   \author Junhua Gu
 */


#ifndef POWER_LAW_BKG_MODEL_H_
#define POWER_LAW_BKG_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>
#include <misc/optvec.hpp>

namespace opt_utilities
{
    template <typename T>
    class pl_bkg : public model<data<optvec<T>, optvec<T>>, optvec<T>, std::string>
    {
      private:
        pl_bkg *do_clone () const
        {
            return new pl_bkg<T> (*this);
        }

        const char *do_get_type_name () const
        {
            return "power law with bkg";
        }

      public:
        pl_bkg ()
        {
            this->push_param_info (param_info<optvec<T>> ("Ampl", 1));
            this->push_param_info (param_info<optvec<T>> ("gamma", 1));
            this->push_param_info (param_info<optvec<T>> ("bkg", 1));
        }

        optvec<T> do_eval (const optvec<T> &x, const optvec<T> &param)
        {
            T A = get_element (param, 0);
            T gamma = get_element (param, 1);
            T bkg = get_element (param, 2);
            return A * pow (x, gamma) + bkg;
        }

      private:
        std::string do_get_information () const
        {
#ifdef WITH_OPT_DOC
#include <model_doc/pl_bkg.info>
#endif
            return "";
        }
    };
}


#endif
// EOF
