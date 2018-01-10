/**
   \file func_model.hpp
   \brief model wrapper of raw function
   \author Junhua Gu
 */


#ifndef FUNC_MODEL_H_
#define FUNC_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
namespace opt_utilities
{
    template <typename T> class func_model : public model<data<T, T>, std::vector<T>, std::string>
    {
      private:
        T (*func) (T x, const T *const &p);
        int nparams;

      private:
        model<T, T, std::vector<T>> *do_clone () const
        {
            return new func_model<T> (*this);
        }

        const char *do_get_type_name () const
        {
            return "function wrapping model";
        }

        //  public:
      private:
        func_model ()
        {
        }


      public:
        func_model (T (*_func) (T x, const T *const &p), int n) : func (_func), nparams (n)
        {
            for (int i = 0; i != n; ++i)
                {
                    std::ostringstream oss;
                    oss << i;
                    this->push_param_info (param_info<std::vector<T>> (oss.str (), 0));
                }
        }


        T do_eval (const T &x, const std::vector<T> &param)
        {
            return func (x, &get_element (param, 0));
        }

      private:
        std::string do_get_information () const
        {
            return "Wrapper for necked C function\n";
        }
    };
}


#endif
// EOF
