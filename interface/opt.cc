#include "opt.h"
#include <utilities/opt_types.hpp>
#include <math/num_diff.hpp>
#include <map>
#include <core/freeze_param.hpp>
#include <data_sets/default_data_set.hpp>
#include "type_depository.hpp"
#include <memory>

//models:
#include <models/gauss1d.hpp>
#include <models/bl1d.hpp>
#include <models/nfw1d.hpp>
#include <models/bpl1d.hpp>
#include <models/beta1d.hpp>
#include <models/nbeta1d.hpp>
#include <models/dbeta1d.hpp>
#include <models/lin1d.hpp>
#include <models/pl1d.hpp>
#include <models/poly1d.hpp>
#include <models/bremss.hpp>
#include <models/beta2d2.hpp>
#include <models/beta2d.hpp>
#include <models/dbeta2d2.hpp>
#include <models/dbeta2d3.hpp>
#include <models/dbeta2d.hpp>
#include <models/polar_ellipse.hpp>
//end models
//#include <iostream>

using namespace std;
using namespace opt_utilities;
static const int max_fit_space_num=100;
struct fit_space;
static map<int,fit_space> fit_space_map;
struct fit_space
{
  dopt::fitter fit;
  ///  dopt::model model;
  fit_space()
  {
    fit.set_opt_method(dopt::powell_method());
    dopt::chisq cq;
    dopt::leastsq lsq;
    //  cq.verbose(true);
    fit.set_statistic(cq);
  }
  
};

std::vector<std::string> model_names;

class func_obj_raw
  :public func_obj<double,std::vector<double> >
{
public:
  double (*pfunc)(const double*);
private:
  double do_eval(const std::vector<double>& x)
  {
    return (*pfunc)(x.data());
  }
  func_obj_raw* do_clone()const
  {
    return new func_obj_raw(*this);
  }
};


void regist_model(const dopt::model& m,const char* addr)
{
  
  if(find(model_names.begin(),model_names.end(),m.get_type_name())!=model_names.end())
    {
      cerr<<m.get_type_name()<<" has been registed"<<endl;
      return;
    }
  cerr<<"registing:"<<m.get_type_name()<<endl;
  model_names.push_back(m.get_type_name());
  opt_utilities::register_model(m);
}



class initializer
{
public:
  initializer()
  {
    regist_model(lin1d<double>(),"lin1d");
  }
}_initializer;

extern "C"
{
  void optimize_powell_(double (*pfunc)(const double*),const int& np,double* params,const double& precision)
  {
    std::vector<double> p(np);
    for(int i=0;i<np;++i)
      {
	p[i]=params[i];
      }
    func_obj_raw fo;
    fo.pfunc=pfunc;
    optimizer<double,std::vector<double> > opt;
    opt.set_func_obj(fo);
    opt.set_opt_method(powell_method<double,std::vector<double> >());
    opt.set_start_point(p);
    opt.set_precision(precision);
    p=opt.optimize();
    for(int i=0;i<np;++i)
      {
	params[i]=p[i];
      }
  }
  
  int alloc_fit_(int& n)
  {
    n=0;
    for(int i=1;i<max_fit_space_num;++i)
      {
	if(fit_space_map.find(i)==fit_space_map.end())
	  {
	    fit_space_map.insert(make_pair(i,fit_space()));
	    //cout<<i<<endl;
	    //return i;
	    n=i;
	    return 0;
	  }
      }
    
    return 1;
  }
  
  
  int free_fit_(const int& n)
  {
    map<int,fit_space>::iterator iter=fit_space_map.find(n);
    if(iter==fit_space_map.end())
      {
	return 1;
      }
    fit_space_map.erase(iter);
    return 0;
  }
  
  int load_data_(const int& nfit,const int& ndatas,double* x,double* y,double* yl,double* yu,double* xl,double* xu)
  {
    map<int,fit_space>::iterator iter=fit_space_map.find(nfit);
    if(iter==fit_space_map.end())
      {
	return 1;
      }
    //  cout<<x[0]<<endl;
    default_data_set<data<double,double> > ds;
    for(int i=0;i<ndatas;++i)
      {
	data<double,double> d(x[i],y[i],yl[i],(yu==0?yl[i]:yu[i]),(xl==0?0:xl[i]),(xu==0?0:xu[i]));
	//  cout<<x[i]<<" "<<y[i]<<endl;
	ds.add_data(d);
      }
    iter->second.fit.load_data(ds);
    return 0;
  }
  
  
  int set_model_(const int& nfit,const char* model_name)
  {
    map<int,fit_space>::iterator iter=fit_space_map.find(nfit);
    
    if(iter==fit_space_map.end())
      {
	cerr<<"fit not found"<<endl;
	
	return 1;
      }
    try
      {
	const dopt::model* p(opt_utilities::get_model<data<double,double>,std::vector<double>,std::string>(model_name));
	iter->second.fit.set_model(*p);
	return 0;
      }
    catch(opt_exception& e)
      {
	//cout<<model_name<<endl;
	cout<<e.what()<<endl;
	//throw e;
	return 1;
      }
  }
  
  int set_param_(const int& nfit,const char* pname,const double& value)
  {
    map<int,fit_space>::iterator iter=fit_space_map.find(nfit);
    cerr<<"pname="<<pname<<endl;
    cerr<<"value="<<value<<endl;
    if(iter==fit_space_map.end())
      {
	cerr<<"fit not found"<<endl;
	return 1;
      }
    iter->second.fit.set_param_value(pname,value);
    return 0;
  }
  
  
  int freeze_param_(const int& nfit,const char* pname)
  {
    map<int,fit_space>::iterator iter=fit_space_map.find(nfit);
    if(iter==fit_space_map.end())
      {
	return 1;
      }
    opt_utilities::freeze_param<data<double,double>,std::vector<double> > fp(pname);
    try
      {
	dynamic_cast<opt_utilities::freeze_param<data<double,double>,vector<double> >& >(iter->second.fit.get_param_modifier())+=fp;
	return 0;
      }
    catch(opt_exception& e)
      {
	(void)e;
	iter->second.fit.set_param_modifier(fp);
	return 0;
      }
    return 0;
  }
  
  
  int thraw_param_(const int& nfit,const char* pname)
  {
    map<int,fit_space>::iterator iter=fit_space_map.find(nfit);
    if(iter==fit_space_map.end())
      {
	return 1;
      }
    opt_utilities::freeze_param<data<double,double>,std::vector<double> > fp(pname);
    try
      {
	dynamic_cast<opt_utilities::freeze_param<data<double,double>,vector<double> >& >(iter->second.fit.get_param_modifier())-=fp;
      }
    catch(opt_exception& e)
      {
	(void)e;
	//iter->second.fit.set_param_modifier(fp);
      }
    return 0;
  }
  
  int perform_fit_(const int& nfit)
  {
    map<int,fit_space>::iterator iter=fit_space_map.find(nfit);
    if(iter==fit_space_map.end())
      {
	return 1;
      }
    iter->second.fit.fit();
    return 0;
  }
  
  
  int get_param_(const int& nfit,double& r,const char* pname)
  {
    map<int,fit_space>::iterator iter=fit_space_map.find(nfit);
    if(iter==fit_space_map.end())
      {
	//return 0;
	cerr<<"fit not found"<<endl;
	r=0;
	return 1;
      }
    //  cerr<<"fdsaf"<<r<<endl;
    r=iter->second.fit.get_param_value(pname);
    return 0;
  }
}

