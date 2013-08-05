/**
   \file quad_pl.hpp
   \brief power law model
   \author Junhua Gu
 */


#ifndef QUAD_PL_MODEL_H_
#define QUAD_PL_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>
#include <misc/optvec.hpp>

namespace opt_utilities
{
  template <typename T>
  class quad_pl
    :public model<data<optvec<T>,optvec<T> >,optvec<T>,std::string>
  {
  private:
    quad_pl* do_clone()const
    {
      return new quad_pl<T>(*this);
    }

    const char* do_get_type_name()const
    {
      return "quad power law";
    }
  public:
    quad_pl()
    {
      this->push_param_info(param_info<optvec<T> >("norm",1));
      this->push_param_info(param_info<optvec<T> >("gamma",1));
      this->push_param_info(param_info<optvec<T> >("corr",1));
    }

    optvec<T> do_eval(const optvec<T>& x,const optvec<T>& param)
    {
      T norm=get_element(param,0);
      T gamma=get_element(param,1);
      T corr=get_element(param,2);
      return norm*pow(x,gamma)*exp(corr*log(x)*log(x));
    }

  private:
    std::string do_get_information()const
    {
#ifdef WITH_OPT_DOC
#include <model_doc/quad_pl.info>
#endif
      return "";
    }
  };
}



#endif
//EOF
