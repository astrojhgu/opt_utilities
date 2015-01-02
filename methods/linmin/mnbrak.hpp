#ifndef MNBRAK_HPP
#define MNBRAK_HPP 
#define OPT_HEADER
//#include "optimizer.hpp"
#include "bas_util.hpp"
namespace opt_utilities
{

  
  template <typename T>
  void mnbrak(T& ax,T& bx,T& cx,T& fa,T& fb,T& fc,func_obj<T,T>& func)
  {
    const T GOLD=1.618034;
    const T GLIMIT=100;
    const T TINY=std::numeric_limits<T>::epsilon();
    T ulim,u,r,q,fu;
    fa=func.eval(ax);
    fb=func.eval(bx);
    
    if(fb>fa)
      {
	//shft(dum,ax,bx,dum);
	//shft(dum,fb,fa,dum);
	std::swap(ax,bx);
	std::swap(fa,fb);
      }

    cx=bx+GOLD*(bx-ax);
    fc=func.eval(cx);
    while(fb>fc)
      {
	r=(bx-ax)*(fb-fc);
	q=(bx-cx)*(fb-fa);
	u=bx-T((bx-cx)*q-(bx-ax)*r)/
	  T(T(2.)*sign(T(tmax(T(tabs(T(q-r))),T(TINY))),T(q-r)));
	ulim=bx+GLIMIT*(cx-bx);
	if((bx-u)*(u-cx)>0.)
	  {
	    fu=func.eval(u);
	    if(fu<fc)
	      {
		ax=bx;
		bx=u;
		fa=fb;
		fb=fu;
		return;
	      }
	    else if(fu>fb)
	      {
		cx=u;
		fc=fu;
		return;
	      }
	    u=cx+GOLD*(cx-bx);
	    fu=func.eval(u);
	  }
	else if((cx-u)*(u-ulim)>0.)
	  {
	    fu=func.eval(u);
	    if(fu<fc)
	      {
		shft3(bx,cx,u,T(cx+GOLD*(cx-bx)));
		shft3(fb,fc,fu,func.eval(u));
	      }
	  }
	else if((u-ulim)*(ulim-cx)>=0)
	  {
	    u=ulim;
	    fu=func.eval(u);
	  }
	else
	  {
	    u=cx+GOLD*(cx-bx);
	    fu=func.eval(u);
	  }
	shft3(ax,bx,cx,u);
	shft3(fa,fb,fc,fu);
      }
  }
}

#endif
