/**
   \file nfw1d.hpp
   \brief 1d nfw model
   \author Junhua Gu
 */


#ifndef NFW_MODEL_H_
#define NFW_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>
#include <iostream>
namespace opt_utilities
{
    template <typename T> class nfw1d : public model<data<T, T>, std::vector<T>, std::string>
    {
      private:
        model<data<T, T>, std::vector<T>> *do_clone () const
        {
            return new nfw1d<T> (*this);
        }

        const char *do_get_type_name () const
        {
            return "1d NFW model";
        }

      public:
        nfw1d ()
        {
            this->push_param_info (param_info<std::vector<T>> ("rho0", 1.));
            this->push_param_info (param_info<std::vector<T>> ("rs", 1.));
        }


        T do_eval (const T &x, const std::vector<T> &param)
        {
            T rho0 = get_element (param, 0);
            T rs = get_element (param, 1);


            return rho0 / (x / rs * (1 + x / rs) * (1 + x / rs));
        }

        std::string do_get_information () const
        {
            return "NFW model\n"
                   "y=rho0/(r/rs*(1+r/rs)^2\n";
        }
    };
}


#endif
// EOF
