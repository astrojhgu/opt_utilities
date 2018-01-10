/**
   \file bpl1d.hpp
   \brief broken powerlaw
   \author Junhua Gu
 */


#ifndef BROKEN_POWER_LAW_MODEL_H_
#define BROKEN_POWER_LAW_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>

namespace opt_utilities
{
    template <typename T> class bpl1d : public model<data<T, T>, std::vector<T>, std::string>
    {
      private:
        model<data<T, T>, std::vector<T>> *do_clone () const
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
            this->push_param_info (param_info<std::vector<T>> ("bpx", 1));
            this->push_param_info (param_info<std::vector<T>> ("bpy", 1));
            this->push_param_info (param_info<std::vector<T>> ("gamma1", 1));
            this->push_param_info (param_info<std::vector<T>> ("gamma2", 1));
        }

        T do_eval (const T &x, const std::vector<T> &param)
        {
            T x_b = get_element (param, 0);
            T f_b = get_element (param, 1);
            T gamma1 = get_element (param, 2);
            T gamma2 = get_element (param, 3);
            if (x < x_b)
                {
                    return f_b * pow (x, gamma1) / pow (x_b, gamma1);
                }
            else
                {
                    return f_b * pow (x, gamma2) / pow (x_b, gamma2);
                }
        }


      private:
        std::string do_get_information () const
        {
            return "";
            // return "<math xmlns=\"http://www.w3.org/1998/Math/MathML\" display=\"block\"
            // class=\"equation\"><mi>f</mi><mrow><mo
            // class=\"MathClass-open\">(</mo><mrow><mi>x</mi><mo
            // class=\"MathClass-punc\">;</mo><msub><mrow><mi>x</mi></mrow><mrow><mi>b</mi></mrow></msub><mo
            // class=\"MathClass-punc\">,</mo><msub><mrow><mi>y</mi></mrow><mrow><mi>b</mi></mrow></msub><mo
            // class=\"MathClass-punc\">,</mo><msub><mrow><mi>γ</mi></mrow><mrow><mn>1</mn></mrow></msub><mo
            // class=\"MathClass-punc\">,</mo><msub><mrow><mi>γ</mi></mrow><mrow><mn>2</mn></mrow></msub></mrow><mo
            // class=\"MathClass-close\">)</mo></mrow> <mo class=\"MathClass-rel\">=</mo> <mfenced
            // separators=\"\" open=\"{\" close=\"\"><mrow> <mtable style=\"\" equalrows=\"false\"
            // equalcolumns=\"false\" class=\"array\"><mtr><mtd class=\"array\"
            // columnalign=\"center\"><msub><mrow><mi>y</mi></mrow><mrow><mi>b</mi></mrow></msub>
            // <mfrac><mrow><msup><mrow><mi>x</mi></mrow><mrow><msub><mrow><mi>γ</mi></mrow><mrow><mn>1</mn></mrow></msub></mrow></msup></mrow><mrow><msubsup><mrow><mi>x</mi></mrow><mrow><mi>b</mi></mrow><mrow><msub><mrow><mi>γ</mi></mrow><mrow><mn>1</mn></mrow></msub></mrow></msubsup></mrow></mfrac></mtd><mtd
            // class=\"array\" columnalign=\"center\">   <mi>x</mi> <mo
            // class=\"MathClass-rel\">&lt;</mo>
            // <msub><mrow><mi>x</mi></mrow><mrow><mi>b</mi></mrow></msub></mtd></mtr><mtr><mtd
            // class=\"array\" columnalign=\"center\">
            // <msub><mrow><mi>y</mi></mrow><mrow><mi>b</mi></mrow></msub>
            // <mfrac><mrow><msup><mrow><mi>x</mi></mrow><mrow><msub><mrow><mi>γ</mi></mrow><mrow><mn>2</mn></mrow></msub></mrow></msup></mrow><mrow><msubsup><mrow><mi>x</mi></mrow><mrow><mi>b</mi></mrow><mrow><msub><mrow><mi>γ</mi></mrow><mrow><mn>2</mn></mrow></msub></mrow></msubsup></mrow></mfrac>
            // </mtd><mtd class=\"array\"
            // columnalign=\"center\"><mi>o</mi><mi>t</mi><mi>h</mi><mi>e</mi><mi>r</mi><mi>w</mi><mi>i</mi><mi>s</mi><mi>e</mi></mtd></mtr><mtr><mtd
            // class=\"array\" columnalign=\"center\">        </mtd></mtr>
            // <!--cc--></mtable></mrow></mfenced></math>";
        }
    };
}


#endif
// EOF
