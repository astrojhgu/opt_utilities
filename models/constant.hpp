/**
   \file constant.hpp
   \brief constant model
   \author Junhua Gu
 */


#ifndef CONSTANT_MODEL_H_
#define CONSTANT_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>

namespace opt_utilities
{
    template <typename T> class constant : public model<data<T, T>, std::vector<T>, std::string>
    {
      private:
        model<data<T, T>, std::vector<T>> *do_clone () const
        {
            return new constant<T> (*this);
        }
        const char *do_get_type_name () const
        {
            return "constant";
        }

      public:
        constant ()
        {
            this->push_param_info (param_info<std::vector<T>> ("c", 1));
        }

      public:
        T do_eval (const T &x, const std::vector<T> &param)
        {
            // return x*param[0]+param[1];
            return get_element (param, 0);
        }

      private:
        std::string do_get_information () const
        {
            return "Constant\n"
                   "y=C\n";
        }
    };
}


#endif
// EOF
