#ifndef OPTIMIZE_HPP
#define OPTIMIZE_HPP

#define OPT_HEADER

#include <cstddef>
#include "opt_traits.hpp"
#include "opt_exception.hpp"
#include <cstdlib>
#include <functional>
#include <typeinfo>

namespace opt_utilities
{
  template <typename rT,typename pT,typename funcT>
  class fopt_method
  {
  public:
    virtual ~fopt_method()
    {}

  public:
    pT optimize(const funcT& func)const
    {
      return do_optimize(func);
    }

  private:
    virtual pT do_optimize(const funcT&)const=0;
  };

  
  template <typename rT,typename pT,typename funcT>
  pT optimize(funcT&& func,const fopt_method<rT,pT,funcT>& om)
  {
    return om.optimize(func);
  }
}

#endif
