/**
   \file dbeta1d.hpp
   \brief double beta model
   \author Junhua Gu
 */


#ifndef DBETA_MODEL_H_
#define DBETA_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>
#include <iostream>
namespace opt_utilities
{
    template <typename T> class dbeta1d : public model<data<T, T>, std::vector<T>, std::string>
    {
      private:
        model<data<T, T>, std::vector<T>> *do_clone () const
        {
            return new dbeta1d<T> (*this);
        }

        const char *do_get_type_name () const
        {
            return "1d double beta model";
        }

      public:
        dbeta1d ()
        {
            this->push_param_info (param_info<std::vector<T>> ("S01", 1));
            this->push_param_info (param_info<std::vector<T>> ("rc1", 30));
            this->push_param_info (param_info<std::vector<T>> ("beta1", .6));

            this->push_param_info (param_info<std::vector<T>> ("S02", .5));
            this->push_param_info (param_info<std::vector<T>> ("rc2", 20));
            this->push_param_info (param_info<std::vector<T>> ("beta2", .4));


            this->push_param_info (param_info<std::vector<T>> ("bkg", 0));
        }


        T do_eval (const T &x, const std::vector<T> &param)
        {
            T S01 = get_element (param, 0);
            T r_c1 = get_element (param, 1);
            T beta1 = get_element (param, 2);

            T S02 = get_element (param, 3);
            T r_c2 = get_element (param, 4);
            T beta2 = get_element (param, 5);


            T bkg = get_element (param, 6);

            return bkg + S01 * pow (1 + (x * x) / (r_c1 * r_c1), -3 * beta1 + static_cast<T> (.5)) +
                   S02 * pow (1 + (x * x) / (r_c2 * r_c2), -3 * beta2 + static_cast<T> (.5));
        }

      private:
        std::string do_get_information () const
        {
            return "double 1d beta model\n"
                   "S=beta(S01,beta1,rc1)+beta(S02,beta2,rc2)\n";
        }
    };
}


#endif
// EOF
