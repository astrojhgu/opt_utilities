#ifndef OPTIMIZE_FUNC_HPP
#define OPTIMIZE_FUNC_HPP

#if __cplusplus < 201103L
#error This header must be used with C++ 11(0x) or newer
#endif

#include <vector>
#include <functional>
#include <cmath>
#include <utility>
#include "../core/optimizer.hpp"
#include "../methods/powell/powell_method.hpp"

namespace opt_utilities
{

    template <typename Ty, typename Tx>
    Tx optimize (const std::function<Ty (Tx)> &func,
                 const Tx &start_point,
                 const opt_utilities::opt_method<Ty, Tx> &opm =
                 opt_utilities::powell_method<double, std::vector<double>> ())
    {
        class func_wrapper : public opt_utilities::func_obj<Ty, Tx>
        {
            std::function<Ty (Tx)> f;

          public:
            func_wrapper (const std::function<Ty (Tx)> &f1) : f (f1){};

            func_wrapper *do_clone () const
            {
                return const_cast<func_wrapper *> (this);
            }

            void do_destroy ()
            {
                // do nothing
            }

            Ty do_eval (const Tx &x)
            {
                return f (x);
            }
        } foo (func);
        opt_utilities::optimizer<Ty, Tx> opt;
        opt.set_opt_method (opm);
        opt.set_func_obj (foo);
        opt.set_start_point (start_point);
        return opt.optimize ();
    }
}

#endif
// EOF
