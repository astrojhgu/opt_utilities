/**
   \file leastsq.hpp
   \brief least square statistic
   \author Junhua Gu
 */

#ifndef LEAST_SQ_HPP
#define LEAST_SQ_HPP
#define OPT_HEADER
#include <core/fitter.hpp>
#include <misc/optvec.hpp>
#include <iostream>
#include <vector>
#include <cmath>
using std::cerr;using std::endl;

namespace opt_utilities
{

   /**
      \brief least-square statistic
      \tparam Ty the return type of model
      \tparam Tx the type of the self-var
      \tparam Tp the type of model parameter
      \tparam Ts the type of the statistic
      \tparam Tstr the type of the string used
   */
  template<typename Tdata,typename Tp,typename Ts,typename Tstr>
  class leastsq
    :public statistic<Tdata,Tp,Ts,Tstr>
  {
  private:
    bool verb;
    bool limit_considered;
    int n;
    typedef typename Tdata::Ty Ty;
    typedef typename Tdata::Tx Tx;
    
    statistic<Tdata,Tp,Ts,Tstr>* do_clone()const
    {
      // return const_cast<statistic<Ty,Tx,Tp>*>(this);
      return new leastsq<Tdata,Tp,Ts,Tstr>(*this);
    }

    const char* do_get_type_name()const
    {
      return "least square statistic";
    }
  public:
    void verbose(bool v)
    {
      verb=v;
    }

  public:
    void consider_limit()
    {
      limit_considered=true;
    }
    
    void ignore_limit()
    {
      limit_considered=false;
    }
  public:
    leastsq()
      :verb(false),limit_considered(false)
    {}
    
    

    Ts do_eval(const Tp& p)
    {
      if(limit_considered)
	{
	  if(!this->get_fitter().get_model().meets_constraint(p))
	    {
	      return 1e99;
	    }
	}

      
      Ts result(0);
      for(int i=(this->get_data_set()).size()-1;i>=0;--i)
	{
	  Ty chi=(this->get_data_set().get_data(i).get_y()-this->eval_model(this->get_data_set().get_data(i).get_x(),p));
	  result+=chi*chi;

	}
      if(verb)
	{
	  n++;
	  if(n%10==0)
	    {

	      cerr<<result<<"\t";
	      for(size_t i=0;i<get_size(p);++i)
		{
		  cerr<<get_element(p,i)<<",";
		}
	      cerr<<endl;
	    }

	}
      
      return result;
    }
  };

  template<typename T,typename Ts,typename Tstr>
  class leastsq<data<optvec<T>,optvec<T> >,optvec<T>,Ts,Tstr>
    :public statistic<data<optvec<T>,optvec<T> >,optvec<T>,Ts,Tstr>
  {
  private:
    bool verb;
    bool limit_considered;
    int n;
    
    typedef optvec<T> Tx;
    typedef optvec<T> Ty;
    typedef optvec<T> Tp;
    statistic<data<optvec<T>,optvec<T> >,Tp,Ts,Tstr>* do_clone()const
    {
      // return const_cast<statistic<Ty,Tx,Tp>*>(this);
      return new leastsq<data<optvec<T>,optvec<T> >,Tp,Ts,Tstr>(*this);
    }

    const char* do_get_type_name()const
    {
      return "least square statistic";
    }
    
  public:
    void verbose(bool v)
    {
      verb=v;
    }
    
    void consider_limit()
    {
      limit_considered=true;
    }
    
    void ignore_limit()
    {
      limit_considered=false;
    }

  public:
    leastsq()
      :verb(false),limit_considered(false)
    {}
    
    

    Ts do_eval(const Tp& p)
    {
      if(limit_considered)
	{
	  if(!this->get_fitter().get_model().meets_constraint(p))
	    {
	      return 1e99;
	    }
	}
      
      Ts result(0);
      for(int i=(this->get_data_set()).size()-1;i>=0;--i)
	{
	  Ty chi(this->get_data_set().get_data(0).get_y().size());
	  for(int j=0;j<chi.size();++j)
	    {
	      Ty model_y(this->eval_model(this->get_data_set().get_data(i).get_x(),p));
	      if(model_y[j]>this->get_data_set().get_data(i).get_y()[j])
		{
		  chi[j]=(this->get_data_set().get_data(i).get_y()[j]-model_y[j]);
		}
	      else
		{
		  chi[j]=(this->get_data_set().get_data(i).get_y()[j]-model_y[j]);
		}
	    }
	  result+=sum(chi*chi);
	  
	}
      return result;
    }
  };
  

  
}



#endif
//EOF


