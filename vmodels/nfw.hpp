/**
   \file nfw.hpp
   \brief NFW model
   \author Junhua Gu
 */


#ifndef NFW_MODEL_H_
#define NFW_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>
#include <misc/optvec.hpp>

namespace opt_utilities
{
  template <typename T>
  class nfw
    :public model<data<optvec<T>,optvec<T> >,optvec<T>,std::string>
  {
  private:
    nfw* do_clone()const
    {
      return new nfw<T>(*this);
    }

    const char* do_get_type_name()const
    {
      return "nfw mass profile";
    }
  public:
    nfw()
    {
      this->push_param_info(param_info<optvec<T> >("rho0",1));
      this->push_param_info(param_info<optvec<T> >("rs",1));
    }

    optvec<T> do_eval(const optvec<T>& x,const optvec<T>& param)
    {
      T rho0=get_element(param,0);
      T rs=get_element(param,1);
      return rho0/(x/rs*(x/rs+T(1))*(x/rs+T(1)));
    }

  private:
    std::string do_get_information()const
    {
#ifdef WITH_OPT_DOC
#include <model_doc/nfw.info>
#endif
      return "";
    }
  };
}



#endif
//EOF
