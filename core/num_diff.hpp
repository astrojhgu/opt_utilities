#ifndef NUMDIFF_HPP
#define NUMDIFF_HPP

#include <core/optimizer.hpp>
#include <core/opt_traits.hpp>
#include <algorithm>
#include <limits>
#include <cmath>

namespace opt_utilities
{
  template <typename rT,typename pT>
  class dfunc_obj
    :public func_obj<rT,pT>
  {
  private:
    virtual pT do_diff(const pT& p)=0;
    
  public:
    pT diff(const pT& p)
    {
      return do_diff(p);
    }
  };
  
  class underivable
    :public opt_exception
  {
  public:
    underivable()
      :opt_exception("underivable")
    {}
  };

  template <typename rT,typename pT>
    pT numdiff(func_obj<rT,pT>& f,const pT& p)
  {
    rT ep=std::sqrt(std::numeric_limits<rT>::epsilon());
    
    pT result;
    resize(result,get_size(p));
    pT p2;
    resize(p2,get_size(p));
    pT p1;
    resize(p1,get_size(p));
    
    for(size_t i=0;i<get_size(p);++i)
      {
	set_element(p2,i,get_element(p,i));
	set_element(p1,i,get_element(p,i));
      }
    for(size_t i=0;i<get_size(p);++i)
      {
	typename element_type_trait<pT>::element_type h=
	  std::max(get_element(p,i),rT(1))*ep;
	  
	set_element(p2,i,get_element(p,i)+h);
	set_element(p1,i,get_element(p,i)-h);
	
	rT v2=f(p2);
	rT v1=f(p1);
	set_element(result,i,
		    (v2-v1)/h/2
		    );
	set_element(p2,i,get_element(p,i));
	set_element(p1,i,get_element(p,i));
      }
    return result;
  }

  template <typename rT,typename pT>
    pT diff(func_obj<rT,pT>& f,const pT& p)
  {
    dfunc_obj<rT,pT>* pdf=dynamic_cast<dfunc_obj<rT,pT>*>(&f);
    if(pdf)
      {
	return pdf->diff(p);
      }
    return numdiff(f,p);
    
  }
}

#endif