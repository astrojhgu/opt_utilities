/**
   \file guss.hpp
   \brief dpar model
   \author Junhua Gu
 */


#ifndef DPAR_MODEL_H_
#define DPAR_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>

namespace opt_utilities
{
    template <typename T> class dpar : public model<data<T, T>, std::vector<T>, std::string>
    {
      private:
        T c;

        model<data<T, T>, std::vector<T>> *do_clone () const
        {
            return new dpar<T> (*this);
        }

        const char *do_get_type_name () const
        {
            return " dparian";
        }

      public:
        dpar (const T &_c) : c (_c)
        {
            this->push_param_info (param_info<std::vector<T>> ("alpha", 1));
        }

      public:
        T do_eval (const T &x, const std::vector<T> &param)
        {
            T alpha = param[0];
            if (x < c)
                {
                    return 0;
                }
            return alpha * std::pow (c, alpha) * std::pow (x, -alpha - 1);
        }


      private:
        std::string do_get_information () const
        {
            return "<math xmlns=\"http://www.w3.org/1998/Math/MathML\" display=\"block\" "
                   "class=\"equation\"><mi>f</mi><mrow><mo "
                   "class=\"MathClass-open\">(</mo><mrow><mi>x</mi><mo "
                   "class=\"MathClass-punc\">;</mo><mi>N</mi><mo "
                   "class=\"MathClass-punc\">,</mo><msub><mrow><mi>x</mi></mrow><mrow><mn>0</mn></"
                   "mrow></msub><mo class=\"MathClass-punc\">,</mo><mi>σ</mi></mrow><mo "
                   "class=\"MathClass-close\">)</mo></mrow> <mo class=\"MathClass-rel\">=</mo> "
                   "<mi>N</mi><msup><mrow><mi>e</mi></mrow><mrow><mo "
                   "class=\"MathClass-bin\">−</mo><mfrac><mrow><msup><mrow><mrow><mo "
                   "class=\"MathClass-open\">(</mo><mrow><mi>x</mi><mo "
                   "class=\"MathClass-bin\">−</mo><msub><mrow><mi>x</mi></mrow><mrow><mn>0</mn></"
                   "mrow></msub></mrow><mo "
                   "class=\"MathClass-close\">)</mo></mrow></mrow><mrow><mn>2</mn></mrow></msup></"
                   "mrow><mrow><mn>2</mn><msup><mrow><mi>σ</mi></mrow><mrow><mn>2</mn></mrow></"
                   "msup></mrow></mfrac>    </mrow></msup></math>";
        }
    };
}


#endif
// EOF
