/**
   \file linmin.hpp
   \brief linear search
   \author Junhua Gu
 */


#ifndef FLINMIN_HPP
#define FLINMIN_HPP
#define OPT_HEADER
#include "fmnbrak.hpp"
#include "fbrent.hpp"
#include <cmath>
#include <core/opt_traits.hpp>

namespace opt_utilities
{
  //for functional style
  template <typename rT,typename pT,typename fT>
  class fadaptor
  {
  private:
    const pT& p0;
    const pT& xi;
    const fT& func;
  public:
    fadaptor(const pT& _p0,const pT& _xi,const fT& _func)
      :p0(_p0),xi(_xi),func(_func)
    {};
    
  public:
    rT operator()(const rT& x)const
    {
      pT xt;
      opt_assign(xt,p0);
      for(size_t i=0;i<get_size(xt);++i)
	{
	  set_element(xt,i,get_element(xt,i)+x*get_element(xi,i));
	}
      return func(xt);
    }
  };

  //functional style
  template<typename rT,typename pT,typename fT>
  void flinmin(pT& p,pT& xi,rT& fret,fT& func)
  {
    
    //  assert(p.size()==10);
    //assert(xi.size()==10);
    //func_adaptor<rT,pT> fadpt(p,xi,func);
    fadaptor<rT,pT,fT> fadpt(p,xi,func);
    
    int j=0;
    const rT TOL=std::sqrt(std::numeric_limits<rT>::epsilon());
    rT xx=0,xmin=0,fx=0,fb=0,fa=0,bx=0,ax=0;
    int n=(int)get_size(p);
    
    
    ax=0.;
    xx=1.;
    

    fmnbrak(ax,xx,bx,fa,fx,fb,fadpt);
    //cout<<xx<<endl;
    fret=fbrent(ax,xx,bx,fadpt,TOL,xmin);
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
