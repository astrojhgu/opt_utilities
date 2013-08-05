/**
   \file dualgauss1d.hpp
   \brief Dualgauss model
   \author Junhua Gu
 */

#ifndef DUALGAUSS_MODEL_H_
#define DUALGAUSS_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>
#include <misc/optvec.hpp>

namespace opt_utilities
{
  template <typename T>
  class dualgauss1d
    :public model<data<optvec<T>,optvec<T> >,optvec<T>,std::string>
    ,public pre_estimatable<data<optvec<T>,optvec<T> >,optvec<T>,std::string>
  {
  private:
    dualgauss1d* do_clone()const
    {
      return new dualgauss1d<T>(*this);
    }

    const char* do_get_type_name()const
    {
      return "1d dualgaussian";
    }
  public:
    dualgauss1d()
    {
      this->push_param_info(param_info<optvec<T> >("N1",1));
      this->push_param_info(param_info<optvec<T> >("x01",0));
      this->push_param_info(param_info<optvec<T> >("sigma1",1));
      this->push_param_info(param_info<optvec<T> >("N2",1));
      this->push_param_info(param_info<optvec<T> >("x02",1));
      this->push_param_info(param_info<optvec<T> >("sigma2",1));
    }

  public:
    optvec<T> do_eval(const optvec<T>& x,const optvec<T>& param)
    {
      T N1=get_element(param,0);
      T x01=get_element(param,1);
      T sigma1=get_element(param,2);
      
      T N2=get_element(param,3);
      T x02=get_element(param,4);
      T sigma2=get_element(param,5);

      optvec<T> y1=(x-x01)/sigma1;
      optvec<T> y2=(x-x02)/sigma2;
      return N1*exp(-y1*y1/2.)+N2*exp(-y2*y2/2.);
    }

  private:
    std::string do_get_information()const
    {
#ifdef WITH_OPT_DOC
#include <model_doc/dualgauss1d.info>
#endif
      return "";
    }
  };
}



#endif
//EOF
