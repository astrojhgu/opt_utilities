/**
   \file lin1d.hpp
   \brief linear 
   \author Junhua Gu
 */


#ifndef VLINEAR_MODEL_H_
#define VLINEAR_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <misc/optvec.hpp>
#include <core/pre_estimater.hpp>
#include <cmath>

namespace opt_utilities
{
  template <typename T>
  class lin1d
    :public model<data<optvec<T>,optvec<T> >,optvec<T>,std::string>
    ,public pre_estimatable<data<optvec<T>,optvec<T> >,optvec<T>,std::string>
  {
    typedef optvec<T> Tv;
  private:
    lin1d<T>* do_clone()const
    {
      return new lin1d<T>(*this);
    }

    const char* do_get_type_name()const
    {
      return "1d linear model";
    }
  public:
    lin1d()
    {
      this->push_param_info(param_info<Tv>("k",1));
      this->push_param_info(param_info<Tv>("b",0));
    }

  public:
    Tv do_eval(const Tv& x,const Tv& param)
    {
      Tv result(x.size());
      
      //return x*get_element(param,0)+get_element(param,1);
      for(size_t i=0;i!=x.size();++i)
	{
	  result[i]=param[0]*x[i]+param[1];
	}
      return result;
    }

  private:
    std::string do_get_information()const
    {
#ifdef WITH_OPT_DOC
#include <model_doc/lin1d.info>
#endif
      return "";
    }
  };
}



#endif
//EOF
