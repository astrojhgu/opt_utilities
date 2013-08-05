/**
   \file beta2d.hpp
   \brief 2d beta 
   \author Junhua Gu
 */


#ifndef BETA_MODEL2d_H_
#define BETA_MODEL2d_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>
#include <cassert>
#include "vecn.hpp"


namespace opt_utilities
{
  
  template <typename T>
  class beta2d
    :public model<data<T,vecn<T,2> >,std::vector<T>,std::string>
  {
  private:
    model<data<T,vecn<T,2> >,std::vector<T> >* do_clone()const
    {
      return new beta2d<T>(*this);
    }

    const char* do_get_type_name()const
    {
      return "2d beta model";
    }
  public:
    beta2d()
    {
      this->push_param_info(param_info<std::vector<T> >("S0",1));
      this->push_param_info(param_info<std::vector<T> >("rc1",100));
      this->push_param_info(param_info<std::vector<T> >("rc2",100));
      this->push_param_info(param_info<std::vector<T> >("rho",0));
      this->push_param_info(param_info<std::vector<T> >("x0",100));
      this->push_param_info(param_info<std::vector<T> >("y0",100));
      this->push_param_info(param_info<std::vector<T> >("beta",2./3.));
      this->push_param_info(param_info<std::vector<T> >("bkg",0));
    }


    T do_eval(const vecn<T,2>& xy,const std::vector<T>& param)
    {
      T S0=get_element(param,0);
      T rc1=get_element(param,1);
      T rc2=get_element(param,2);
      T rho=get_element(param,3);
      T x0=get_element(param,4);
      T y0=get_element(param,5);
      T beta=get_element(param,6);
      T bkg=get_element(param,7);
      T x=xy[0];
      T y=xy[1];

      T r=(x-x0)*(x-x0)/(rc1*rc1)+(y-y0)*(y-y0)/(rc2*rc2)
	-2*rho*(x-x0)*(y-y0)/(rc1*rc2);
      r=r<0?0:r;
      
      return bkg+S0*pow(1+r,-3*beta+static_cast<T>(.5));
    }
  };
}



#endif
//EOF
