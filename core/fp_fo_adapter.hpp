#ifndef FP_FO_ADAPTER_HPP
#define FP_FO_ADAPTER_HPP
#include "optimizer.hpp"

namespace opt_utilities
{
    template <typename rT, typename pT> class fp_fo_adapter : public func_obj<rT, pT>
    {
      private:
        fp_fo_adapter ();
        rT (*fp) (const pT &);

      public:
        fp_fo_adapter (rT (*_fp) (const pT &)) : fp (_fp)
        {
        }

        fp_fo_adapter<rT, pT> *do_clone () const
        {
            return new fp_fo_adapter<rT, pT> (*this);
        }

        rT do_eval (const pT &x)
        {
            return fp (x);
        }
    };
};


#endif
