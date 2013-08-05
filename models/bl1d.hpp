/**
   \file bl1d.hpp
   \brief 1d broken linear
   \author Junhua Gu
 */


#ifndef BROKEN_LINE_MODEL_H_
#define BROKEN_LINE_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>

namespace opt_utilities
{
  template <typename T>
  class bl1d
    :public model<data<T,T>,std::vector<T>,std::string>
  {
  private:
    model<data<T,T>,std::vector<T> >* do_clone()const
    {
      return new bl1d<T>(*this);
    }
    
    const char* do_get_type_name()const
    {
      return "broken linear model";
    }
  public:
    bl1d()
    {
      this->push_param_info(param_info<std::vector<T> >("break point y value",1));
      this->push_param_info(param_info<std::vector<T> >("break point x value",1));
      this->push_param_info(param_info<std::vector<T> >("slop 1",1));
      this->push_param_info(param_info<std::vector<T> >("slop 2",1));
    }

  public:
    T do_eval(const T& x,const std::vector<T>& param)
    {
      T x_b=get_element(param,0);
      T f_b=get_element(param,1);
      T k1=get_element(param,2);
      T k2=get_element(param,3);
      if(x<x_b)
	{
	  return k1*(x-x_b)+f_b;
	}
      else
	{
	  return k2*(x-x_b)+f_b;
	}
    }

  private:
    std::string do_get_information()const
    {
      return "broken linear model\n"
	"y=k1*(x-x_b)+y_b for x<x_b\n"
	"y=k2*(x-x_b)+y_b otherwise\n";
    }
  };
}



#endif
//EOF
