/**
   \file guss1d.hpp
   \brief norm_gauss model
   \author Junhua Gu
 */


#ifndef NORM_POWERLAW_MODEL_H_
#define NORM_POWERLAW_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>

namespace opt_utilities
{
  template <typename T>
  class norm_powerlaw
    :public model<data<T,T>,std::vector<T>,std::string>
  {
  private:
    T lower_limit,upper_limit;
  private:
    model<data<T,T>,std::vector<T> >* do_clone()const
    {
      return new norm_powerlaw<T>(*this);
    }

    const char* do_get_type_name()const
    {
      return "1d norm_powerlaw";
    }
  public:
    norm_powerlaw()
      :lower_limit(1),upper_limit(2)
    {
      this->push_param_info(param_info<std::vector<T> >("gamma",1));
    }

    void set_limits(const T& a,const T& b)
    {
      lower_limit=a;
      upper_limit=b;
    }

  public:
    T do_eval(const T& x,const std::vector<T>& param)
    {
      T gamma=get_element(param,0);
      T N=0;
      T a=lower_limit;
      T b=upper_limit;
      if(gamma==-1)
	{
	  N=std::log(b/a);
	}
      else
	{
	  N=(pow(b,gamma+1)-pow(a,gamma+1))/(gamma+1);
	}
      return pow(x,gamma)/N;
    }

  private:
    std::string do_get_information()const
    {
      return "";
    }
  };
}



#endif
//EOF
