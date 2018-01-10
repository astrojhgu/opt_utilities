/**
   \file lin1d.hpp
   \brief 1d linear model
   \author Junhua Gu
 */


#ifndef LINEAR_MODEL_H_
#define LINEAR_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <core/pre_estimater.hpp>
#include <cmath>

namespace opt_utilities
{
    template <typename T>
    class lin1d : public model<data<T, T>, std::vector<T>, std::string>,
                  public pre_estimatable<data<T, T>, std::vector<T>, std::string>
    {
      private:
        model<data<T, T>, std::vector<T>> *do_clone () const
        {
            return new lin1d<T> (*this);
        }

        const char *do_get_type_name () const
        {
            return "1d linear model";
        }

      public:
        lin1d ()
        {
            this->push_param_info (param_info<std::vector<T>> ("k", 1));
            this->push_param_info (param_info<std::vector<T>> ("b", 0));
        }

      public:
        T do_eval (const T &x, const std::vector<T> &param)
        {
            return x * get_element (param, 0) + get_element (param, 1);
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
