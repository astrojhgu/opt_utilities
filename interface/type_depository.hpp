/**
   \file type_depository.hpp
   \brief depository and draw objects
   \author Junhua Gu
 */



#ifndef TYPE_DEPOSITORY_HPP
#define TYPE_DEPOSITORY_HPP
#include <string>
#include <utility>
#include <core/optimizer.hpp>
#include <core/fitter.hpp>
#include <map>
#include <iostream>
namespace opt_utilities
{

  enum fetch_direction
    {
      in,out
    };

  class type_unregistered
    :public opt_exception
  {
    const char* what()const throw()
    {
      return "type not registred";
    }
  };

  template <typename T>
  class holder
  {
  private:
    T* ptr;
  public:
    holder()
      :ptr(0)
    {}
    
    holder(T* p)
      :ptr(p)
    {}

    holder(const holder& rhs)
      :ptr(rhs.ptr)
    {
      const_cast<holder&>(rhs).ptr=0;
    }

    ~holder()
    {
      destroy();
    }

    holder& operator=(const holder& rhs)
    {
      if(this==&rhs)
	{
	  return *this;
	}
      destroy();
      ptr=rhs.ptr;
      const_cast<holder&>(rhs).ptr=0;
      return *this;
    }

  public:
    T* release()
    {
      T* p=ptr;
      ptr=0;
      return p;
    }

    void destroy()
    {
      if(ptr)
	{
	  ptr->destroy();
	}
    }

    T* get()const
    {
      return ptr;
    }

    void reset(T* p)
    {
      destroy();
      ptr=p;
    }

    operator T*()
    {
      return ptr;
    }

    operator const T*()const
    {
      return ptr;
    }
    
  public:
    T& operator*()
    {
      return *ptr;
    }

  public:
    T* operator->()
    {
      return ptr;
    }
  };

  
  template <typename Ty,typename Tx>
  void delete_clone(const func_obj<Ty,Tx>* pfo)
  {
    const_cast<func_obj<Ty,Tx>* >(pfo)->destroy();
  }

  template <typename Ty,typename Tx>
  void fetch_func_obj(const func_obj<Ty,Tx>* &fo,std::string cname,fetch_direction dir=in)
  {
    static std::map<std::string,holder<func_obj<Ty,Tx> > >pm;
    typename std::map<std::string,
      holder<func_obj<Ty,Tx> > >::iterator it=pm.find(cname);
    
    if(dir==out)
      {
	if(it==pm.end())
	  {
	    std::cerr<<cname<<std::endl;
	    throw type_unregistered();;
	  }
	else
	  {
	    func_obj<Ty,Tx>* result=it->second;
	    fo=result;
	  }
      }
    else if(dir==in)
      {
	//pm.insert(make_pair(cname,holder<func_obj<Ty,Tx> >(fo->clone())));
	
	pm[cname]=holder<func_obj<Ty,Tx> >(fo->clone());
      }
  }

  template <typename Ty,typename Tx>
  void register_func_obj(const func_obj<Ty,Tx>& fo)
  {
    const func_obj<Ty,Tx>* pfo=&fo;
    fetch_func_obj(pfo,fo.get_type_name(),in);
  }

  template <typename Ty,typename Tx>
  const func_obj<Ty,Tx>* get_func_obj(std::string cname)
  {
    const func_obj<Ty,Tx>* pom;
    fetch_func_obj(pom,cname,out);
    return pom;
  }


  template <typename Ty,typename Tx>
  void delete_clone(const opt_method<Ty,Tx>* pfo)
  {
    const_cast<opt_method<Ty,Tx>* >(pfo)->destroy();
  }

  template <typename Ty,typename Tx>
  void fetch_opt_method(const opt_method<Ty,Tx>* &fo,std::string cname,fetch_direction dir)
  {
    static std::map<std::string,holder<opt_method<Ty,Tx> > > pm;
    typename std::map<std::string,
      holder<opt_method<Ty,Tx> > >::iterator it=pm.find(cname);
    
    if(dir==out)
      {
	if(it==pm.end())
	  {
	    std::cerr<<cname<<std::endl;
	    throw type_unregistered();;
	  }
	else
	  {
	    opt_method<Ty,Tx>* result=it->second;
	    fo=result;
	  }
      }
    else if(dir==in)
      {
	//pm.insert(cname,fo->clone());
	pm[cname]=holder<opt_method<Ty,Tx> >(fo->clone());
      }
  }

  template <typename Ty,typename Tx>
  void register_opt_method(const opt_method<Ty,Tx>& fo)
  {
    const opt_method<Ty,Tx>* pfo=&fo;
    fetch_opt_method(pfo,fo.get_type_name(),in);
  }

  template <typename Ty,typename Tx>
  const opt_method<Ty,Tx>* get_opt_method(std::string cname)
  {
    const opt_method<Ty,Tx>* pom;
    fetch_opt_method(pom,cname,out);
    return pom;
  }

  template<typename Tdata,typename Tp,typename Ts,typename Tstr>
  void delete_clone(const statistic<Tdata,Tp,Ts,Tstr>* pfo)
  {
    const_cast<statistic<Tdata,Tp,Ts,Tstr>* >(pfo)->destroy();
  }


  template<typename Tdata,typename Tp,typename Ts,typename Tstr>
  void fetch_statistic(const statistic<Tdata,Tp,Ts,Tstr>* &fo,std::string cname,fetch_direction dir)
  {
    static std::map<std::string,holder<statistic<Tdata,Tp,Ts,Tstr> > > pm;
    typename std::map<std::string,
      holder<statistic<Tdata,Tp,Ts,Tstr> > >::iterator it=pm.find(cname);
    
    if(dir==out)
      {
	if(it==pm.end())
	  {
	    std::cerr<<cname<<std::endl;
	    throw type_unregistered();;
	  }
	else
	  {
	    statistic<Tdata,Tp,Ts,Tstr>* result=it->second;
	    fo=result;
	  }
      }
    else if(dir==in)
      {
	//pm.insert(cname,fo->clone());
	pm[cname]=holder<statistic<Tdata,Tp,Ts,Tstr> >(fo->clone());
      }
  }

  template<typename Tdata,typename Tp,typename Ts,typename Tstr>
  void register_statistic(const statistic<Tdata,Tp,Ts,Tstr>& fo)
  {
    const statistic<Tdata,Tp,Ts,Tstr>* pfo=&fo;
    fetch_statistic(pfo,fo.get_type_name(),in);
  }

  template<typename Tdata,typename Tp,typename Ts,typename Tstr>
  const statistic<Tdata,Tp,Ts,Tstr>* get_statistic(std::string cname)
  {
    const statistic<Tdata,Tp,Ts,Tstr>* pst;
    fetch_statistic(pst,cname,out);
    return pst;
  }

  template <typename Tdata,typename Tp,typename Tstr>
  void delete_clone(const param_modifier<Tdata,Tp,Tstr>* pfo)
  {
    const_cast<param_modifier<Tdata,Tp,Tstr>* >(pfo)->destroy();
  }

  template <typename Tdata,typename Tp,typename Tstr>
  void fetch_param_modifier(const param_modifier<Tdata,Tp,Tstr>* &fo,std::string cname,fetch_direction dir)
  {
    static std::map<std::string,holder<param_modifier<Tdata,Tp,Tstr> > > pm;
    typename std::map<std::string,
      holder<param_modifier<Tdata,Tp,Tstr> > >::iterator it=pm.find(cname);
    
    if(dir==out)
      {
	if(it==pm.end())
	  {
	    std::cerr<<cname<<std::endl;
	    throw type_unregistered();
	  }
	else
	  {
	    param_modifier<Tdata,Tp,Tstr>* result=it->second;
	    fo=result;
	  }
      }
    else if(dir==in)
      {
	//pm.insert(cname,fo->clone());
	pm[cname]=holder<param_modifier<Tdata,Tp,Tstr> >(fo->clone());
      }
  }

  template<typename Tdata,typename Tp,typename Tstr>
  void register_param_modifier(const param_modifier<Tdata,Tp,Tstr>& fo)
  {
    const param_modifier<Tdata,Tp,Tstr>* pfo=&fo;
    fetch_param_modifier(pfo,fo.get_type_name(),in);
  }

  template<typename Tdata,typename Tp,typename Tstr>
  const param_modifier<Tdata,Tp,Tstr>* get_param_modifier(std::string cname)
  {
    const param_modifier<Tdata,Tp,Tstr>* ppm;
    fetch_param_modifier(ppm,cname,out);
    return ppm;
  }
  
  
  template <typename Tdata>
  void delete_clone(const data_set<Tdata>* pfo)
  {
    const_cast<data_set<Tdata>* >(pfo)->destroy();
  }

  template <typename Tdata>
  void fetch_data_set(const data_set<Tdata>* &fo,std::string cname,fetch_direction dir)
  {
    static std::map<std::string,holder<data_set<Tdata> > > pm;
    typename std::map<std::string,
      holder<data_set<Tdata> > >::iterator it=pm.find(cname);
    
    if(dir==out)
      {
	if(it==pm.end())
	  {
	    std::cerr<<cname<<std::endl;
	    throw type_unregistered();;
	  }
	else
	  {
	    data_set<Tdata>* result=it->second;
	    fo=result;
	  }
      }
    else if(dir==in)
      {
	//pm.insert(cname,fo->clone());
	pm[cname]=holder<data_set<Tdata> >(fo->clone());
      }
  }

  template <typename Tdata>
  void register_data_set(const data_set<Tdata>& fo)
  {
    const data_set<Tdata>* pfo=&fo;
    fetch_data_set(pfo,fo.get_type_name(),in);
  }

  template<typename Tdata>
  const data_set<Tdata>* get_data_set(std::string cname)
  {
    const data_set<Tdata>* pds;
    fetch_data_set(pds,cname,out);
    return pds;
  }
  
  

  ////////////////////
  template <typename Tdata,typename Tp,typename Tstr>
  void delete_clone(const model<Tdata,Tp,Tstr>* pfo)
  {
    const_cast<model<Tdata,Tp,Tstr>* >(pfo)->destroy();
  }


  template <typename Tdata,typename Tp,typename Tstr>
  void fetch_model(const model<Tdata,Tp,Tstr>* &fo,std::string cname,fetch_direction dir)
  {
    static std::map<std::string,holder<model<Tdata,Tp,Tstr> > > pm;
    typename std::map<std::string,
      holder<model<Tdata,Tp,Tstr> > >::iterator it=pm.find(cname);
    
    if(dir==out)
      {
	if(it==pm.end())
	  {
	    std::cerr<<cname<<std::endl;
	    throw type_unregistered();
	  }
	else
	  {
	    model<Tdata,Tp,Tstr>* result=it->second;
	    fo=result;
	  }
      }
    else if(dir==in)
      {
	pm[cname]= holder<model<Tdata,Tp,Tstr> >(fo->clone());
      }
  }

  template <typename Tdata,typename Tp,typename Tstr>
  void register_model(const model<Tdata,Tp,Tstr>& fo)
  {
    const model<Tdata,Tp,Tstr>* pfo=&fo;
    fetch_model(pfo,fo.get_type_name(),in);
  }

  template <typename Tdata,typename Tp,typename Tstr>
  const model<Tdata,Tp,Tstr>* get_model(std::string cname)
  {
    const model<Tdata,Tp,Tstr>* pds;
    fetch_model(pds,cname,out);
    return pds;
  }
  
}


#endif
