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
#include <misc/optvec.hpp>

namespace opt_utilities
{
  template <typename T>
  class bremss
    :public model<data<optvec<T>,optvec<T> >,optvec<T>,std::string>
  {
  private:
    bremss* do_clone()const
    {
      return new bremss<T>(*this);
    }

    const char* do_get_type_name()const
    {
      return "Bremsstrahlung emission";
    }
  public:
    bremss()
    {
      this->push_param_info(param_info<optvec<T> >("norm",1));
      this->push_param_info(param_info<optvec<T> >("kT",1));
    }

    optvec<T> do_eval(const optvec<T>& x,const optvec<T>& param)
    {
      T norm=get_element(param,0);
      T kT=get_element(param,1);
      
      return norm*sqrt(kT)*exp(-x/kT);
    }

  private:
    std::string do_get_information()const
    {
#ifdef WITH_OPT_DOC
#include <model_doc/bremss.info>
#endif
      return "";
    }
  };
}



#endif
//EOF
