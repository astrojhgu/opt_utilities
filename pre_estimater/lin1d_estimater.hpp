#ifndef LIN1D_ESTIMATER
#define LIN1D_ESTIMATER
#include "pre_estimater.hpp"
#include <misc/optvec.hpp>
#include <models/lin1d.hpp>
#include <vector>

namespace opt_utilities
{
  template <typename T>
  class lin1d_estimater
    :public pre_estimater<T,T,std::vector<T>,std::string>
  {
  public:
    lin1d_estimater()
    {
      this->set_model_id("1d linear model");
    }

    lin1d_estimater<T>* do_clone()const
    {
      return new lin1d_estimater<T>(*this);
    }

    void do_estimate(const data_set<T,T>& d,model<T,T,std::vector<T>,std::string>& m)const
    {
      T n=d.size();
      T sy=0;
      T sxx=0;
      T sx=0;
      T sxy=0;
      
      for(int i=0;i<d.size();++i)
	{
	  sy+=d.get_data(i).get_y();
	  sxx+=d.get_data(i).get_x()*d.get_data(i).get_x();
	  sx+=d.get_data(i).get_x();
	  sxy+=d.get_data(i).get_x()*d.get_data(i).get_y();
	}
      T b=(sy*sxx-sx*sxy)/(n*sxx-sx*sx);
      T k=(n*sxy-sx*sy)/(n*sxx-sx*sx);

      m.set_param_value("k",k);
      m.set_param_value("b",b);
      
    }
  };
}



#endif
