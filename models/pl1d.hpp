/**
   \file pl1d.hpp
   \brief 1d power law
   \author Junhua Gu
 */


#ifndef POWER_LAW_MODEL_H_
#define POWER_LAW_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>

namespace opt_utilities
{
    template <typename T> class pl1d : public model<data<T, T>, std::vector<T>, std::string>
    {
      private:
        model<data<T, T>, std::vector<T>> *do_clone () const
        {
            return new pl1d<T> (*this);
        }

        const char *do_get_type_name () const
        {
            return "1d power law";
        }

      public:
        pl1d ()
        {
            this->push_param_info (param_info<std::vector<T>> ("Ampl", 1));
            this->push_param_info (param_info<std::vector<T>> ("gamma", 1));
        }

        T do_eval (const T &x, const std::vector<T> &param)
        {
            T A = get_element (param, 0);
            T gamma = get_element (param, 1);
            return A * pow (x, gamma);
        }

      private:
        std::string do_get_information () const
        {
            return "Simple power law model\n"
                   "y=A*x^gamma\n";
        }
    };
}


#endif
// EOF
