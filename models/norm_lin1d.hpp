/**
   \file guss1d.hpp
   \brief norm_gauss model
   \author Junhua Gu
 */


#ifndef NORM_LIN1D_MODEL_H_
#define NORM_LIN1D_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>

namespace opt_utilities
{
    template <typename T> class norm_lin1d : public model<data<T, T>, std::vector<T>, std::string>
    {
      private:
        T lower_limit, upper_limit;

      private:
        model<data<T, T>, std::vector<T>> *do_clone () const
        {
            return new norm_lin1d<T> (*this);
        }

        const char *do_get_type_name () const
        {
            return "1d norm_lin1d";
        }

      public:
        norm_lin1d () : lower_limit (1), upper_limit (2)
        {
            this->push_param_info (param_info<std::vector<T>> ("k", 0));
        }

        void set_limits (const T &a, const T &b)
        {
            lower_limit = a;
            upper_limit = b;
        }

      public:
        T do_eval (const T &x, const std::vector<T> &param)
        {
            T k = get_element (param, 0);

            T a = lower_limit;
            T b = upper_limit;

            T intercept = (k * (b * b - a * a) - 2) / (2 * (a - b));

            return k * x + intercept;
        }

      private:
        std::string do_get_information () const
        {
            return "";
        }
    };
}


#endif
// EOF
