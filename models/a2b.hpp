/**
   \file lin1d.hpp
   \brief 1d linear model
   \author Junhua Gu
 */


#ifndef A2B_MODEL_H_
#define A2B_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <core/pre_estimater.hpp>
#include <cmath>

namespace opt_utilities
{
  template <typename T>
  class a2b
    :public model<data<T,T>,std::vector<T>,std::string>
    ,public pre_estimatable<data<T,T>,std::vector<T>,std::string>
  {
  private:
    model<data<T,T>,std::vector<T> >* do_clone()const
    {
      return new a2b<T>(*this);
    }

    const char* do_get_type_name()const
    {
      return "1d linear model";
    }
  public:
    a2b()
    {
      this->push_param_info(param_info<std::vector<T> >("a",0));
      this->push_param_info(param_info<std::vector<T> >("b",1));
      this->push_param_info(param_info<std::vector<T> >("x0",0));
      this->push_param_info(param_info<std::vector<T> >("w",1));
    }

  public:
    T do_eval(const T& x,const std::vector<T>& param)
    {
      T a=get_element(param,0);
      T b=get_element(param,1);
      T x0=get_element(param,2);
      T w=get_element(param,3);
      static T half_pi=std::atan((T)1)*2;
      return (std::atan((x-x0)/w)/half_pi+1)/2*(b-a)+a;
    }

  private:
    std::string do_get_information()const
    {
      return "<math><mrow> <mtext>f(x;k,b)=k x+b</mtext>		\
    </mrow>								\
</math>";
    }
  };
}



#endif
//EOF
