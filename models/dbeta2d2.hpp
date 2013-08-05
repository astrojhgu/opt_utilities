/**
   \file dbeta2de.hpp
   \brief 2d double beta model
   \author Junhua Gu
 */


#ifndef DDBETA_OOOMODEL2d2_H_
#define DDBETA_OOOMODEL2d2_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>
#include <cassert>
#include "vecn.hpp"


namespace opt_utilities
{
  
  template <typename T>
  class dbeta2d2
    :public model<data<T,vecn<T,2> >,std::vector<T>,std::string>
  {
  private:
    model<data<T,vecn<T,2> >,std::vector<T> >* do_clone()const
    {
      return new dbeta2d2<T>(*this);
    }

    const char* do_get_type_name()const
    {
      return "2d double beta model 2";
    }
  public:
    dbeta2d2()
    {
      this->push_param_info(param_info<std::vector<T> >("ampl1",1));//0
      this->push_param_info(param_info<std::vector<T> >("r01",1));//1
      this->push_param_info(param_info<std::vector<T> >("x01",100));//2
      this->push_param_info(param_info<std::vector<T> >("y01",100));//3      
      this->push_param_info(param_info<std::vector<T> >("theta1",0));//4      
      this->push_param_info(param_info<std::vector<T> >("beta1",2./3.));//5
      this->push_param_info(param_info<std::vector<T> >("epsilon1",0));//6


      this->push_param_info(param_info<std::vector<T> >("ampl2",1));//7
      this->push_param_info(param_info<std::vector<T> >("r02",1));//8
      this->push_param_info(param_info<std::vector<T> >("x02",100));//9
      this->push_param_info(param_info<std::vector<T> >("y02",100));//10
      this->push_param_info(param_info<std::vector<T> >("theta2",0));//11
      this->push_param_info(param_info<std::vector<T> >("beta2",2./4.));//12
      this->push_param_info(param_info<std::vector<T> >("epsilon2",0));//13

      this->push_param_info(param_info<std::vector<T> >("bkg",0));//14
    }


    T do_eval(const vecn<T,2>& xy,const std::vector<T>& param)
    {
      T x=xy[0];
      T y=xy[1];

      T ampl1=get_element(param,0);
      T r01=get_element(param,1);
      T x01=get_element(param,2);
      T y01=get_element(param,3);

      T theta1=get_element(param,4);

      T beta1=get_element(param,5);
      T epsilon1=get_element(param,6);

      T ampl2=get_element(param,7);      
      T r02=get_element(param,8);
      T x02=get_element(param,9);
      T y02=get_element(param,10);
   
      T theta2=get_element(param,11);

      T beta2=get_element(param,12);

      T epsilon2=get_element(param,13);
      T bkg=get_element(param,14);

      T x_new1=(x-x01)*cos(theta1)+(y-y01)*sin(theta1);
      T y_new1=(y-y01)*cos(theta1)-(x-x01)*sin(theta1);
      
      //T r1=sqrt(x_new1*x_new1*(1-epsilon1)*(1-epsilon1)
      //       + y_new1*y_new1);
      //r1/=(1-epsilon1);

      T r1_r1=x_new1*x_new1/exp(epsilon1/30)+y_new1*y_new1/exp(-epsilon1/30);

      T x_new2=(x-x02)*cos(theta2)+(y-y02)*sin(theta2);
      T y_new2=(y-y02)*cos(theta2)-(x-x02)*sin(theta2);
      
      T r2_r2=x_new2*x_new2/exp(epsilon2/30)+y_new2*y_new2/exp(-epsilon2/30);
      //   T r2=sqrt(x_new2*x_new2*(1-epsilon2)*(1-epsilon2)
      //	       + y_new2*y_new2);
      //r2/=(1-epsilon2);
      
      
      
      return bkg+ampl1*pow(1+(r1_r1/r01/r01),-3*beta1+static_cast<T>(.5))
	+ampl2*pow(1+(r2_r2/r02/r02),-3*beta2+static_cast<T>(.5));
    }
  };
}



#endif
//EOF
