/**
   \file chisq.hpp
   \brief chi-square statistic
   \author Junhua Gu
 */

#ifndef CHI_SQ_HPP
#define CHI_SQ_HPP
#define OPT_HEADER
#include <core/fitter.hpp>
#include <iostream>
#include <vector>
#include <misc/optvec.hpp>
#include <cmath>
using std::cerr;using std::endl;

namespace opt_utilities
{

  /**
     \brief chi-square statistic
     \tparam Ty the return type of model
     \tparam Tx the type of the self-var
     \tparam Tp the type of model parameter
     \tparam Ts the type of the statistic
     \tparam Tstr the type of the string used
   */
  template<typename Tdata,typename Tp,typename Ts,typename Tstr>
  class chisq
    :public statistic<Tdata,Tp,Ts,Tstr>
  {
  public:
    typedef typename Tdata::Tx Tx;
    typedef typename Tdata::Ty Ty;
  private:
    bool verb;
    int n;
    
    
    statistic<Tdata,Tp,Ts,Tstr>* do_clone()const
    {
      // return const_cast<statistic<Ty,Tx,Tp>*>(this);
      return new chisq<Tdata,Tp,Ts,Tstr>(*this);
    }

    const char* do_get_type_name()const
    {
      return "chi^2 statistic";
    }
    
  public:
    void verbose(bool v)
    {
      verb=v;
    }
  public:
    chisq()
      :verb(false)
    {}
    
    

    Ts do_eval(const Tp& p)
    {
      Ts result(0);
      for(int i=(this->get_data_set()).size()-1;i>=0;--i)
	{
	  Ty chi=(this->get_data_set().get_data(i).get_y()-this->eval_model(this->get_data_set().get_data(i).get_x(),p))/this->get_data_set().get_data(i).get_y_upper_err();
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

#if 1

  template<>
  class chisq<data<double,double>,std::vector<double>,double,std::string>
    :public statistic<data<double,double>,std::vector<double> ,double,std::string>
  {
  public:
    typedef double Ty;
    typedef double Tx;
    typedef std::vector<double> Tp;
    typedef double Ts;
    typedef std::string Tstr;
    typedef data<double,double> Tdata;
  private:
    bool verb;
    int n;
    
    statistic<Tdata,Tp,Ts,Tstr>* do_clone()const
    {
      // return const_cast<statistic<Ty,Tx,Tp>*>(this);
      return new chisq<Tdata,Tp,Ts,Tstr>(*this);
    }

    const char* do_get_type_name()const
    {
      return "chi^2 statistics (specialized for double)";
    }
  public:
    void verbose(bool v)
    {
      verb=v;
    }
  public:
    chisq()
      :verb(false)
    {}
    
    

    Ty do_eval(const Tp& p)
    {
      Ty result(0);
      for(int i=(this->get_data_set()).size()-1;i>=0;--i)
	{

#ifdef HAVE_X_ERROR
	  Tx x1=this->get_data_set().get_data(i).get_x()-this->get_data_set().get_data(i).get_x_lower_err();
	  Tx x2=this->get_data_set().get_data(i).get_x()+this->get_data_set().get_data(i).get_x_upper_err();
	  Tx x=this->get_data_set().get_data(i).get_x();
	  Ty errx1=(this->eval_model(x1,p)-this->eval_model(x,p));
	  Ty errx2=(this->eval_model(x2,p)-this->eval_model(x,p));
	  //Ty errx=0;
#else
	  Ty errx1=0;
	  Ty errx2=0;
#endif

	  Ty y_model=this->eval_model(this->get_data_set().get_data(i).get_x(),p);
	  Ty y_obs=this->get_data_set().get_data(i).get_y();
	  Ty y_err;
	  
	  Ty errx=0;
	  if(errx1<errx2)
	    {
	      if(y_obs<y_model)
		{
		  errx=errx1>0?errx1:-errx1;
		}
	      else
		{
		  errx=errx2>0?errx2:-errx2;
		}
	    }
	  else
	    {
	      if(y_obs<y_model)
		{
		  errx=errx2>0?errx2:-errx2;
		}
	      else
		{
		  errx=errx1>0?errx1:-errx1;
		}
	    }


	  if(y_model>y_obs)
	    {
	      y_err=this->get_data_set().get_data(i).get_y_upper_err();
	    }
	  else
	    {
	      y_err=this->get_data_set().get_data(i).get_y_lower_err();
	    }

	  Ty chi=(y_obs-y_model)/std::sqrt(y_err*y_err+errx*errx);

	  //	  Ty chi=(this->get_data_set().get_data(i).get_y()-this->eval_model(this->get_data_set().get_data(i).get_x(),p));
	  //	  cerr<<chi<<"\n";
	  result+=chi*chi;
	  //std::cerr<<chi<<std::endl;
	  //cerr<<this->eval_model(this->get_data_set()[i].x,p)<<endl;
	  //cerr<<this->get_data_set()[i].y_upper_err<<endl;
	  //	  cerr<<this->get_data_set()[i].x<<"\t"<<this->get_data_set()[i].y<<"\t"<<this->eval_model(this->get_data_set()[i].x,p)<<endl;
	}
      if(verb)
	{
	  n++;
	  if(n%10==0)
	    {
	      cerr<<result<<"\t";
	      for(int i=0;i<(int)get_size(p);++i)
		{
		  cerr<<get_element(p,i)<<",";
		}
	      cerr<<endl;
	    }

	}
      
      return result;
    }
  };
#endif

  template<typename T,typename Ts,typename Tstr>
  class chisq<data<optvec<T>,optvec<T> >,optvec<T>,Ts,Tstr>
    :public statistic<data<optvec<T>,optvec<T> >,optvec<T>,Ts,Tstr>
  {
  private:
    bool verb;
    int n;
    
    typedef optvec<T> Tx;
    typedef optvec<T> Ty;
    typedef optvec<T> Tp;
    typedef data<optvec<T>,optvec<T> > Tdata;
    statistic<Tdata,Tp,Ts,Tstr>* do_clone()const
    {
      // return const_cast<statistic<Ty,Tx,Tp>*>(this);
      return new chisq<Tdata,Tp,Ts,Tstr>(*this);
    }

    const char* do_get_type_name()const
    {
      return "chi^2 statistic";
    }
    
  public:
    void verbose(bool v)
    {
      verb=v;
    }
  public:
    chisq()
      :verb(false)
    {}
    
    

    Ts do_eval(const Tp& p)
    {
      Ts result(0);
      for(int i=(this->get_data_set()).size()-1;i>=0;--i)
	{
	  Ty chi(this->get_data_set().get_data(0).get_y().size());
	  for(int j=0;j<chi.size();++j)
	    {
	      Ty model_y(this->eval_model(this->get_data_set().get_data(i).get_x(),p));
	      if(model_y[j]>this->get_data_set().get_data(i).get_y()[j])
		{
		  chi[j]=(this->get_data_set().get_data(i).get_y()[j]-model_y[j])/this->get_data_set().get_data(i).get_y_upper_err()[j];
		}
	      else
		{
		  chi[j]=(this->get_data_set().get_data(i).get_y()[j]-model_y[j])/this->get_data_set().get_data(i).get_y_lower_err()[j];
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


