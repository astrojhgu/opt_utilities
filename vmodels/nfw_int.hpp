/**
   \file nfw_int.hpp
   \brief NFW_INT model
   \author Junhua Gu
 */


#ifndef NFW_INT_MODEL_H_
#define NFW_INT_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>
#include <misc/optvec.hpp>

namespace opt_utilities
{
  template <typename T>
  class nfw_int
    :public model<data<optvec<T>,optvec<T> >,optvec<T>,std::string>
  {
  private:
    nfw_int* do_clone()const
    {
      return new nfw_int<T>(*this);
    }

    const char* do_get_type_name()const
    {
      return "nfw_int mass profile";
    }
  public:
    nfw_int()
    {
      this->push_param_info(param_info<optvec<T> >("rho0",1));
      this->push_param_info(param_info<optvec<T> >("rs",1));
    }

    optvec<T> do_eval(const optvec<T>& x,const optvec<T>& param)
    {
      T rho0=get_element(param,0);
      T rs=get_element(param,1);
      return 4*3.1415926*rho0*rs*rs*rs*(std::log((x+rs)/rs)-x/(x+rs));
    }

  private:
    std::string do_get_information()const
    {
#ifdef WITH_OPT_DOC
#include <model_doc/nfw_int.info>
#endif
      return "";
    }
  };
}



#endif
//EOF
