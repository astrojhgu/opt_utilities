#ifndef VT_T
#define VT_T
#include <core/fitter.hpp>
#include <misc/optvec.hpp>
#include <vector>
#include <string>
#include <cmath>

using namespace std;
using namespace opt_utilities;
template <typename T>
class vk_temperature : public model<data<optvec<T>, optvec<T>>, optvec<T>, std::string>
{
  private:
    vk_temperature<T> *do_clone () const
    {
        return new vk_temperature<T> (*this);
    }
    typedef optvec<T> Tv;

  public:
    vk_temperature ()
    {
        this->push_param_info (param_info<Tv> ("T0", 2, 0.1, 10));
        this->push_param_info (param_info<Tv> ("rcool", 1e22, 0.001, 10));
        this->push_param_info (param_info<Tv> ("acool", 1));
        this->push_param_info (param_info<Tv> ("Tmin", 1, 0.1, 10));
        this->push_param_info (param_info<Tv> ("rt", 1e22, 0.001, 10));
        this->push_param_info (param_info<Tv> ("a", 1, -10, 10));
        this->push_param_info (param_info<Tv> ("c", 1, -10, 10));
        this->push_param_info (param_info<Tv> ("b", 1, -10, 10));
    };
    Tv do_eval (const Tv &r, const Tv &param)
    {
        Tv result (r.size ());
        T T0 = param[0];
        T rcool = param[1];
        T acool = param[2];
        T Tmin = param[3];
        T rt = param[4];
        T a = param[5];
        T c = param[6];
        T b = param[7];
        for (size_t i = 0; i != r.size (); ++i)
            {
                T x = pow (r[i] / rcool, acool);
                T tcool = (x + Tmin / T0) / (x + 1);
                T t = pow (r[i] / rt, -a) / pow (pow (r[i] / rt, b) + T (1), c / b);
                result[i] = T0 * tcool * t;
            }
        return result;
    }

    std::string do_get_information () const
    {
#ifdef WITH_OPT_DOC
#include <model_doc/vt_temperature.info>
#endif
        return "";
    }
};

#endif
