/**
   \file beta2d2.hpp
   \brief 2d beta model
   \author Junhua Gu
 */


#ifndef BETA_MODEL2d2_H_
#define BETA_MODEL2d2_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>
#include <cassert>
#include "vecn.hpp"


namespace opt_utilities
{
  
  template <typename T>
  class beta2d2
    :public model<data<T,vecn<T,2> >,std::vector<T>,std::string>
  {
  private:
    model<data<T,vecn<T,2> >,std::vector<T> >* do_clone()const
    {
      return new beta2d2<T>(*this);
    }

    const char* do_get_type_name()const
    {
      return "2d double beta model";
    }
  public:
    beta2d2()
    {
      this->push_param_info(param_info<std::vector<T> >("r0",20));
      this->push_param_info(param_info<std::vector<T> >("x0",100));
      this->push_param_info(param_info<std::vector<T> >("y0",100));
      this->push_param_info(param_info<std::vector<T> >("epsilon",0));
      this->push_param_info(param_info<std::vector<T> >("theta",100));
      this->push_param_info(param_info<std::vector<T> >("ampl",3));
      this->push_param_info(param_info<std::vector<T> >("beta",2./3.));
      this->push_param_info(param_info<std::vector<T> >("bkg",0));
    }


    T do_eval(const vecn<T,2>& xy,const std::vector<T>& param)
    {
      T x=xy[0];
      T y=xy[1];

      T r0=get_element(param,0);
      T x0=get_element(param,1);
      T y0=get_element(param,2);
      T epsilon=get_element(param,3);
      T theta=get_element(param,4);
      T ampl=get_element(param,5);
      T beta=get_element(param,6);
      T bkg=get_element(param,7);

      T x_new=(x-x0)*cos(theta)+(y-y0)*sin(theta);
      T y_new=(y-y0)*cos(theta)-(x-x0)*sin(theta);
      
      //T _epsilon=sin(epsilon)-0.00001;
      
      T _epsilon=epsilon;

      //      T r=sqrt(x_new*x_new*(1-_epsilon)*(1-_epsilon)
      //	       + y_new*y_new);
      
      //r/=(1-_epsilon);
      T r_r=x_new*x_new/exp(_epsilon)+y_new*y_new/exp(-_epsilon);
      
      
      return bkg+ampl*pow(1+r_r/r0/r0,-3*beta+static_cast<T>(.5));
    }


  };
}



#endif
//EOF
