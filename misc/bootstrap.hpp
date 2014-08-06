/**
   \file bootstrap.hpp
   \brief A simple implement of bootstrap method for error estimation
   \autho Junhua Gu
*/


#ifndef BOOT_STRAP
#define BOOT_STRAP
#define OPT_HEADER
#include <core/fitter.hpp>
#include <vector>
#include <list>
#include <cstdlib>
#include <iostream>
#include <utility>
#include <algorithm>
#include <cmath>
#include <data_sets/default_data_set.hpp>
using std::cerr;
namespace opt_utilities
{
  template <typename Ty>
  inline Ty rand_norm(Ty y0,Ty y_err)
  {
    Ty y;
    do
      {
	y=(rand()/(Ty)RAND_MAX-(Ty).5)*(10*y_err)+y0;
      }
    while(rand()/(Ty)RAND_MAX>std::exp(-(y-y0)*(y-y0)/(y_err*y_err)));
    return y;
  }


  /**
     \brief using bootstrap method to estimate confidence interval
     \tparam Ty the return type of a model
     \tparam Tx the type of self-var
     \tparam Tp the type of model parameters
     \tparam Ts the type of statistic
     \tparam Tstr the type of string used
   */
  template <typename Tdata,typename Tp,typename Ts,typename Tstr=std::string>
  class bootstrap
  {
  private:
    typedef typename Tdata::Ty Ty;
    typedef typename Tdata::Tx Tx;
  public:
    typedef typename std::list<Tp>::const_iterator param_iterator;
  public:
    std::list<Tp> param_pool;
    default_data_set<Tdata> current_data_set;
    default_data_set<Tdata> origin_data_set;
    fitter<Tdata,Tp,Ts,Tstr>* p_fitter;
    Tp origin_param;
    mutable bool bstop;
    
  public:

    /**
       default construct
     */
    bootstrap()
      :p_fitter(NULL)
    {}

    /**
       destructure
     */
    ~bootstrap()
    {
      reset();
    }


    /**
       set a fitter
       \param pf the fitter, the confidence interval of which will be estimated
     */
    void set_fitter(fitter<Tdata,Tp,Ts,Tstr>& pf)
    {
      param_pool.clear();
      p_fitter=&pf;
      origin_data_set=dynamic_cast<const default_data_set<Tdata>&>(pf.get_data_set());
      origin_param=pf.get_all_params();
    }
    
    /**
       reset the estimation
       reload the data
       restore the parameters
     */
    void reset()
    {
      if(p_fitter!=0)
	{
	  p_fitter->load_data(origin_data_set);
	  p_fitter->set_param_value(origin_param);
	}
      p_fitter=0;
    }


    /**
       resample n times
       \param n the times to sample
     */
    void sample(int n)
    {
      bstop=false;
      if(p_fitter!=NULL)
	{
	  for(int i=0;i<n&&!bstop;++i)
	    {
	      sample();
	    }
	  
	  p_fitter->load_data(origin_data_set);
	  p_fitter->set_param_value(origin_param);
	}
      else
	{
	  throw opt_exception("Fitter not_set");
	}
    }

    void stop()
    {
      bstop=true;
    }

    /**
       get the param of i-th sampling
       \param i the order of parameter
       \return the parameter
     */
    const Tp& get_param(int n)const
    {
      //return param_pool.at(i);
      int cnt=0;
      for(typename std::list<Tp>::const_iterator i=param_pool.begin();
	  i!=param_pool.end();++i)
	{
	  if(cnt==n)
	    {
	      return *i;
	    }
	  ++cnt;
	}
      throw opt_exception("excesses param_pool size");
      return *param_pool.begin();
    }

    typename std::list<Tp>::const_iterator 
    param_begin()const
    {
      return param_pool.begin();
    }

    typename std::list<Tp>::const_iterator 
    param_end()const
    {
      return param_pool.end();
    }
    

    int get_param_pool_size()const
    {
      return param_pool.size();
    }
    
  private:
    Tp sample()
    {
      if(p_fitter==NULL)
	{
	  throw fitter_not_set();
	}
      current_data_set=default_data_set<Tdata>();
      for(size_t i=0;i<origin_data_set.size();++i)
	{
	  Tdata d;
	  d=origin_data_set.get_data(i);
	  d.set_y(rand_norm(d.get_y(),(d.get_y_upper_err()+d.get_y_lower_err())/2.));
	  current_data_set.add_data(d);
	}
      p_fitter->load_data(current_data_set);
      p_fitter->set_param_value(origin_param);
      param_pool.push_back(p_fitter->fit());
#if 0
      for(size_t i=0;i<(param_pool.back()).size();++i)
	{
	  cerr<<param_pool.back()[i]<<",";
	}
      std::cerr<<std::endl;
#endif
      return param_pool.back();
    }
    
  public:

    /**
       Calculate the confdence interval for one parameter under given level
       \param param_name the name of the parameter to be estimated
       \param level the confidence level
       \return a std::pair containing the lower and upper boundaries of the confidence interval
     */
    std::pair<typename element_type_trait<Tp>::element_type,typename element_type_trait<Tp>::element_type>
    interval(const Tstr& param_name,double level)
    {
      if(p_fitter==NULL)
	{
	  throw opt_exception("Fitter not_set");
	}
      if(param_pool.empty())
	{
	  throw opt_exception("Bootstrap not done");
	}
      //sample();
      std::vector<typename element_type_trait<Tp>::element_type> _tmp;
      int order=p_fitter->get_param_order(param_name);
      for(typename std::list<Tp>::iterator i=param_pool.begin();
	  i!=param_pool.end();++i)
	{
	  _tmp.push_back((*i)[order]);
	}
      sort(_tmp.begin(),_tmp.end());
      std::pair<typename std::vector<typename element_type_trait<Tp>::element_type>::iterator,
	typename std::vector<typename element_type_trait<Tp>::element_type>::iterator> 
	itv=equal_range(_tmp.begin(),_tmp.end(),origin_param[order]);
      int current_param_position=itv.second-_tmp.begin();
      //std::cerr<<_tmp.size()<<std::endl;
      return std::pair<typename element_type_trait<Tp>::element_type,
	typename element_type_trait<Tp>::element_type>(
				 _tmp.at((int)((1-level)*current_param_position)),
				 _tmp.at((int)(current_param_position+level*(_tmp.size()-current_param_position)))
				 );
      
    }

  };
  
}

#endif
//EOF
