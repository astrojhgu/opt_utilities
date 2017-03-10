#include <iostream>
#include <vector>
#include <string>
//#include <sstream>
using namespace std;
#include <core/opt_traits.hpp>
//#include <boost/python.hpp>


#include <core/fitter.hpp>

namespace opt_utilities
{
  template <typename T>
  class exp_poly
    :public model<data<T,T>,std::vector<T>,std::string>
  {
  private:
    const int Norder;
  private:
    exp_poly* do_clone()const
    {
      return new exp_poly<T>(*this);
    }
    
    const char* do_get_type_name()const
    {
      return "exp poly";
    }

  public:
    exp_poly(int _Norder)
      :Norder(_Norder)
    {
      this->push_param_info(param_info<std::vector<T> >("x0",0));
      for(int i=0;i<=Norder;++i)
	{
	  //ostringstream oss;
	  //oss<<"p"<<i;
	  
	  this->push_param_info(param_info<std::vector<T> >(std::string("p")+std::to_string(i),0));
	}
    }
    
  public:
    T do_eval(const T& x,const std::vector<T>& param)
    {
      T x0=param[0];
      T x_shifted=x-x0;
      T y=0;
      for(int i=0;i<=Norder;++i)
	{
	  T p=param[i+1];
	  for(int j=0;j<i;++j)
	    {
	      p*=x_shifted;
	    }
	  y+=p;
	}
      return exp(y);
    }
  };
}
