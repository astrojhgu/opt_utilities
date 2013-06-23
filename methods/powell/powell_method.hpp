/**
   \file powell_method.hpp
   \brief powerll optimization method
   \author Junhua Gu
 */

#ifndef POWELL_METHOD
#define POWELL_METHOD
#define OPT_HEADER
#include <core/optimizer.hpp>
//#include <blitz/array.h>
#include <limits>
#include <cassert>
#include <cmath>
#include "../linmin/linmin.hpp"
#include <algorithm>
#include <iostream>

namespace opt_utilities
{
  /**
     \brief Impliment of an optimization method
     \tparam rT return type of the object function
     \tparam pT parameter type of the object function
   */
  template <typename rT,typename pT>
  class powell_method
    :public opt_method<rT,pT>
  {
  public:
    typedef pT array1d_type;
    typedef rT T;
  private:
    func_obj<rT,pT>* p_fo;
    optimizer<rT,pT>* p_optimizer;
    volatile bool bstop;
    //typedef blitz::Array<rT,2> array2d_type;
    
    const char* do_get_type_name()const
    {
      return "powell method";
    }
  private:
    array1d_type start_point;
    array1d_type end_point;
    
  private:
    int ncom;
    array1d_type pcom_p;
    array1d_type xicom_p;
    rT threshold;
    T** xi;
    T* xi_1d;
  private:
    rT func(const pT& x)
    {
      assert(p_fo!=NULL_PTR);
      return p_fo->eval(x);
    }

   
  private:
    void clear_xi()
    {
      if(xi_1d!=NULL_PTR)
	{
	  delete[] xi_1d;
	}
      if(xi!=NULL_PTR)
	{
	  delete[] xi;
	}
    }

    void init_xi(int n)
    {
      clear_xi();
      xi_1d=new T[n*n];
      xi=new T*[n];
      for(int i=0;i!=n;++i)
	{
	  xi[i]=xi_1d+i*n;
	}
      for(int i=0;i!=n;++i)
	{
	  for(int j=0;j!=n;++j)
	    {
	      xi[i][j]=(j==i?1:0);
	    }
	}
    }



    void powell(array1d_type& p,const T ftol,
	   int& iter,T& fret)
    {
      const int ITMAX=200;
      const T TINY=std::numeric_limits<T>::epsilon();
      int i,j,ibig;
      T del,fp,fptt,t;
      int n=(int)get_size(p);
      array1d_type pt(n);
      array1d_type ptt(n);
      array1d_type xit(n);
      fret=p_fo->eval(p);

      for(j=0;j<n;++j)
	{
	  //get_element(pt,j)=get_element(p,j);
	  set_element(pt,j,get_element(p,j));
	}
      for(iter=0;!bstop;++iter)
	{
	  fp=fret;
	  ibig=0;
	  del=0.0;
	  for(i=0;i<n;++i)
	    {
#pragma omp parallel for 
	      for(j=0;j<n;++j)
		{
		  //get_element(xit,j)=xi[j][i];
		  set_element(xit,j,xi[j][i]);
		}
	      fptt=fret;
	      linmin(p,xit,fret,(*p_fo));
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
#pragma omp parallel for 
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
		  linmin(p,xit,fret,*p_fo);
#pragma omp parallel for 
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
    
    powell_method()
      :threshold(1e-4),xi(NULL_PTR),xi_1d(NULL_PTR)
    {}

    virtual ~powell_method()
    {
      clear_xi();
    };

    powell_method(const powell_method<rT,pT>& rhs)
      :opt_method<rT,pT>(rhs),p_fo(rhs.p_fo),p_optimizer(rhs.p_optimizer),
       start_point(rhs.start_point),
       end_point(rhs.end_point),
       ncom(rhs.ncom),
       threshold(rhs.threshold),xi(NULL_PTR),xi_1d(NULL_PTR)
    {
    }

    powell_method<rT,pT>& operator=(const powell_method<rT,pT>& rhs)
    {
      threshold=rhs.threshold;
      xi=0;
      xi_1d=0;
      p_fo=rhs.p_fo;
      p_optimizer=rhs.p_optimizer;
      start_point=rhs.start_point;
      end_point=rhs.end_point;
      ncom=rhs.ncom;
      threshold=rhs.threshold;
    }
    
    opt_method<rT,pT>* do_clone()const
    {
      return new powell_method<rT,pT>(*this);
    }
    
    void do_set_start_point(const array1d_type& p)
    {
      resize(start_point,get_size(p));
      opt_assign(start_point,p);
    }

    array1d_type do_get_start_point()const
    {
      return start_point;
    }

    void do_set_lower_limit(const array1d_type& p)
    {}

    void do_set_upper_limit(const array1d_type& p)
    {}

    void do_set_precision(rT t)
    {
      threshold=t;
    }

    rT do_get_precision()const
    {
      return threshold;
    }

    void do_set_optimizer(optimizer<rT,pT>& o)
    {
      p_optimizer=&o;
      p_fo=p_optimizer->ptr_func_obj();
    }
    
    
    
    pT do_optimize()
    {
      bstop=false;
      init_xi((int)get_size(start_point));


      for(int i=0;i<(int)get_size(start_point);++i)
	{
	  for(int j=0;j<(int)get_size(start_point);++j)
	    {
	      xi[i][j]=(i==j)?1:0;
	    }
	}
      
      int iter=100;
      opt_assign(end_point,start_point);
      rT fret;
      powell(end_point,threshold,iter,fret);
      return end_point;
    }
    
    void do_stop()
    {
      bstop=true;
    }

  };

}


#endif
//EOF
