/**
   \file aga.hpp
   \brief asexual genetic algorithm method
   \author Junhua Gu
*/

#ifndef AGA_METHOD
#define AGA_METHOD
#define OPT_HEADER
#include <core/optimizer.hpp>
//#include <blitz/array.h>
#include <limits>
#include <cstdlib>
#include <core/opt_traits.hpp>
#include <cassert>
#include <cmath>
#include <ctime>
#include <vector>
#include <algorithm>
/*
 *
*/
#include <iostream>
using std::cout;
using std::endl;

namespace opt_utilities
{

  template <typename rT,typename pT>
  struct vp_pair
  {
    rT v;
    pT p;
  };
  
  template <typename rT,typename pT>
  class vp_comp
  {
  public:
    bool operator()(const vp_pair<rT,pT>& x1,
		    const vp_pair<rT,pT>& x2)
    {
      return x1.v<x2.v;
    }
  };


  /**
     \brief Implement of the asexual genetic algorithm
     2009A&A...501.1259C
     http://adsabs.harvard.edu/abs/2009A%26A...501.1259C
     \tparam rT return type of the object function
     \tparam pT parameter type of the object function
   */
  template <typename rT,typename pT>
  class aga_method
    :public opt_method<rT,pT>
  {
  public:
    typedef pT array1d_type;
  private:
    int n1,n2,n0;
    func_obj<rT,pT>* p_fo;
    optimizer<rT,pT>* p_optimizer;
    rT threshold;
    pT lower_bound;
    pT upper_bound;
    
    typename element_type_trait<pT>::element_type decay_factor;
    pT reproduction_box;
    std::vector<vp_pair<rT,pT> > samples;
    std::vector<pT> buffer;
    mutable bool bstop;
  private:
    typename element_type_trait<pT>::element_type uni_rand
    (typename element_type_trait<pT>::element_type x1,
     typename element_type_trait<pT>::element_type x2)
    {
      return rand()/(double)RAND_MAX*(x2-x1)+x1;
    }
    
  private:
    const char* do_get_type_name()const
    {
      return "asexual genetic algorithm";
    }
    
    rT func(const pT& x)
    {
      assert(p_fo!=0);
      return p_fo->eval(x);
    }

  public:
    aga_method(int _n1,int _n2)
      :n1(_n1),n2(_n2),n0(n1*n2+n1),
       p_fo(0),p_optimizer(0),threshold(1e-4),
       decay_factor(.999),
       samples(n1*n2+n1)      
    {
    }
    
    aga_method()
      :n1(50),n2(20),n0(n1*n2+n1),
       p_fo(0),p_optimizer(0),threshold(1e-4),
       decay_factor(.999),
       samples(n1*n2+n1)      
    {
    }
    

    virtual ~aga_method()
    {     
    };
    
    aga_method(const aga_method<rT,pT>& rhs)
      :n1(rhs.n1),n2(rhs.n2),n0(rhs.n0),
       p_fo(rhs.p_fo),p_optimizer(rhs.p_optimizer),
       threshold(rhs.threshold),
       decay_factor(rhs.decay_factor),
       samples(rhs.samples)
    {
    }

    aga_method<rT,pT>& operator=(const aga_method<rT,pT>& rhs)
    {
      threshold=rhs.threshold;
      p_fo=rhs.p_fo;
      p_optimizer=rhs.p_optimizer;
      samples=rhs.samples;
      n1=rhs.n1;
      n2=rhs.n2;
      n0=rhs.n0;
    }

    void set_decay_factor(typename element_type_trait<pT>::element_type _decay_factor)
    {
      decay_factor=_decay_factor;
    }

    
    opt_method<rT,pT>* do_clone()const
    {
      return new aga_method<rT,pT>(*this);
    }
    
    void do_set_start_point(const array1d_type& p)
    {
      for(size_t i=0;i<samples.size();++i)
	{
	  //  cout<<i<<" ";
	  resize(samples[i].p,get_size(p));
	  //	  std::cout<<samples[i].p.size()<<std::endl;;
	  for(size_t j=0;j<get_size(p);++j)
	    {
	      set_element(samples[i].p,j,
			  uni_rand(get_element(lower_bound,j),
				   get_element(upper_bound,j))
			  );
	    }
	}
      
    }

    array1d_type do_get_start_point()const
    {
      return array1d_type();
    }
    
    void do_set_lower_limit(const array1d_type& p)
    {
      opt_eq(lower_bound,p);
    }

    array1d_type do_get_lower_limit()const
    {
      return lower_bound;
    }
    
    void do_set_upper_limit(const array1d_type& p)
    {
      opt_eq(upper_bound,p);
    }

    array1d_type do_get_upper_limit()const
    {
      return upper_bound;
    }
    

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
    
    bool iter()
    {
      rT sum2=0;
      rT sum=0;
      for(size_t i=0;i<samples.size();++i)
	{
	  samples[i].v=func(samples[i].p);
	  sum2+=samples[i].v*samples[i].v;
	  sum+=samples[i].v;
	}
      
      std::sort(samples.begin(),samples.end(),vp_comp<rT,pT>());
      if(sum2/samples.size()-pow(sum/samples.size(),2)<threshold)
	{
	  return false;
	}
      pT lb(get_size(samples[0].p));
      pT ub(get_size(samples[0].p));
      for(int i=0;i<n2&&!bstop;++i)
	{
	  pT p(samples[i].p);
	  for(size_t j=0;j<get_size(p);++j)
	    {
	      if(i==0)
		{
		  ub[j]=p[j];
		  lb[j]=p[j];
		}
	      ub[j]=std::max(ub[j],p[j]);
	      lb[j]=std::min(lb[j],p[j]);
	      
	      set_element(p,j,
			  get_element(p,j)+
			  uni_rand(-get_element(reproduction_box,j),
				   get_element(reproduction_box,j)));
	      if(get_element(p,j)>get_element(upper_bound,j))
		{
		  set_element(p,j,get_element(upper_bound,j));
		}
	      if(get_element(p,j)<get_element(lower_bound,j))
		{
		  set_element(p,j,get_element(lower_bound,j));
		}
	    }
	  buffer[i]=p;
	}
      if(bstop)
	{
	  return false;
	}
      for(int i=0;i<n1&&!bstop;++i)
	{
	  for(int j=0;j<n2&&!bstop;++j)
	    {
	      pT p(samples[i].p);
	      for(size_t k=0;k<get_size(p);++k)
		{
		  set_element(samples[i*n2+j+n1].p,k,
			      (get_element(samples[i].p,k)+
			       get_element(buffer[j],k))/2.);
		  

		  ub[k]=std::max(ub[k],samples[i*n2+j+n1].p[k]);
		  lb[k]=std::min(lb[k],samples[i*n2+j+n1].p[k]);
		}
	    }
	}
      if(bstop)
	{
	  return false;
	}
      double n_per_dim=pow((double)n0,1./get_size(lower_bound));
      for(size_t i=0;i<get_size(reproduction_box);++i)
	{
	  //	  set_element(reproduction_box,i,
	  //get_element(reproduction_box,i)*decay_factor);
	  set_element(reproduction_box,i,
		      (get_element(ub,i)-
		       get_element(ub,i))/n_per_dim);
	  
	}
      return true;
    }
      
    pT do_optimize()
    {
      bstop=false;
      srand(time(0));
      buffer.resize(n2);
      double n_per_dim=pow((double)n0,1./get_size(lower_bound));
      resize(reproduction_box,get_size(lower_bound));
      
      for(size_t i=0;i<get_size(lower_bound);++i)
	{
	  
	  set_element(reproduction_box,i,
		      (get_element(upper_bound,i)-
		       get_element(lower_bound,i))/n_per_dim);
	}
      
      while(iter()&&!bstop){}
      
      return samples.begin()->p;
    }
    
    void do_stop()
    {
      bstop=true;
    }

  };

}


#endif
//EOF
