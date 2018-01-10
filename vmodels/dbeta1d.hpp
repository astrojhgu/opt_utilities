/**
   \file dbeta1d.hpp
   \brief 1d beta model
   \author Junhua Gu
 */


#ifndef V2BETA_MODEL_H_
#define V2BETA_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <misc/optvec.hpp>
#include <cmath>

namespace opt_utilities
{
    template <typename T>
    class dbeta1d : public model<data<optvec<T>, optvec<T>>, optvec<T>, std::string>
    {
        typedef optvec<T> Tv;

      private:
        dbeta1d<T> *do_clone () const
        {
            return new dbeta1d<T> (*this);
        }

        const char *do_get_type_name () const
        {
            return "1d surface brightness 2beta model";
        }

      public:
        dbeta1d ()
        {
            this->push_param_info (param_info<Tv> ("S01", 1));
            this->push_param_info (param_info<Tv> ("rc1", 10));
            this->push_param_info (param_info<Tv> ("beta1", 2. / 3.));

            this->push_param_info (param_info<Tv> ("S02", 1));
            this->push_param_info (param_info<Tv> ("rc2", 10));
            this->push_param_info (param_info<Tv> ("beta2", 2. / 3.));

            this->push_param_info (param_info<Tv> ("bkg", 0));
        }

      public:
        Tv do_eval (const Tv &x, const Tv &param)
        {
            Tv result (x.size ());

            T S01 = get_element (param, 0);
            T r_c1 = get_element (param, 1);
            T beta1 = get_element (param, 2);

            T S02 = get_element (param, 3);
            T r_c2 = get_element (param, 4);
            T beta2 = get_element (param, 5);

            T bkg = get_element (param, 6);

            // return x*get_element(param,0)+get_element(param,1);
            for (size_t i = 0; i != x.size (); ++i)
                {

                    result[i] =
                    bkg + S01 * pow (1 + (x[i] * x[i]) / (r_c1 * r_c1), -3 * beta1 + static_cast<T> (.5)) +
                    S02 * pow (1 + (x[i] * x[i]) / (r_c2 * r_c2), -3 * beta2 + static_cast<T> (.5));
                }
            return result;
        }

      private:
        std::string do_get_information () const
        {
#ifdef WITH_OPT_DOC
#include <model_doc/dbeta1d.info>
#endif
            return "";
        }
    };
}


#endif
// EOF
