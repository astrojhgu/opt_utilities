/**
   \file fpowell_method.hpp
   \brief powerll optimization method
   \author Junhua Gu
 */

#ifndef FPOWELL_METHOD
#define FPOWELL_METHOD
#define OPT_HEADER
#include <core/optimize.hpp>
//#include <blitz/array.h>
#include <limits>
#include <cassert>
#include <cmath>
#include "../linmin/flinmin.hpp"
#include <algorithm>
#include <iostream>

namespace opt_utilities
{
  /**
     \brief Impliment of an optimization method
     \tparam rT return type of the object function
     \tparam pT parameter type of the object function
   */
  template <typename rT,typename pT,typename fT>
  class fpowell_method
    :public fopt_method<rT,pT,fT>
  {
  private:
    typedef rT T;
  private:
    pT start_point;
    rT threshold;
  private:

    void powell(pT& p,const T ftol,
		int& iter,T& fret,const fT& func)const
    {
      int n=(int)get_size(p);
	    
      std::vector<T> xi_1d(n*n);
      std::vector<T*> xi(n);
      for(int i=0;i!=n;++i)
	{
	  xi[i]=xi_1d.data()+i*n;
	}
      for(int i=0;i!=n;++i)
	{
	  for(int j=0;j!=n;++j)
	    {
	      xi[i][j]=(j==i?1:0);
	    }
	}
      const int ITMAX=200;
      const T TINY=std::numeric_limits<T>::epsilon();
      int i,j,ibig;
      T del,fp,fptt,t;
      pT pt(n);
      pT ptt(n);
      pT xit(n);
      fret=func(p);

      for(j=0;j<n;++j)
	{
	  //get_element(pt,j)=get_element(p,j);
	  set_element(pt,j,get_element(p,j));
	}
      bool bstop=false;
      for(iter=0;!bstop;++iter)
	{
	  fp=fret;
	  ibig=0;
	  del=0.0;
	  for(i=0;i<n;++i)
	    {
#ifdef OPT_USE_OMP
#pragma omp parallel for 
#endif
	      for(j=0;j<n;++j)
		{
		  //get_element(xit,j)=xi[j][i];
		  set_element(xit,j,xi[j][i]);
		}
	      fptt=fret;
	      flinmin(p,xit,fret,func);
	      if((fptt-fret)>del)
		{
		  del=fptt-fret;
		  ibig=i+1;
		}
	    }
	  if(T(2.)*(fp-fret)<=ftol*(tabs(fp)+tabs(fret))+TINY)
	    {
	      return;
	    }
	  if(iter==ITMAX)
	    {
	      std::cerr<<"powell exceeding maximun iterations."<<std::endl;
	      return;
	    }
#ifdef OPT_USE_OMP
#pragma omp parallel for 
#endif
	  for(j=0;j<n;++j)
	    {
	      //get_element(ptt,j)=T(2.)*get_element(p,j)-get_element(pt,j);
	      set_element(ptt,j,T(2.)*get_element(p,j)-get_element(pt,j));
	      //get_element(xit,j)=
	      //get_element(p,j)-get_element(pt,j);
	      set_element(xit,j,get_element(p,j)-get_element(pt,j));
	      //get_element(pt,j)=get_element(p,j);
	      set_element(pt,j,get_element(p,j));
	    }
	  fptt=func(ptt);
	  if(fptt<fp)
	    {
	      t=T(2.)*(fp-T(2.)*fret+fptt)*sqr(T(fp-fret-del))-del*sqr(T(fp-fptt));
	      if(t<T(0.))
		{
		  flinmin(p,xit,fret,func);
#ifdef OPT_USE_OMP
#pragma omp parallel for 
#endif
		  for(j=0;j<n;++j)
		    {
		      xi[j][ibig-1]=xi[j][n-1];
		      xi[j][n-1]=get_element(xit,j);
		      
		    }
		}
	    }
	}
    }

    
  public:
    fpowell_method(const pT& _start_point,const rT& _threshold)
      :start_point(_start_point),threshold(_threshold)
    {
    }

    virtual ~fpowell_method()
    {
    };

  public:
    
    pT do_optimize(const fT& func)const
    {
      int iter=100;
      pT end_point;
      opt_assign(end_point,start_point);
      rT fret;
      powell(end_point,threshold,iter,fret,func);
      return end_point;
    }
    
  private:
    

    fpowell_method(const fpowell_method<rT,pT,fT>& rhs)
    {
    }

    fpowell_method<rT,pT,fT>& operator=(const fpowell_method<rT,pT,fT>& rhs)
    {
    }
    


  };

}


#endif
//EOF
