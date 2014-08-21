/**
   \file linmin.hpp
   \brief linear search
   \author Junhua Gu
 */


#ifndef LINMIN_HPP
#define LINMIN_HPP
#define OPT_HEADER
#include "mnbrak.hpp"
#include "brent.hpp"
#include <cmath>
#include <core/opt_traits.hpp>

namespace opt_utilities
{
  template <typename rT,typename pT>
  class func_adaptor
    :public func_obj<rT,rT>
  {
  private:
    const pT p1,xi1;
    const func_obj<rT,pT>* pfoo;
    func_adaptor(){}
    func_adaptor(const func_adaptor&)
      :func_obj<rT,rT>(),p1(),xi1(),pfoo(NULL_PTR)
    {}
    
  public:
    /*
    void set_origin(pT& p2)
    {
      p1=p2;
    }
    
    void set_direction(pT& xi2)
    {
      xi1=xi2;
    }
    
    void set_func_obj(func_obj<rT,pT>& foo)
    {
      pfoo=&foo;
      }*/
  public:
    func_adaptor(const pT& _p,const pT& _xi,const func_obj<rT,pT>& pf)
      :p1(_p),xi1(_xi),pfoo(&pf)
    {}
    
  private:
    func_obj<rT,rT>* do_clone()const
    {
      return new func_adaptor(*this);
    }
      
    rT do_eval(const rT& x)
    {
      //assert(p1.size()==xi1.size());
      
      pT xt;
      opt_assign(xt,p1);
#ifdef OPT_USE_OMP
#pragma omp parallel for 
#endif
      for(size_t i=0;i<get_size(xt);++i)
	{
	  //get_element(xt,i)+=x*get_element((pT)xi1,i);
	  set_element(xt,i,
		      get_element(xt,i)+x*get_element(xi1,i));
	  //get_element((pT)xi1,i);
	}
      return const_cast<func_obj<rT,pT>&>(*pfoo).eval(xt);
      //return x;
    }
  };
  

  template<typename rT,typename pT>
  void linmin(pT& p,pT& xi,rT& fret,func_obj<rT,pT>& func)
  {

    //  assert(p.size()==10);
    //assert(xi.size()==10);
    func_adaptor<rT,pT> fadpt(p,xi,func);

    int j=0;
    const rT TOL=std::sqrt(std::numeric_limits<rT>::epsilon());
    rT xx=0,xmin=0,fx=0,fb=0,fa=0,bx=0,ax=0;
    int n=(int)get_size(p);


    ax=0.;
    xx=1.;
    

    mnbrak(ax,xx,bx,fa,fx,fb,fadpt);
    //cout<<xx<<endl;
    fret=brent(ax,xx,bx,fadpt,TOL,xmin);
    //cout<<xmin<<endl;
#ifdef OPT_USE_OMP
#pragma omp parallel for 
#endif
    for(j=0;j<n;++j)
      {
	//get_element(xi,j)*=xmin;
	set_element(xi,j,
		    get_element(xi,j)*xmin);
	//get_element(p,j)+=get_element(xi,j);
	set_element(p,j,
		    get_element(p,j)+get_element(xi,j));
      }
    //  delete xicom_p;
    //delete pcom_p;
  }
}


#endif
