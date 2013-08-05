/**
   \file lin1d.hpp
   \brief linear 
   \author Junhua Gu
 */


#ifndef VRQUARTER_MODEL_H_
#define VRQUARTER_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <misc/optvec.hpp>
#include <cmath>

namespace opt_utilities
{
  template <typename T>
  class rquarter
    :public model<data<optvec<T>,optvec<T> >,optvec<T>,std::string>
  {
    typedef optvec<T> Tv;
  private:
    rquarter<T>* do_clone()const
    {
      return new rquarter<T>(*this);
    }

    const char* do_get_type_name()const
    {
      return "de Vaucoulurs 1/4 law";
    }
  public:
    rquarter()
    {
      this->push_param_info(param_info<Tv>("I0",1));
      this->push_param_info(param_info<Tv>("Re",10));
      this->push_param_info(param_info<Tv>("bkg",0));
    }

  public:
    Tv do_eval(const Tv& x,const Tv& param)
    {
      Tv result(x.size());
      
      
      //return x*get_element(param,0)+get_element(param,1);
      for(size_t i=0;i!=x.size();++i)
	{
	  result[i]=param[0]*exp(-7.67*(pow(x[i]/param[1],.25)-T(1.)))+param[2];
	}
      return result;
    }

  private:
    std::string do_get_information()const
    {
#ifdef WITH_OPT_DOC
#include <model_doc/rquarter.info>
#endif
      return "";
    }
  };
}



#endif
//EOF
