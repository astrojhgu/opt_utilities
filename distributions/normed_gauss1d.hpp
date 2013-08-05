/**
   \file normed_gauss1d.hpp
   \brief normalized guassian distribution
   \author Junhua Gu
 */


#ifndef NGAUSS_MODEL_H_
#define NGAUSS_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>
#include <misc/optvec.hpp>
#include <limits>

namespace opt_utilities
{
  template <typename T>
  class normed_gauss1d
    :public model<data<optvec<T>,optvec<T> >,optvec<T>,std::string>
  {
  private:
    normed_gauss1d* do_clone()const
    {
      return new normed_gauss1d<T>(*this);
    }

    const char* do_get_type_name()const
    {
      return "1d normed gaussian";
    }
  public:
    normed_gauss1d()
    {
      this->push_param_info(param_info<optvec<T> >("x0",0));
      this->push_param_info(param_info<optvec<T> >("sigma",1));
    }

    
  public:
    optvec<T> do_eval(const optvec<T>& x,const optvec<T>& param)
    {
      const double pi=3.14159265358979323846;
      T x0=get_element(param,0);
      T sigma=get_element(param,1);
      if(sigma*sigma<std::numeric_limits<double>::epsilon())
	{
	  sigma=std::numeric_limits<double>::epsilon();
	}
      T N=1/sqrt(sigma*sigma*pi*2);
      optvec<T> y=(x-x0)/sigma;
      return N*exp(-y*y/2.);
    }

  private:
    std::string do_get_information()const
    {
#ifdef WITH_OPT_DOC
#include <model_doc/normed_gauss1d.info>
#endif
      return "";
    }
  };
}



#endif
//EOF
