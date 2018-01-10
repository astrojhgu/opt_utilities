/**
   \file lin1d.hpp
   \brief 1d linear model
   \author Junhua Gu
 */


#ifndef ZERO2ONE_MODEL_H_
#define ZERO2ONE_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <core/pre_estimater.hpp>
#include <cmath>

namespace opt_utilities
{
    template <typename T>
    class zero2one : public model<data<T, T>, std::vector<T>, std::string>,
                     public pre_estimatable<data<T, T>, std::vector<T>, std::string>
    {
      private:
        model<data<T, T>, std::vector<T>> *do_clone () const
        {
            return new zero2one<T> (*this);
        }

        const char *do_get_type_name () const
        {
            return "1d linear model";
        }

      public:
        zero2one ()
        {
            this->push_param_info (param_info<std::vector<T>> ("x0", 0));
            this->push_param_info (param_info<std::vector<T>> ("w", 1));
        }

      public:
        T do_eval (const T &x, const std::vector<T> &param)
        {
            T x0 = get_element (param, 0);
            T w = get_element (param, 1);
            static T half_pi = std::atan ((T)1) * 2;
            return (std::atan ((x - x0) / w) / half_pi + 1) / 2;
        }

      private:
        std::string do_get_information () const
        {
            return "<math><mrow> <mtext>f(x;k,b)=k x+b</mtext>		\
    </mrow>								\
</math>";
        }
    };
}


#endif
// EOF
