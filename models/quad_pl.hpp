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
    :public model<data<T,T>,std::vector<T>,std::string>
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
      this->push_param_info(param_info<std::vector<T> >("norm",1));
      this->push_param_info(param_info<std::vector<T> >("gamma",1));
      this->push_param_info(param_info<std::vector<T> >("corr",0));
    }

    T do_eval(const T& x,const std::vector<T>& param)
    {
      T norm=get_element(param,0);
      T gamma=get_element(param,1);
      T corr=get_element(param,2);
      return norm*pow(x,gamma)*exp(corr*log(x)*log(x));
    }
  };
}



#endif
//EOF
