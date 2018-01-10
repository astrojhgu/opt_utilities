/**
   \file bremss.hpp
   \brief bremss emission
   \author Junhua Gu
 */


#ifndef BREMSS_MODEL_H_
#define BREMSS_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>

namespace opt_utilities
{
    template <typename T> class bremss : public model<data<T, T>, std::vector<T>, std::string>
    {
      private:
        model<data<T, T>, std::vector<T>> *do_clone () const
        {
            return new bremss<T> (*this);
        }

        const char *do_get_type_name () const
        {
            return "Bremsstrahlung emission";
        }

      public:
        bremss ()
        {
            this->push_param_info (param_info<std::vector<T>> ("norm", 1));
            this->push_param_info (param_info<std::vector<T>> ("kT", 1));
        }

        T do_eval (const T &x, const std::vector<T> &param)
        {
            T norm = get_element (param, 0);
            T kT = get_element (param, 1);

            return norm * sqrt (kT) * exp (-x / kT);
        }

      private:
        std::string do_get_information () const
        {
            return "<math xmlns=\"http://www.w3.org/1998/Math/MathML\" display=\"block\" "
                   "class=\"equation\"><mi>f</mi><mrow><mo "
                   "class=\"MathClass-open\">(</mo><mrow><mi>x</mi><mo "
                   "class=\"MathClass-punc\">;</mo><mi>n</mi><mi>o</mi><mi>r</mi><mi>m</mi><mo "
                   "class=\"MathClass-punc\">,</mo><mi>k</mi><mi>T</mi></mrow><mo "
                   "class=\"MathClass-close\">)</mo></mrow> <mo class=\"MathClass-rel\">=</mo> "
                   "<mi>n</mi><mi>o</mi><mi>r</mi><mi>m</mi><msqrt><mrow><mi>k</mi><mi>T</mi></"
                   "mrow></msqrt><msup><mrow><mi>e</mi></mrow><mrow><mo "
                   "class=\"MathClass-bin\">−</mo><mi>x</mi><mo "
                   "class=\"MathClass-bin\">∕</mo><mi>k</mi><mi>T</mi> </mrow></msup></math>";
        }
    };
}


#endif
// EOF
