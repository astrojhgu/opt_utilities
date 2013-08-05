/**
   \file pow_model.hpp
   \brief combing two model by power operation
   \author Junhua Gu
 */



#ifndef POW_MODEL_H_
#define POW_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <core/opt_traits.hpp>
#include <cmath>

namespace opt_utilities
{
  template <typename Tdata,typename Tp,typename Tstr>
  class pow_model
    :public model<Tdata,Tp,Tstr>
  {
  public:
    typedef typename Tdata::Ty Ty;
    typedef typename Tdata::Tx Tx;
  private:
    model<Tdata,Tp,Tstr>* do_clone()const
    {
      return new pow_model<Tdata,Tp,Tstr>(*this);
    }
  private:
    pow_model()
    {
    }

    const char* do_get_type_name()const
    {
      return "combine two models by power operation";
    }
  private:
    model<Tdata,Tp,Tstr>* pm1;
    typename element_type_trait<Tp>::element_type idx;

  public:
    pow_model(const model<Tdata,Tp,Tstr>& m1,
	      const typename element_type_trait<Tp>::element_type& index)
      :pm1(m1.clone()),idx(index)
    {
      int np1=m1.get_num_params();
      
      for(int i=0;i<np1;++i)
	{
	  param_info<Tp,Tstr> p(m1.get_param_info(i));
	  //param_info<Tp,Tstr> p1(p.get_name(),p.get_value());
	  this->push_param_info(p);
	}
    }

    pow_model(const pow_model& rhs)
      :pm1(NULL),idx(0)
    {
      int np1(0);
      if(rhs.pm1)
	{
	  pm1=rhs.pm1->clone();
	  np1=rhs.pm1->get_num_params();
	  for(int i=0;i<np1;++i)
	    {
	      param_info<Tp,Tstr> p(rhs.pm1->get_param_info(i));
	      param_info<Tp,Tstr> p1(p.get_name()+"1",p.get_value());
	      this->push_param_info(p1);
	    }
	}
      idx=rhs.idx;
    }

    pow_model& operator=(const pow_model& rhs)
    {
      if(this==&rhs)
	{
	  return *this;
	}
      if(!pm1)
	{
	  //delete pm1;
	  pm1->destroy();
	}

      int np1(0);
      if(rhs.pm1)
	{
	  pm1=rhs.pm1->clone();
	  np1=rhs.pm1->get_num_params();
	  for(int i=0;i<np1;++i)
	    {
	      param_info<Tp,Tstr> p(rhs.pm1->get_param_info(i));
	      // param_info<Tp,Tstr> p1(p.get_name()+"1",p.get_value());
	      this->push_param_info(p);
	    }
	}
      idx=rhs.idx;

      return *this;
    }

    ~pow_model()
    {
      if(!pm1)
	{
	  //delete pm1;
	  pm1->destroy();
	}
    }

  public:
    Ty do_eval(const Tx& x,const Tp& param)
    {
      if(!pm1)
	{
	  throw opt_exception("incomplete model!");
	}

      return std::pow(pm1->eval(x,param),idx);
    }
  };
  
  template <typename Tdata,typename Tp,typename Tstr>
  pow_model<Tdata,Tp,Tstr> pow(const model<Tdata,Tp,Tstr>& m1,
			       const typename element_type_trait<Tp>::
			       element_type& idx)
  {
    return pow_model<Tdata,Tp,Tstr>(m1,idx);
  }
}



#endif
//EOF
