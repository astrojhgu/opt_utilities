/**
   \file beta1d.hpp
   \brief 1d beta model
   \author Junhua Gu
 */


#ifndef VBETA_MODEL_H_
#define VBETA_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <misc/optvec.hpp>
#include <cmath>

namespace opt_utilities
{
    template <typename T>
    class beta1d : public model<data<optvec<T>, optvec<T>>, optvec<T>, std::string>
    {
        typedef optvec<T> Tv;

      private:
        beta1d<T> *do_clone () const
        {
            return new beta1d<T> (*this);
        }

        const char *do_get_type_name () const
        {
            return "1d surface brightness beta model";
        }

      public:
        beta1d ()
        {
            this->push_param_info (param_info<Tv> ("S0", 1));
            this->push_param_info (param_info<Tv> ("rc", 10));
            this->push_param_info (param_info<Tv> ("beta", 2. / 3.));
            this->push_param_info (param_info<Tv> ("bkg", 0));
        }

      public:
        Tv do_eval (const Tv &x, const Tv &param)
        {
            Tv result (x.size ());
            T S0 = get_element (param, 0);
            T r_c = get_element (param, 1);
            T beta = get_element (param, 2);
            T bkg = get_element (param, 3);

            // return x*get_element(param,0)+get_element(param,1);
            for (size_t i = 0; i != x.size (); ++i)
                {

                    result[i] =
                    bkg + S0 * pow (1 + (x[i] * x[i]) / (r_c * r_c), -3 * beta + static_cast<T> (.5));
                }
            return result;
        }

      private:
        std::string do_get_information () const
        {
#ifdef WITH_OPT_DOC
#include <model_doc/beta1d.info>
#endif
            return "";
        }
    };
}


#endif
// EOF
