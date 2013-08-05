/**
   \file normed_dgauss1d.hpp
   \brief normalized double gaussian distribution
   \author Junhua Gu
 */


#ifndef NDGAUSS_MODEL_H_
#define NDGAUSS_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>
#include <misc/optvec.hpp>

namespace opt_utilities
{
  template <typename T>
  class normed_dgauss1d
    :public model<data<optvec<T>,optvec<T> >,optvec<T>,std::string>
  {
  private:
    normed_dgauss1d* do_clone()const
    {
      return new normed_dgauss1d<T>(*this);
    }

    const char* do_get_type_name()const
    {
      return "1d double normed gaussian";
    }
  public:
    normed_dgauss1d()
    {
      this->push_param_info(param_info<optvec<T> >("x01",0));
      this->push_param_info(param_info<optvec<T> >("sigma1",1));
      this->push_param_info(param_info<optvec<T> >("x02",0.1));
      this->push_param_info(param_info<optvec<T> >("sigma2",1));
      this->push_param_info(param_info<optvec<T> >("theta",1));      
    }


  public:
    optvec<T> do_eval(const optvec<T>& x,const optvec<T>& param)
    {
      const double pi=3.14159265358979323846;
      T x01=get_element(param,0);
      T sigma1=get_element(param,1);
      T x02=get_element(param,2);
      T sigma2=get_element(param,3);
      T theta=get_element(param,4);
      if(sigma1*sigma1<std::numeric_limits<double>::epsilon())
	{
	  sigma1=std::numeric_limits<double>::epsilon();
	}
      if(sigma2*sigma2<std::numeric_limits<double>::epsilon())
	{
	  sigma2=std::numeric_limits<double>::epsilon();
	}
      T N1=1/sqrt(sigma1*sigma1*pi*2);
      T N2=1/sqrt(sigma2*sigma2*pi*2);
      
      optvec<T> y1=(x-x01)/sigma1;
      optvec<T> y2=(x-x02)/sigma2;
      
      T r1=sin(theta);
      T r2=cos(theta);
      
      return r1*r1*N1*exp(-y1*y1/2.)+r2*r2*N2*exp(-y2*y2/2.);;
    }

  private:
    std::string do_get_information()const
    {
#ifdef WITH_OPT_DOC
#include <model_doc/normed_dgauss1d.info>
#endif
      return "";
    }
  };
}



#endif
//EOF
