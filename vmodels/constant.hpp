/**
   \file constant.hpp
   \brief constant model
   \author Junhua Gu
 */


#ifndef VCONSTANT_MODEL_H_
#define VCONSTANT_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <misc/optvec.hpp>
#include <cmath>

namespace opt_utilities
{
    template <typename T>
    class constant : public model<data<optvec<T>, optvec<T>>, optvec<T>, std::string>
    {
        typedef optvec<T> Tv;

      private:
        constant<T> *do_clone () const
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
            this->push_param_info (param_info<Tv> ("c", 1));
        }

      public:
        Tv do_eval (const Tv &x, const Tv &param)
        {
            // return x*param[0]+param[1];
            Tv result (x.size ());
            for (int i = 0; i < result.size (); ++i)
                {
                    result[i] = param[0];
                }
            return result;
        }

      private:
        std::string do_get_information () const
        {
#ifdef WITH_OPT_DOC
#include <model_doc/constant.info>
#endif
            return "";
        }
    };
}


#endif
// EOF
