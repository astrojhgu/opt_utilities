/**
   \file dbeta2d.hpp
   \brief 2d double beta model
   \author Junhua Gu
 */


#ifndef DBETA_MODEL2d_H_
#define DBETA_MODEL2d_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>
#include <cassert>
#include "vecn.hpp"

namespace opt_utilities
{

    template <typename T>
    class dbeta2d : public model<data<T, vecn<T, 2>>, std::vector<T>, std::string>
    {
      private:
        model<data<T, vecn<T, 2>>, std::vector<T>> *do_clone () const
        {
            return new dbeta2d<T> (*this);
        }

        const char *do_get_type_name () const
        {
            return "2d double beta model";
        }

      public:
        dbeta2d ()
        {
            push_param_info (param_info<std::vector<T>> ("S01", 1));
            push_param_info (param_info<std::vector<T>> ("rc11", 50));
            push_param_info (param_info<std::vector<T>> ("rc21", 50));
            push_param_info (param_info<std::vector<T>> ("rho1", 0));
            push_param_info (param_info<std::vector<T>> ("x01", 200));
            push_param_info (param_info<std::vector<T>> ("y01", 200));
            push_param_info (param_info<std::vector<T>> ("beta1", 2. / 3.));

            push_param_info (param_info<std::vector<T>> ("S02", 1));
            push_param_info (param_info<std::vector<T>> ("rc12", 60));
            push_param_info (param_info<std::vector<T>> ("rc22", 60));
            push_param_info (param_info<std::vector<T>> ("rho2", 0));
            push_param_info (param_info<std::vector<T>> ("x02", 200));
            push_param_info (param_info<std::vector<T>> ("y02", 200));
            push_param_info (param_info<std::vector<T>> ("beta2", 2. / 2.5));
            push_param_info (param_info<std::vector<T>> ("bkg", 0));
        }

        T do_eval (const vecn<T, 2> &xy, const std::vector<T> &param)
        {
            T S01 = get_element (param, 0);
            T rc11 = get_element (param, 1);
            T rc21 = get_element (param, 2);
            T rho1 = get_element (param, 3);
            T x01 = get_element (param, 4);
            T y01 = get_element (param, 5);
            T beta1 = get_element (param, 6);
            T S02 = get_element (param, 7);
            T rc12 = get_element (param, 8);
            T rc22 = get_element (param, 9);
            T rho2 = get_element (param, 10);
            T x02 = get_element (param, 11);
            T y02 = get_element (param, 12);
            T beta2 = get_element (param, 13);
            T bkg = get_element (param, 14);


            T x = xy[0];
            T y = xy[1];

            rho1 = rho1 > 1 ? 1 : rho1;
            rho1 = rho1 < -1 ? -1 : rho1;
            rho2 = rho2 > 1 ? 1 : rho2;
            rho2 = rho2 < -1 ? -1 : rho2;

            T r1 = (x - x01) * (x - x01) / (rc11 * rc11) + (y - y01) * (y - y01) / (rc21 * rc21) -
                   2 * rho1 * (x - x01) * (y - y01) / (rc11 * rc21);

            T r2 = (x - x02) * (x - x02) / (rc12 * rc12) + (y - y02) * (y - y02) / (rc22 * rc22) -
                   2 * rho2 * (x - x02) * (y - y02) / (rc12 * rc22);
            //      r1=r1<0?0:r1;
            // r2=r2<0?0:r2;
            assert (r1 >= 0);
            assert (r2 >= 0);


            return bkg + S01 * pow (1 + r1, -3 * beta1 + static_cast<T> (.5)) +
                   S02 * pow (1 + r2, -3 * beta2 + static_cast<T> (.5));
        }
    };
}


#endif
// EOF
