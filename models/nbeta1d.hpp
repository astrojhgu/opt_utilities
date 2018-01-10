/**
   \file nbeta1d
   \brief 1d density beta model
   \author Junhua Gu
 */


#ifndef NBETA_MODEL_H_
#define NBETA_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>
#include <iostream>
namespace opt_utilities
{
    template <typename T> class nbeta1d : public model<data<T, T>, std::vector<T>, std::string>
    {
      private:
        model<data<T, T>, std::vector<T>> *do_clone () const
        {
            return new nbeta1d<T> (*this);
        }

        const char *do_get_type_name () const
        {
            return "1d density beta model";
        }

      public:
        nbeta1d ()
        {
            this->push_param_info (param_info<std::vector<T>> ("n0", 1));
            this->push_param_info (param_info<std::vector<T>> ("rc", 10));
            this->push_param_info (param_info<std::vector<T>> ("beta", 2. / 3.));
            this->push_param_info (param_info<std::vector<T>> ("bkg", 0));
        }


        T do_eval (const T &x, const std::vector<T> &param)
        {
            T n0 = get_element (param, 0);
            T r_c = get_element (param, 1);
            T beta = get_element (param, 2);
            T bkg = get_element (param, 3);

            return bkg + n0 * pow (1 + (x * x) / (r_c * r_c), -3. / 2. * beta);
        }

      private:
        std::string do_get_information () const
        {
            return "<math xmlns=\"http://www.w3.org/1998/Math/MathML\" display=\"block\" "
                   "class=\"equation\"><mi>f</mi><mrow><mo "
                   "class=\"MathClass-open\">(</mo><mrow><mi>x</mi><mo "
                   "class=\"MathClass-punc\">;</mo><msub><mrow><mi>n</mi></mrow><mrow><mn>0</mn></"
                   "mrow></msub><mo class=\"MathClass-punc\">,</mo><mi>β</mi><mo "
                   "class=\"MathClass-punc\">,</mo><msub><mrow><mi>r</mi></mrow><mrow><mi>c</mi></"
                   "mrow></msub><mo "
                   "class=\"MathClass-punc\">,</mo><mi>b</mi><mi>k</mi><mi>g</mi></mrow><mo "
                   "class=\"MathClass-close\">)</mo></mrow> <mo class=\"MathClass-rel\">=</mo> "
                   "<msub><mrow><mi>n</mi></mrow><mrow><mn>0</mn></mrow></msub><msup><mrow> "
                   "<mfenced separators=\"\" open=\"[\" close=\"]\"><mrow><mn>1</mn> <mo "
                   "class=\"MathClass-bin\">+</mo><msup><mrow> <mfenced separators=\"\" open=\"(\" "
                   "close=\")\"><mrow> "
                   "<mfrac><mrow><mi>r</mi></mrow><mrow><msub><mrow><mi>r</mi></mrow><mrow><mi>c</"
                   "mi></mrow></msub></mrow></mfrac></mrow></mfenced> "
                   "</mrow><mrow><mn>2</mn></mrow></msup></mrow></mfenced> </mrow><mrow><mo "
                   "class=\"MathClass-bin\">−</mo><mfrac><mrow><mn>3</mn></mrow><mrow><mn>2</mn></"
                   "mrow></mfrac><mi>β</mi><mo class=\"MathClass-bin\">+</mo><mn>1</mn><mo "
                   "class=\"MathClass-bin\">∕</mo><mn>2</mn></mrow></msup> <mo "
                   "class=\"MathClass-bin\">+</mo> <mi>b</mi><mi>k</mi><mi>g</mi></math>";
        }
    };
}


#endif
// EOF
