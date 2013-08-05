#ifndef VDUALGAUSS1D_ESTIMATER
#define VDUALGAUSS1D_ESTIMATER
#include <core/pre_estimater.hpp>
#include <misc/optvec.hpp>
#include <vmodels/dualgauss1d.hpp>
#include <vector>

namespace opt_utilities
{
  template <typename T>
  class dualgauss1d_estimater
    :public pre_estimater<data<optvec<T>,optvec<T> >,optvec<T>,std::string>
  {
  public:
    dualgauss1d_estimater()
    {
      this->set_model_id("1d dualgaussian");
    }

    dualgauss1d_estimater<T>* do_clone()const
    {
      return new dualgauss1d_estimater<T>(*this);
    }

    void do_estimate(const data_set<data<optvec<T>,optvec<T> > >& d,model<data<optvec<T>,optvec<T> >,optvec<T>,std::string>& m)const
    {
      int n=d.size();

      T xp1st=0;
      T xp2nd=0;
      T yp1st=d.get_data(0).get_y()[0];
      T yp2nd=d.get_data(0).get_y()[0];

      for(int i=0;i<n;++i)
	{
	  if(d.get_data(i).get_y()[0]>yp1st)
	    {
	      yp1st=d.get_data(i).get_y()[0];
	      xp1st=d.get_data(i).get_x()[0];
	    }
	}
      
      for(int i=0;i<n;++i)
	{
	  
	}
      

      T xmean=0;
      T x2mean=0;
      T wgt=0;
      T wgt2=0;
      for(int i=0;i<n;++i)
	{
	  T x=d.get_data(i).get_x()[0];
	  T y=d.get_data(i).get_y()[0];
	  xmean+=x*y;
	  x2mean+=x*x*y;
	  wgt+=y;
	}
      xmean/=wgt;
      x2mean/=wgt;
      T sigma=std::sqrt(x2mean-xmean*xmean);
      m.set_param_value("x0",xmean);
      m.set_param_value("sigma",sigma);
      
    }
  };
}



#endif
