/**
   \file lbfgs_method.hpp
   \brief large bfgs method, used to perform large scale optimization
   \author Junhua Gu
 */

#ifndef LBFGS_METHOD
#define LBFGS_METHOD
#define OPT_HEADER
#include <core/optimizer.hpp>
//#include <blitz/array.h>
#include <limits>
#include <cstdlib>
#include <core/opt_traits.hpp>
#include "../linmin/linmin.hpp"
#include <math/num_diff.hpp>
#include <cassert>
#include <cmath>
#include <ctime>
#include <vector>
#include <algorithm>
#include "lbfgs.h"
#include "lbfgs.cpp"
/*
 *
*/
#include <iostream>
using std::cerr;
using std::endl;

namespace opt_utilities
{

  template<typename rT,typename pT>
  lbfgsfloatval_t lbfgs_adapter(
				void *instance,
				const lbfgsfloatval_t *x,
				lbfgsfloatval_t *g,
				const int n,
				const lbfgsfloatval_t step
				)
  {
    pT px;
    resize(px,n);
    for(int i=0;i<n;++i)
      {
	set_element(px,i,x[i]);
      }
    
    lbfgsfloatval_t result=((func_obj<rT,pT>*)instance)->eval(px);
    pT grad(gradient(*static_cast<func_obj<rT,pT>*>(instance),px));
    for(int i=0;i<n;++i)
      {
	g[i]=get_element(grad,i);
      }
    return result;
  }


  
  template <typename rT,typename pT>
  class lbfgs_method
    :public opt_method<rT,pT>
  {
  public:
    typedef pT array1d_type;
    typedef rT T;
  private:
    func_obj<rT,pT>* p_fo;
    optimizer<rT,pT>* p_optimizer;
    
    //typedef blitz::Array<rT,2> array2d_type;
    
    
  private:
    array1d_type start_point;
    array1d_type end_point;
    
  private:
    rT threshold;
  private:
    rT func(const pT& x)
    {
      assert(p_fo!=0);
      return p_fo->eval(x);
    }

    const char* do_get_type_name()const
    {
      return "large scale bfgs";
    }
  public:
    lbfgs_method()
      :threshold(1e-4)
    {}

    virtual ~lbfgs_method()
    {
    };

    lbfgs_method(const lbfgs_method<rT,pT>& rhs)
      :p_fo(rhs.p_fo),p_optimizer(rhs.p_optimizer),
       start_point(rhs.start_point),
       end_point(rhs.end_point),
       threshold(rhs.threshold)
    {
    }

    lbfgs_method<rT,pT>& operator=(const lbfgs_method<rT,pT>& rhs)
    {
      threshold=rhs.threshold;
      p_fo=rhs.p_fo;
      p_optimizer=rhs.p_optimizer;
      opt_assign(start_point,rhs.start_point);
      opt_assign(end_point,rhs.end_point);
    }
    
    opt_method<rT,pT>* do_clone()const
    {
      return new lbfgs_method<rT,pT>(*this);
    }
    
    void do_set_start_point(const array1d_type& p)
    {
      start_point.resize(get_size(p));
      opt_assign(start_point,p);
      
    }

    array1d_type do_get_start_point()const
    {
      return start_point;
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
    
    
    
    pT do_optimize()
    {
      lbfgs_parameter_t param;
      lbfgs_parameter_init(&param);
      param.ftol=threshold;
      std::vector<lbfgsfloatval_t> buffer(get_size(start_point));
      for(int i=0;i<buffer.size();++i)
	{
	  buffer[i]=get_element(start_point,i);
	}
      lbfgsfloatval_t fx;
      lbfgs(get_size(start_point),&buffer[0],&fx,
	    lbfgs_adapter<rT,pT>,0,p_fo,&param);
      for(int i=0;i<buffer.size();++i)
	{
	  set_element(start_point,i,buffer[i]);
	}
      return start_point;
    } 
  };
  
}


#endif
//EOF
