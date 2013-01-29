/**
   \file optimizer.hpp
   \brief the definition of classes optimizer, func_obj, and opt_method
   \author Junhua Gu
 */


#ifndef OPTIMZER_HPP
#define OPTIMZER_HPP
#define OPT_HEADER
#include <cstddef>
#include "opt_traits.hpp"
#include "opt_exception.hpp"
#include <cstdlib>
#include <functional>
#include <typeinfo>
#ifdef DEBUG
#include <iostream>
using namespace std;
#endif

/**
   The root namespace of the opt_utilities library
 */
namespace opt_utilities
{
  /////////NULL_PTR//////////////////////////////////////////
#if __cplusplus<201103L
#define NULL_PTR 0
#else
  const std::nullptr_t NULL_PTR=nullptr;
#endif

  /////////Forward declare///////////////////////////////////
  template <typename rT,typename pT>
  class optimizer;
  
  template <typename rT,typename pT>
  class func_obj;
  
  template <typename rT,typename pT>
  class opt_method;

  /**
     \brief Virtual class representing an object function.
     \tparam rT the return type
     \tparam pT the self-varible type
   */
  template <typename rT,typename pT>
  class func_obj
    :public std::unary_function<pT,rT>
  {
  private:
    /**
      \param x the self-varible
      \return the value evaluated
     */
    virtual rT do_eval(const pT& x)=0;
    /**
       \return the clone of an object.
     */
    virtual func_obj<rT,pT>* do_clone()const=0;
    /**
       Destroy the object generated by clone function
     */
    virtual void do_destroy()
    {
      delete this;
    }

    /**
       \return the type name of self
     */

    virtual const char* do_get_type_name()const
    {
      return typeid(*this).name();
    }

  public:
    /**
       Interface function to perform the clone
       \return the clone of an pre-existed object.
     */
    func_obj<rT,pT>* clone()const
    {
      return do_clone();
    }

    /**
       Interface function to perform the destroy.
     */
    void destroy()
    {
      do_destroy();
    }

    const char* get_type_name()const
    {
      return this->do_get_type_name();
    }

    /**
       Makes the class object like a real function.
     */
    rT operator()(const pT& p)
    {
      return do_eval(p);
    }
    

    /**
       The same as operator().
     */
    rT eval(const pT& p)
    {
      return do_eval(p);
    };

    /**
       deconstruct function
     */
    virtual ~func_obj(){};
    //    virtual XT walk(XT,YT)=0;
  };
  

  /**
     \brief virtual class representing optimization methods
     \tparam rT the return type
     \tparam pT the self-varible type
   */
  template <typename rT,typename pT>
  class opt_method
  {
  private:
    /**
       Set the optimizer
       \param op optimizer to be set
     */
    virtual void do_set_optimizer(optimizer<rT,pT>& op)=0;
    /**
       Set the precision
       \param th threshold
     */
    virtual void do_set_precision(rT th)=0;

    /**
       get the precision
       \return threshold
     */
    virtual rT do_get_precision()const=0;
    /**
       perform the optimization
       \return final optimized parameter.
     */
    virtual pT do_optimize()=0;
    /**
       \param p start point
     */
    virtual void do_set_start_point(const pT& p)=0;
    /**
       \param p the lower limit
     */
    virtual void do_set_lower_limit(const pT& p){};
    /**
       \param p the upper limit
     */
    virtual void do_set_upper_limit(const pT& p){};

    /**
       \return start point
     */
    virtual pT do_get_start_point()const=0;
    /**
       \return the lower limit
     */
    virtual pT do_get_lower_limit()const
    {
      return pT();
    };
    /**
       \return the upper limit
     */
    virtual pT do_get_upper_limit()const
    {
      return pT();
    };
    /**
       \return the clone of current object
     */
    virtual opt_method<rT,pT>* do_clone()const=0;
    /**
       destroy the object created by clone()
     */
    virtual void do_destroy()
    {
      delete this;
    }
    
    /**
       \return the type name of self
    */
    
    virtual const char* do_get_type_name()const
    {
      return typeid(*this).name();
    }

    /**
       interrupting the optimization
     */
    virtual void do_stop()
    {
    }
  public:
    /**
       Interface function for seting optimizer
       \param op optimizer to be set
     */
    void  set_optimizer(optimizer<rT,pT>& op)
      {
	do_set_optimizer(op);
      };

    /**
       Interface function to set precision
       \param x the threshold
     */
    void set_precision(rT x)
    {
      do_set_precision(x);
    }
    
    /**
       \return precision
     */
    
    rT get_precision()const
    {
      return do_get_precision();
    }

    /**
       Interface function to set start point
       \param p start point
     */
    void set_start_point(const pT& p)
    {
      do_set_start_point(p);
    }
    
    /**
       \return start point
     */
    pT get_start_point()const
    {
      return do_get_start_point();
    }

    /**
       Interface function to set lower limit
       \param p the lower limit
     */

    void set_lower_limit(const pT& p)
    {
      do_set_lower_limit(p);
    }

    /**
       \return lower limit
     */

    pT get_lower_limit()const
    {
      return do_get_lower_limit();
    }

    /**
       Interface function to set upper limit
       \param p upper limit
     */

    void set_upper_limit(const pT& p)
    {
      do_set_upper_limit(p);
    }

    /**
       \return upper limit
     */
    
    pT get_upper_limit()const
    {
      return do_get_upper_limit();
    }

    /**
       Interface function for performing the optimization
       \return the optimized parameter.
     */
    
    pT optimize()
      {
	return do_optimize();
      };
    

    /**
       \return the cloned object.
     */
    opt_method<rT,pT>* clone()const
    {
      return do_clone();
    }

    /**
       stop the optimization
     */
    void stop()
    {
      do_stop();
    }

    /**
       destroy the cloned object.
     */
    void destroy()
    {
      do_destroy();
    }

    /**
       \return the type name of self
     */

    virtual const char* get_type_name()const
    {
      return this->do_get_type_name();
    }

    /**
       deconstruct function
     */
    virtual ~opt_method(){};
  };
  
  
  /**
     \brief The manager for performing the manager
     \tparam rT the return type
     \tparam pT the self-varible type
  */
  template <typename rT,typename pT>
  class optimizer
  {
  public:

  private:

    /**
       pointer pointing a opt_method object
     */
    opt_method<rT,pT>* p_opt_method;

    /**
       pointer pointing a func_obj object
     */
    func_obj<rT,pT>* p_func_obj;
    
  public:
    /**
       default construct function
     */
     optimizer()
      :p_opt_method(NULL_PTR),p_func_obj(NULL_PTR)
    {}

    /**
       construct function
       \param fc object function
       \param om optimization method
     */
    optimizer(func_obj<rT,pT>& fc,const opt_method<rT,pT>& om)
      :p_func_obj(fc.clone()),p_opt_method(om.clone())
    {
      p_opt_method->set_optimizer(*this);
    }
    
    /**
       copy construct function
     */
    optimizer(const optimizer& rhs)
      :p_opt_method(NULL_PTR),p_func_obj(NULL_PTR)
    {
      if(rhs.p_func_obj!=NULL_PTR)
	{
	  set_func_obj(*(rhs.p_func_obj));
	}
      if(rhs.p_opt_method!=NULL_PTR)
	{
	  set_opt_method(*(rhs.p_opt_method));
	}
    }


    /**
       Assignment operator
     */
    optimizer& operator=(const optimizer& rhs)
    {
      if(this==&rhs)
	{
	  return *this;
	}
      if(rhs.p_func_obj!=NULL_PTR)
	{
	  set_func_obj(*(rhs.p_func_obj));
	}
      if(rhs.p_opt_method!=NULL_PTR)
	{
	  set_opt_method(*(rhs.p_opt_method));
	}
      return *this;
    }
    
    /**
       destruct function
     */
    virtual ~optimizer()
    {
      if(p_func_obj!=NULL_PTR)
	{
	  //delete p_func_obj;
	  p_func_obj->destroy();
	}
      if(p_opt_method!=NULL_PTR)
	{
	  //delete p_opt_method;
	  p_opt_method->destroy();
	}
    };

  public:
    /**
       set objection function
       \param fc objection function
     */
    void set_func_obj(const func_obj<rT,pT>& fc)
    {
      if(p_func_obj!=NULL_PTR)
	{
	  //delete p_func_obj;
	  p_func_obj->destroy();
	}
      p_func_obj=fc.clone();
      if(p_opt_method!=NULL_PTR)
	{
	  p_opt_method->set_optimizer(*this);
	}
    }

    /**
       set optimization method
       \param om optmization method
     */
    void set_opt_method(const opt_method<rT,pT>& om)
    {
      if(p_opt_method!=NULL_PTR)
	{
	  //delete p_opt_method;
	  p_opt_method->destroy();
	}
      
      p_opt_method=om.clone();
      p_opt_method->set_optimizer(*this);
    }

    /**
       \return a reference of internally kept optimization method
     */
    opt_method<rT,pT>& get_opt_method()
    {
      if(p_opt_method==NULL_PTR)
	{
	  throw opt_method_not_defined();
	}
      return *(this->p_opt_method);
    }

    
    /**
       \return a const reference of internally kept optimization method
     */
    const opt_method<rT,pT>& get_opt_method()const
    {
      if(p_opt_method==NULL_PTR)
	{
	  throw opt_method_not_defined();
	}
      return *(this->p_opt_method);
    }


    /**
       set precision
       \param x threshold
     */
    void set_precision(rT x)
    {
      if(p_opt_method==NULL_PTR)
	{
	  throw opt_method_not_defined();
	}
      p_opt_method->set_precision(x);
    }

    /**
       \return precision
     */

    rT get_precision()const
    {
      if(p_opt_method==NULL_PTR)
	{
	  throw opt_method_not_defined();
	}
      return p_opt_method->get_precision();
    }
    
    
    /**
       set start point
       \param x start point
     */
    void set_start_point(const pT& x)
    {
      if(p_opt_method==NULL_PTR)
	{
	  throw opt_method_not_defined();
	}
      p_opt_method->set_start_point(x);
    }

    /**
       \return start point
     */
    
    pT get_start_point()const
    {
      if(p_opt_method==NULL_PTR)
	{
	  throw opt_method_not_defined();
	}
      return p_opt_method->get_start_point();
    }
    

    /**
       set lower limit
       \param x lower limit
     */
    void set_lower_limit(const pT& x)
    {
      if(p_opt_method==NULL_PTR)
	{
	  throw opt_method_not_defined();
	}
      p_opt_method->set_lower_limit(x);
    }


    /**
       \return lower limit
     */
    pT get_lower_limit()const
    {
      if(p_opt_method==NULL_PTR)
	{
	  throw opt_method_not_defined();
	}
      return p_opt_method->get_lower_limit();
    }
    

    /**
       set upper limit
       \param x upper limit
     */
    void set_upper_limit(const pT& x)
    {
      if(p_opt_method==NULL_PTR)
	{
	  throw opt_method_not_defined();
	}
      p_opt_method->set_upper_limit(x);
    }

    /**
       \return upper limit
     */

    pT get_upper_limit()const
    {
      if(p_opt_method==NULL_PTR)
	{
	  throw opt_method_not_defined();
	}
      return p_opt_method->get_upper_limit();
    }
    

    /**
       call the objection function
       \param self var
       \return function value
     */
    rT eval(const pT& x)
    {
      if(p_func_obj==NULL_PTR)
	{
	  throw object_function_not_defined();
	}
      return p_func_obj->eval(x);
    }

	

    /**
       perform the optimization
       \return the optimizated paramater
     */
    pT optimize()
    {
      if(p_opt_method==NULL_PTR)
	{
	  throw opt_method_not_defined();
	}
      if(p_func_obj==NULL_PTR)
	{
	  throw object_function_not_defined();
	}
      return p_opt_method->optimize();
    }

    /**
       stop the optimize
     */
    void stop()
    {
      if(p_opt_method)
	{
	  p_opt_method->stop();
	}
    }
    
    /**
       \return the pointer to the inner object function
     */

    func_obj<rT,pT>* ptr_func_obj()
    {
      return p_func_obj;
    }


    /**
       \return the reference of the internal kept func_obj object
     */
    func_obj<rT,pT>& get_func_obj()
    {
      if(p_func_obj==NULL_PTR)
	{
	  throw object_function_not_defined();
	}
      return *p_func_obj;
    }
    
    /**
       \return the const reference of the internal kept func_obj object
    */
    const func_obj<rT,pT>& get_func_obj()const
    {
      if(p_func_obj==NULL_PTR)
	{
	  throw object_function_not_defined();
	}
      return *p_func_obj;
    }
  };
}
  
#endif
//EOF


