/**
   \file powerlaw.hpp
   \brief power law model
   \author Junhua Gu
 */


#ifndef POWER_LAW_MODEL_H_
#define POWER_LAW_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>
#include <misc/optvec.hpp>

namespace opt_utilities
{
  template <typename T>
  class powerlaw
    :public model<data<optvec<T>,optvec<T> >,optvec<T>,std::string>
  {
  private:
    powerlaw* do_clone()const
    {
      return new powerlaw<T>(*this);
    }

    const char* do_get_type_name()const
    {
      return "1d power law";
    }
  public:
    powerlaw()
    {
      this->push_param_info(param_info<optvec<T> >("Ampl",1));
      this->push_param_info(param_info<optvec<T> >("gamma",1));
    }

    optvec<T> do_eval(const optvec<T>& x,const optvec<T>& param)
    {
      T A=get_element(param,0);
      T gamma=get_element(param,1);
      return A*pow(x,gamma);
    }

  private:
    std::string do_get_information()const
    {
#ifdef WITH_OPT_DOC
#include <model_doc/powerlaw.info>
#endif
      return "";
    }
  };
}



#endif
//EOF
