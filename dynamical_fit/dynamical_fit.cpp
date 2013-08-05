#include <core/fitter.hpp>
#include <interface/optdl.hpp>
#include <misc/data_loaders.hpp>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <methods/powell/powell_method.hpp>
#include <methods/gsl_simplex/gsl_simplex.hpp>
#include <statistics/chisq.hpp>
#include <cstdlib>

using namespace std;
using namespace opt_utilities;


int main(int argc,char* argv[])
{
  if(argc!=3)
    {
      cerr<<"Usage: "<<argv[0]<<" <config file> <model dump file>"<<endl;
      exit(-1);
    }
  ifstream cfg_file(argv[1]);
  fitter<data<double,double>,std::vector<double>,double,std::string> fit;
  chisq<data<double,double>,vector<double>,double,string> stat;
  stat.verbose(true);
  fit.set_opt_method(powell_method<double,vector<double> >());
  //
  fit.set_statistic(stat);
  std::string model_so_name;
  cfg_file>>model_so_name;
  cerr<<"loading model shared object "<<model_so_name<<endl;
  fit.set_model(*load_model<data<double,double>,vector<double>,string>(model_so_name.c_str()));
  
  string data_file_name;
  cfg_file>>data_file_name;
  cerr<<"setting initializational values:"<<endl;
  for(;;)
    {
      string p,v;
      cfg_file>>p>>v;
      if(!cfg_file.good())
	{
	  break;
	}
      istringstream oss(v);
      
      double dvp=0;
      oss>>dvp;
      cerr<<p<<"="<<dvp<<endl;
      fit.set_param_value(p,dvp);
    }
  cfg_file.close();

  ifstream data_file(data_file_name.c_str());
  
  dl_x_xu_xl_y_yu_yl<double,double> dl;
  dl.load_from(data_file);
  data_file.close();

  fit.load_data(dl.get_data_set());

  fit.set_precision(1e-5);
  fit.fit();
  //fit.set_opt_method(gsl_simplex<double,vector<double> >());
  fit.fit();
  fit.fit();

  vector<double> p=fit.fit();
  for(int i=0;i<fit.get_num_params();++i)
    {
      cerr<<fit.get_param_info(i).get_name()<<"\t"<<fit.get_param_info(i).get_value()<<endl;
    }
  
  ofstream ofs(argv[2]);
  ofs<<"no no no\n";
  double xmin,xmax;
  xmin=fit.get_data_set().get_data(0).get_x();
  xmax=fit.get_data_set().get_data(0).get_x();
  for(int i=0;i<fit.get_data_set().size();++i)
    {
      xmin=std::min(xmin,fit.get_data_set().get_data(i).get_x());
      xmax=std::max(xmax,fit.get_data_set().get_data(i).get_x());
    }
  
  for(double x=xmin;x<xmax;x+=(xmax-xmin)/1000.)
    {
      ofs<<x<<"\t0\t0\t"<<fit.eval_model_raw(x,fit.get_all_params())<<"\t0\t0"<<endl;
    }

}
