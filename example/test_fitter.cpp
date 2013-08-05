#include <core/optimizer.hpp>
#include <methods/powell/powell_method.hpp>
#include <methods/aga/aga.hpp>
#include <error_estimator/error_estimator.hpp>
#include <core/fitter.hpp>
#include <vector>
#include <iostream>
#include <data_sets/default_data_set.hpp>
#include <statistics/chisq.hpp>

using namespace opt_utilities;
//declear a class derived from func_obj
class lin1d
  :public opt_utilities::model<data<double,double>,std::vector<double>,std::string>
  {
  private:
    model<data<double,double>,std::vector<double> >* do_clone()const
    {
      return new lin1d(*this);
    }
  public:
    lin1d()
    {
      this->push_param_info(param_info<std::vector<double> >("k",1,-10,10));
      this->push_param_info(param_info<std::vector<double> >("b",0,-10,10));
    }

  public:
    double do_eval(const double& x,const std::vector<double>& param)
    {
      return x*get_element(param,0)+get_element(param,1);
    }

  private:
    std::string do_to_string()const
    {
      return "linear model\n"
	"y=k*x+b\n";
    }
  };


int main()
{
  fitter<data<double,double>,std::vector<double>,double,std::string> f;
  f.set_model(lin1d());
  f.set_opt_method(powell_method<double,std::vector<double> >());
  //f.set_opt_method(aga_method<double,std::vector<double> >());
  default_data_set<data<double,double> > ds;

  for(int i=0;i<100;++i)
    {
      ds.add_data(data<double,double>(i,i*3+5,.1,.1,0,0));
    }
  f.load_data(ds);
  f.set_statistic(chisq<data<double,double>,std::vector<double>,double,std::string>());
  f.fit();
  cout<<f.get_param_value("k")<<endl;
  cout<<f.get_param_value("b")<<endl;
  
  double lower=3;
  double upper=6;
  estimate_error(f,std::string("k"),lower,upper,1.,1E-10);
  std::cout<<lower<<"\t"<<upper<<endl;
}
