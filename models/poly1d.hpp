/**
   \file poly1d.hpp
   \brief 1d polynomial
   \author Junhua Gu
 */


#ifndef POLY_MODEL_H_
#define POLY_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>
#include <sstream>
#include <cassert>

namespace opt_utilities
{
  template <typename T,int n>
  class poly1d
    :public model<data<T,T>,std::vector<T>,std::string>
  {
  private:
    model<data<T,T>,std::vector<T> >* do_clone()const
    {
      return new poly1d<T,n>(*this);
    }

    const char* do_get_type_name()const
    {
      return "polynomial";
    }
  public:
    poly1d()
    {
      assert(n>=0);
      for(int i=0;i<=n;++i)
	{
	  std::ostringstream ostr;
	  ostr<<"a"<<i;
	  this->push_param_info(param_info<std::vector<T> >(ostr.str().c_str(),1));  
	}
      
    }

  public:
    T do_eval(const T& x,const std::vector<T>& param)
    {
      //      return x*get_element(param,0)+get_element(param,1);
      T result(0);
      for(int i=0;i<=n;++i)
	{
	  T xn(1);
	  for(int j=0;j<i;++j)
	    {
	      xn*=x;
	    }
	  result+=get_element(param,i)*xn;
	}
      return result;
    }

  private:
    std::string do_get_information()const
    {
      std::ostringstream ostr;
      ostr<<n<<"-order polynorminal model\n";
      for(int i=0;i<n;++i)
	{
	  ostr<<"a"<<i<<"*x^"<<i<<"+";
	}
      ostr<<"a"<<n<<"*x^"<<n;
      return ostr.str();
    }
  };
}



#endif
//EOF
