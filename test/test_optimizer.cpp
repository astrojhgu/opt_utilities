#define private public
#include <cassert>
#include <core/optimizer.hpp>
#include <methods/powell/powell_method.hpp>
#include <methods/lbfgs/lbfgs_method.hpp>
//#include <methods/gsl_simplex/gsl_simplex.hpp>
#include <methods/conjugate_gradient_hybrid/conjugate_gradient_hybrid.hpp>
#include <vector>
#include <iostream>
using namespace opt_utilities;
using namespace std;


class foo1
  :public func_obj<double,vector<double> >
{
  foo1* do_clone()const
  {
    return new foo1(*this);
  }

  double do_eval(const vector<double>& p)
  {
    double result=0;
    for(int i=0;i!=p.size();++i)
      {
	result+=p[i]*p[i];
      }
    return result;
  }
};

class foo2
  :public func_obj<double,vector<double> >
{
  foo2* do_clone()const
  {
    return new foo2(*this);
  }

  double do_eval(const vector<double>& p)
  {
    double result=0;
    for(int i=0;i!=p.size();++i)
      {
	result+=(i+1)*p[i]*p[i];
      }
    return result;
  }
};

class foo3
  :public func_obj<double,vector<double> >
{
  foo3* do_clone()const
  {
    return new foo3(*this);
  }

  double do_eval(const vector<double>& p)
  {
    double result=0;
    for(int i=0;i<p.size();++i)
      {
	double temp=0;
	for(int j=0;j<=i;++j)
	  {
	    temp+=p[j];
	  }
	result+=temp*temp;
      }

    return result;
  }
};


class foo4
  :public func_obj<double,vector<double> >
{
  foo4* do_clone()const
  {
    return new foo4(*this);
  }

  double do_eval(const vector<double>& p)
  {
    double result=0;
    for(int i=0;i<p.size()-1;++i)
      {
	result+=100*(p[i+1]-p[i])*(p[i+1]-p[i])
	  +(1-p[i])*(1-p[i]);
      }

    return result;
  }
};

class foo5
  :public func_obj<double,vector<double> >
{
  foo5* do_clone()const
  {
    return new foo5(*this);
  }

  double do_eval(const vector<double>& p)
  {
    const double pi=3.14159265358979323846;
    double result=0;
    result+=10*p.size();
    for(int i=0;i<p.size();++i)
      {
	result+=(p[i]*p[i]-10*cos(2*pi*p[i]));
      }

    return result;
  }
};


void test_opt(const func_obj<double,vector<double> >& fo,
	      const opt_method<double,vector<double> >& optm)
{
  const int problem_size=5;
  optimizer<double,vector<double> > opt;
  
  opt.set_func_obj(fo);
  opt.set_opt_method(optm);

  vector<double> p(problem_size);
  for(int i=0;i<p.size();++i)
    {
      p[i]=1000;
    }
  opt.set_start_point(p);
  opt.set_precision(1E-6);
  p=opt.optimize();


  cout<<"the result is:\n";
  for(int i=0;i<p.size();++i)
    {
      cout<<p[i]<<endl;
    }
  cout<<"The result of the object function is:\n"
      <<opt.eval(p)<<endl;

}


int main()
{
  //  gsl_simplex<double,vector<double> > pm_simplex;
  powell_method<double,vector<double> > pm_powell;
  lbfgs_method<double,vector<double> > lbfgsm;
  conjugate_gradient_hybrid<double,vector<double> > cgh;
  test_opt(foo1(),lbfgsm);
  test_opt(foo2(),lbfgsm);
  test_opt(foo3(),lbfgsm);
  test_opt(foo4(),pm_powell);
  test_opt(foo5(),pm_powell);
  test_opt(foo1(),cgh);
}
