/**
   \file dbeta2d3.hpp
   \brief 2d double beta model
   \author Junhua Gu
 */


#ifndef DDBETA3_MODEL2d2_H_
#define DDBETA3_MODEL2d2_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>
#include <cassert>
#include "vecn.hpp"


namespace opt_utilities
{
  
  template <typename T>
  class dbeta2d3
    :public model<data<T,vecn<T,2> >,std::vector<T>,std::string>
  {
  private:
    model<data<T,vecn<T,2> >,std::vector<T> >* do_clone()const
    {
      return new dbeta2d3<T>(*this);
    }

    const char* do_get_type_name()const
    {
      return "2d double beta model 3";
    }
  public:
    dbeta2d3()
    {

      this->push_param_info(param_info<std::vector<T> >("x0",256));//1
      this->push_param_info(param_info<std::vector<T> >("y0",256));//2      
      this->push_param_info(param_info<std::vector<T> >("epsilon",0));//3
      this->push_param_info(param_info<std::vector<T> >("theta",0));//4      
      
      this->push_param_info(param_info<std::vector<T> >("ampl1",1));//5
      this->push_param_info(param_info<std::vector<T> >("beta1",0.6));//6
      this->push_param_info(param_info<std::vector<T> >("r01",30));//7
      
      this->push_param_info(param_info<std::vector<T> >("ampl2",.5));//8
      this->push_param_info(param_info<std::vector<T> >("beta2",.4));//9
      this->push_param_info(param_info<std::vector<T> >("r02",20));//10
      
      this->push_param_info(param_info<std::vector<T> >("bkg",0));//11


    }


    T do_eval(const vecn<T,2>& xy,const std::vector<T>& param)
    {
      T x=xy[0];
      T y=xy[1];
      
      T x0=get_element(param,0);
      T y0=get_element(param,1);
      T epsilon=get_element(param,2);
      T theta=get_element(param,3);
 
      T ampl1=(get_element(param,4));
      T beta1=(get_element(param,5));
      T r01=(get_element(param,6));

      T ampl2=(get_element(param,7));
      T beta2=(get_element(param,8));
      T r02=(get_element(param,9));

      T bkg=get_element(param,10);

      T x_new1=(x-x0)*cos(theta)+(y-y0)*sin(theta);
      T y_new1=(y-y0)*cos(theta)-(x-x0)*sin(theta);
      
 
      T r1_r1=x_new1*x_new1/exp(epsilon/30)+y_new1*y_new1/exp(-epsilon/30);
      //T r1=sqrt(x_new1*x_new1*(1-epsilon)*(1-epsilon)+y_new1*y_new1)/(1-epsilon);

      T x_new2=(x-x0)*cos(theta)+(y-y0)*sin(theta);
      T y_new2=(y-y0)*cos(theta)-(x-x0)*sin(theta);
      
      T r2_r2=x_new2*x_new2/exp(epsilon/30)+y_new2*y_new2/exp(-epsilon/30);
      //T r2=sqrt(x_new2*x_new2*(1-epsilon)*(1-epsilon)+y_new2*y_new2)/(1-epsilon);

      
      
      return bkg+ampl1*pow(1+(r1_r1/r01/r01),-3*beta1+static_cast<T>(.5))
      	+ampl2*pow(1+(r2_r2/r02/r02),-3*beta2+static_cast<T>(.5));

      //return bkg+pow(1+r1*r1/r01/r01,-3*beta1+static_cast<T>(.5))+
      //pow(1+r2*r2/r02/r02,-3*beta2+static_cast<T>(.5));
    }
  };
}



#endif
//EOF
