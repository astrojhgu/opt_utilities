/**
   \file bpl.hpp
   \brief broken power law
   \author Junhua Gu
 */


#ifndef BROKEN_POWER_LAW_MODEL_H_
#define BROKEN_POWER_LAW_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>
#include <misc/optvec.hpp>

namespace opt_utilities
{
    template <typename T>
    class bpl1d : public model<data<optvec<T>, optvec<T>>, optvec<T>, std::string>
    {
      private:
        bpl1d *do_clone () const
        {
            return new bpl1d<T> (*this);
        }

        const char *do_get_type_name () const
        {
            return "broken power law";
        }

      public:
        bpl1d ()
        {
            this->push_param_info (param_info<optvec<T>> ("bpx", 1));
            this->push_param_info (param_info<optvec<T>> ("bpy", 1));
            this->push_param_info (param_info<optvec<T>> ("gamma1", 1));
            this->push_param_info (param_info<optvec<T>> ("gamma2", 1));
        }

        optvec<T> do_eval (const optvec<T> &x, const optvec<T> &param)
        {
            T x_b = get_element (param, 0);
            T f_b = get_element (param, 1);
            T gamma1 = get_element (param, 2);
            T gamma2 = get_element (param, 3);

            optvec<T> result (x.size ());
            for (int i = 0; i < result.size (); ++i)
                {
                    if (x[i] < x_b)
                        {
                            result[i] = f_b * pow (x[i], gamma1) / pow (x_b, gamma1);
                        }
                    else
                        {
                            result[i] = f_b * pow (x[i], gamma2) / pow (x_b, gamma2);
                        }
                }
            return result;
        }


      private:
        std::string do_get_information () const
        {
#ifdef WITH_OPT_DOC
#include <model_doc/bpl.info>
#endif
            return "";
        }
    };
}


#endif
// EOF
