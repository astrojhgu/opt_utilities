/**
   \file bl.hpp
   \brief broken linear
   \author Junhua Gu
 */


#ifndef BROKEN_LINE_MODEL_H_
#define BROKEN_LINE_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>
#include <misc/optvec.hpp>
namespace opt_utilities
{
  template <typename T>
  class bl
    :public model<data<optvec<T>,optvec<T> >,optvec<T>,std::string>
  {
  private:
    model<data<optvec<T>,optvec<T> >,optvec<T>,std::string >* do_clone()const
    {
      return new bl<T>(*this);
    }
    
    const char* do_get_type_name()const
    {
      return "broken linear model";
    }
  public:
    bl()
    {
      this->push_param_info(param_info<optvec<T> >("break point y value",1));
      this->push_param_info(param_info<optvec<T> >("break point x value",1));
      this->push_param_info(param_info<optvec<T> >("slop 1",1));
      this->push_param_info(param_info<optvec<T> >("slop 2",1));
    }

  public:
    optvec<T> do_eval(const optvec<T>& x,const optvec<T>& param)
    {
      T x_b=get_element(param,0);
      T f_b=get_element(param,1);
      T k1=get_element(param,2);
      T k2=get_element(param,3);
      optvec<double> result(x.size());
      for(int i=0;i<x.size();++i)
	{
	  if(x[i]<x_b)
	    {
	      result[i]=k1*(x[i]-x_b)+f_b;
	    }
	  else
	    {
	      result[i]=k2*(x[i]-x_b)+f_b;
	    }
	}
      return result;
    }

  private:
    std::string do_get_information()const
    {
#ifdef WITH_OPT_DOC
#include <model_doc/bl.info>
#endif
      return "";
    }
  };
}



#endif
//EOF
