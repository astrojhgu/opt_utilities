/**
   \file bivariate_normal.hpp
   \brief bivariate normal distribution
   \author Junhua Gu
   http://mathworld.wolfram.com/BivariateNormalDistribution.html
 */


#ifndef BVN_MODEL_H_
#define BVN_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>
#include <vector>
#include <limits>

namespace opt_utilities
{
    template <typename T>
    class bivariate_normal : public model<data<T, std::vector<T>>, std::vector<T>, std::string>
    {
      private:
        bivariate_normal *do_clone () const
        {
            return new bivariate_normal<T> (*this);
        }

        const char *do_get_type_name () const
        {
            return "1d normed gaussian";
        }

      public:
        bivariate_normal ()
        {
            this->push_param_info (param_info<std::vector<T>> ("mu1", 0, -1e99, 1e99));
            this->push_param_info (param_info<std::vector<T>> ("mu2", 0, -1e99, 1e99));
            this->push_param_info (param_info<std::vector<T>> ("sigma11", 1, 0, 1e99));
            this->push_param_info (param_info<std::vector<T>> ("sigma22", 1, 0, 1e99));
            this->push_param_info (param_info<std::vector<T>> ("sigma12", 0, -1e99, 1e99));
        }


      public:
        T do_eval (const std::vector<T> &x, const std::vector<T> &param)
        {
            const double pi = 3.14159265358979323846;
            T mu1 = get_element (param, 0);
            T mu2 = get_element (param, 1);
            T sigma11 = get_element (param, 2);
            T sigma22 = get_element (param, 3);
            T sigma12 = get_element (param, 4);
            T sigma21 = sigma12;

            T sigma1_sq = sigma11 * sigma11 + sigma12 * sigma12;
            T sigma2_sq = sigma21 * sigma21 + sigma22 * sigma22;

            T sigma1 = std::sqrt (sigma1_sq);
            T sigma2 = std::sqrt (sigma2_sq);

            T rho = (sigma11 * sigma21 + sigma12 * sigma22) / (sigma1 * sigma2);

            T x1 = x[0];
            T x2 = x[1];
            T z = (x1 - mu1) * (x1 - mu1) / sigma1_sq + (x2 - mu2) * (x2 - mu2) / sigma2_sq -
                  2 * rho * (x1 - mu1) * (x2 - mu2) / (sigma1 * sigma2);
            return std::exp (-z / (2 * (1 - rho * rho))) /
                   (2 * pi * sigma1 * sigma2 * std::sqrt (1 - rho * rho));
        }

      private:
        std::string do_get_information () const
        {
#ifdef WITH_OPT_DOC
#include <model_doc/bivariate_normal.info>
#endif
            return "";
        }
    };
}


#endif
// EOF
