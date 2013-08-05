#include <core/fitter.hpp>
#include <misc/bootstrap.hpp>
#include <methods/powell/powell_method.hpp>
#include <statistics/chisq.hpp>
#include <models/nbeta1d.hpp>
#include <data_sets/default_data_set.hpp>
#include <misc/data_loaders.hpp>
#include <core/freeze_param.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;
using namespace opt_utilities;


int main(int argc,char* argv[])
{
  if(argc!=2)
    {
      cerr<<"Usage:"<<argv[0]<<" <data file>"<<endl;
      return -1;
    }
  fitter<data<double,double>,vector<double>,double,std::string> f;
  f.set_model(nbeta1d<double>());
  f.set_opt_method(powell_method<double,vector<double> >());
  f.set_statistic(chisq<double,double,vector<double>,double,std::string>());
  dl_x_xe_y_ye<double,double> dl;
  ifstream ifs(argv[1]);
  dl.load_from(ifs);
  f.load_data(dl.get_data_set());
  f.set_param_modifier(freeze<double,double,vector<double>,string>("bkg"));
  f.set_param_value("bkg",0);
  f.fit();
  for(int i=0;i<f.get_num_params();++i)
    {
      cout<<f.get_param_info(i).get_name()<<"="<<f.get_param_info(i).get_value()<<endl;
    }
  bootstrap<data<double,double>,vector<double>,double,std::string> bst;
  bst.set_fitter(f);
  bst.sample(1000);
}
