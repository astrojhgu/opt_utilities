/**
   \file fitter.hpp
   \brief classes used to perform model fitting by using optimizer
   \author Junhua Gu
 */

#ifndef FITTER_HPP
#define FITTER_HPP
#define OPT_HEADER
#include "opt_exception.hpp"
#include "optimizer.hpp"
#include <limits>
#include <vector>
#include <string>
#include <cstdlib>
#include <cassert>
#include <iostream>
namespace opt_utilities
{

  ///////////////////////////////////
  //////class data///////////////////
  //////contain single data point////
  ///////////////////////////////////
  template<typename Tdata,typename Tp,typename Ts,typename Tstr>
  class statistic;

  template<typename Tdata,typename Tp,typename Tstr>
  class param_modifier;

  /**
     \brief representing a single data point
     \tparam Ty the type of y
     \tparam Tx the type of x
   */
  template<typename _Ty,typename _Tx>
  class data
  {
  public:
    typedef _Ty Ty;
    typedef _Tx Tx;
  private:
    Tx x,x_lower_err,x_upper_err;
    Ty y,y_lower_err,y_upper_err;
  public:
    /**
       default construct
     */
    data()
      :x(),
       x_lower_err(),
       x_upper_err(),
       y(),
       y_lower_err(),
       y_upper_err()
    {}

    
    /**
       copy construct
     */
    data(const data& rhs)
    {
      opt_assign(x,rhs.get_x());
      opt_assign(x_lower_err,rhs.get_x_lower_err());
      opt_assign(x_upper_err,rhs.get_x_upper_err());
      opt_assign(y,rhs.get_y());
      opt_assign(y_lower_err,rhs.get_y_lower_err());
      opt_assign(y_upper_err,rhs.get_y_upper_err());
    }

    /**
       construct function
       \param _x x
       \param _y y
       \param _y_lower_err lower y error
       \param _y_upper_err upper y error
       \param _x_lower_err lower x error
       \param _x_upper_err upper x error
     */
    data(const Tx& _x,const Ty& _y,
	 const Ty& _y_lower_err,
	 const Ty& _y_upper_err,const Tx& _x_lower_err,const Tx& _x_upper_err)
    {
      opt_assign(x,_x);
      opt_assign(x_lower_err,_x_lower_err);
      opt_assign(x_upper_err,_x_upper_err);
      opt_assign(y,_y);
      opt_assign(y_lower_err,_y_lower_err);
      opt_assign(y_upper_err,_y_upper_err);
    }


    /**
       Assignment operator
     */
    virtual data& operator=(const data& rhs)
    {
      opt_assign(x,rhs.get_x());
      opt_assign(x_lower_err,rhs.get_x_lower_err());
      opt_assign(x_upper_err,rhs.get_x_upper_err());
      opt_assign(y,rhs.get_y());
      opt_assign(y_lower_err,rhs.get_y_lower_err());
      opt_assign(y_upper_err,rhs.get_y_upper_err());
      return *this;
    }

    /**
       destruct
     */
    virtual ~data()
    {}

  public:
    //get functions
    /**
       set x
       \return x
     */
    const Tx& get_x()const
    {
      return x;
    }
    
    /**
       \return x lower error
     */
    const Tx& get_x_lower_err()const
    {
      return x_lower_err;
    }
    
    /**
       \return x upper error
     */
    const Tx& get_x_upper_err()const
    {
      return x_upper_err;
    }

    /**
       \return y
     */
    const Ty& get_y()const
    {
      return y;
    }
    

    /**
       \return y lower error
     */
    const Ty& get_y_lower_err()const
    {
      return y_lower_err;
    }

    /**
       \return y upper error
     */
    
    const Ty& get_y_upper_err()const
    {
      return y_upper_err;
    }

  public:
    //set functions
    /**
       set x
       \param _x x
     */

    void set_x(const Tx& _x)
    {
      opt_assign(x,_x);
    }
    
    /**
       set x lower error
       \param _x x lower error
     */
    void set_x_lower_err(const Tx& _x)
    {
      opt_assign(x_lower_err,_x);
    }
    

    /**
       set x upper error
       \param _x x upper error
     */
    void set_x_upper_err(const Tx& _x)
    {
      opt_assign(x_upper_err,_x);
    }


    /**
       set y 
       \param _y y
     */
    void set_y(const Ty& _y)
    {
      opt_assign(y,_y);
    }
    

    /**
       set y lower error
       \param _y y lower error
     */
    void set_y_lower_err(const Ty& _y)
    {
      opt_assign(y_lower_err,_y);
    }
    
    /**
       set y upper error
       \param _y y upper error
     */
    void set_y_upper_err(const Ty& _y)
    {
      opt_assign(y_upper_err,_y);
    }
    

  };

  /**
     \brief virtual class representing a set of data
     \tparam Ty type of y
     \tparam Tx type of x
   */  
  template <typename Tdata>
  class data_set
  {
  public:
    typedef typename Tdata::Ty Ty;
    typedef typename Tdata::Tx Tx;
  private:
    virtual const Tdata& do_get_data(size_t i)const=0;
    virtual void do_set_data(size_t i,const Tdata& d)
    {
      throw data_unsetable();
    }
    virtual size_t do_size()const=0;
    virtual void do_add_data(const Tdata&)=0;
    virtual void do_clear()=0;
    virtual data_set<Tdata>* do_clone()const=0;
    /**
       \return the type name of self
     */

    virtual const char* do_get_type_name()const
    {
      return typeid(*this).name();
    }
    
    /**
       Overwrite this function to change the 
       behavior when destroying a heap-allocated instance.
       The default function cooperates with then 
       the do_clone function allocates a new instance with
       the new operator.
     */
    virtual void do_destroy()
    {
      delete this;
    }
  public:
    /**
       clone self
       \return a clone of self
     */
    data_set<Tdata>* clone()const
    {
      return this->do_clone();
    }
    
   /**
       destroy the cloned object
     */

    void destroy()
    {
      do_destroy();
    }

    /**
       destruct function
     */

    virtual ~data_set(){}
  public:
    //get functions
    /**
       get data
       \param i the order of the data point
       \return the const reference of a class data point
     */
    const Tdata& get_data(size_t i)const
    {
      return this->do_get_data(i);
    }

    /**
       get the type name
       \return the name of the type
     */
    const char* get_type_name()const
    {
      return this->do_get_type_name();
    } 

    /**
       \return the size of the data set
     */
    size_t size()const
    {
      return do_size();
    }

  public:
    //set functions

    void set_data(size_t i,const Tdata& d)
    {
      do_set_data(i,d);
    }
    
    /**
       add data point
       \param d data point
     */
    void add_data(const Tdata& d)
    {
      return do_add_data(d);
    }

    /**
       clear the data set
     */
    void clear()
    {
      do_clear();
    }
      
  };

  

  /**
     \brief the information of a model parameter
     \tparam Tp type of model param type
     \tparam Tstr the type of string type used
   */
  template <typename Tp,typename Tstr=std::string>
  class param_info
  {
  private:
    Tstr name;
    //bool frozen;
    typename element_type_trait<Tp>::element_type value;
    typename element_type_trait<Tp>::element_type lower_limit;
    typename element_type_trait<Tp>::element_type upper_limit;
    Tstr description;
  public:
    /**
       default construct
     */
    param_info()
      :name(),value(),lower_limit(-std::numeric_limits<typename element_type_trait<Tp>::element_type>::max()),
      upper_limit(std::numeric_limits<typename element_type_trait<Tp>::element_type>::max()),
       description()
    {}


    /**
       copy construct function
     */
    param_info(const param_info& rhs)
      :name(rhs.name),description(rhs.description)
    {
      opt_assign(value,rhs.value);
      opt_assign(lower_limit,rhs.lower_limit);
      opt_assign(upper_limit,rhs.upper_limit);
    }

    /**
       construct function
       \param _name the name of the param
       \param _v the value of the param
       \param _l the lower boundary of the param
       \param _u the upper boundary of the param
     */
    param_info(const Tstr& _name,
	       const typename element_type_trait<Tp>::element_type& _v,
	       const typename element_type_trait<Tp>::element_type& _l=-std::numeric_limits<typename element_type_trait<Tp>::element_type>::max(),
	       const typename element_type_trait<Tp>::element_type& _u=std::numeric_limits<typename element_type_trait<Tp>::element_type>::max(),
	       const Tstr& desc=Tstr())
      :name(_name),value(_v),lower_limit(_l),
       upper_limit(_u),description(desc)
    {}
    
    /**
       assignment operator
     */
    param_info& operator=(const param_info& rhs)
    {
      if(this==&rhs)
	{
	  return *this;
	}
      name=rhs.name;
      description=rhs.description;
      opt_assign(value,rhs.value);
      opt_assign(lower_limit,rhs.lower_limit);
      opt_assign(upper_limit,rhs.upper_limit);
      return *this;
    }

  public:
    //get functions
    /**
       \return the name of the parameter
     */
    const Tstr& get_name()const
    {
      return this->name;
    }

    /**
       \return the current value of the parameter
     */
    const typename element_type_trait<Tp>::element_type& get_value()const
    {
      return value;
    }

    /**
       \return the lower boundary
     */
    const typename element_type_trait<Tp>::element_type& get_lower_limit()const
    {
      return lower_limit;
    }

    /**
       \return the upper boundary
     */
    const typename element_type_trait<Tp>::element_type& get_upper_limit()const
    {
      return upper_limit;
    }

    /**
       Returns a piece of description
       Usually used as the help information of the parameter
       \return a piece of description of the parameter
    */
    const Tstr& get_description()const
    {
      return description;
    }
  public:
    //set functions
    /**
       set the value
       \param x the value of the parameter
     */
    void set_value(const typename element_type_trait<Tp>::element_type& x)
    {
      opt_assign(value,x);
    }


    /**
       set the lower boundary
       \param x the lower boundary
     */
    void set_lower_limit(const typename element_type_trait<Tp>::element_type& x)
    {
      opt_assign(lower_limit,x);
    }

    /**
       set the upper limit
       \param x the upper boundary
     */
    void set_upper_limit(const typename element_type_trait<Tp>::element_type& x)
    {
      opt_assign(upper_limit,x);
    }

    /**
       set the name of the parameter
       \param _name the name of the parameter
    */
    
    void set_name(const Tstr& _name)
    {
      name=_name;
    }
    
    /**
       Set the description
       \param desc the description to be assigned
    */

    void set_description(const Tstr& desc)
    {
      description=desc;
    }

  };




  /**
     \brief virtual class representing a model
     \tparam Ty the type of the returned value of the model
     \tparam Tx the type of the self-var
     \tparam Tp the type of the model param
     \tparam Tstr the type of the string used
   */
  template <typename Tdata,typename Tp,typename Tstr=std::string>
  class model
  {
  public:
    typedef typename Tdata::Ty Ty;
    typedef typename Tdata::Tx Tx;
  private:
    std::vector<param_info<Tp,Tstr> > param_info_list;
    param_info<Tp,Tstr> null_param;
    //    int num_free_params;
    param_modifier<Tdata,Tp,Tstr>* p_param_modifier;
  private:
    /**
       Clone self, 
       The default behavior is to allocate a new instance
       on the heap with new operator and return the pointer.
       \return a point to the cloned instance
     */
    
    virtual model<Tdata,Tp,Tstr>* do_clone()const=0;

    /**
       Destroy the cloned instance
     */

    virtual void do_destroy()
    {
      delete this;
    }
    
    /**
       Should be implemented to evaluate the model
       \param x the varible
       \param p the parameter
       \return the model value
     */
    virtual Ty do_eval(const Tx& x,const Tp& p)=0;
    
    /**
       Can be overrided to return a piece of information of the model.
       The default implement returns a empty string.
     */
    virtual Tstr do_get_information()const
    {
      return Tstr();
    }

    virtual bool do_meets_constraint(const Tp& p)const
    {
      Tp p1=this->reform_param(p);
      for(size_t i=0;i!=p1.size();++i)
	{
	  if(get_element(p,i)>get_param_info(i).get_upper_limit()||
	     get_element(p,i)<get_param_info(i).get_lower_limit())
	    {
	      return false;
	    }
	}
      return true;
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
       default construct function
     */
    model()
      :p_param_modifier(NULL_PTR)
    {}
    

    /**
       copy construct
     */
    model(const model& rhs)
      :p_param_modifier(NULL_PTR)
    {
      param_info_list=rhs.param_info_list;
      if(rhs.p_param_modifier!=NULL_PTR)
	{
	  set_param_modifier(*(rhs.p_param_modifier));
	}
      null_param=rhs.null_param;
      
    }
    

    /**
       assignment operator
     */
    model& operator=(const model& rhs)
    {
      if(this==&rhs)
	{
	  return *this;
	}
      param_info_list=rhs.param_info_list;
      if(rhs.p_param_modifier!=NULL_PTR)
	{
	  set_param_modifier(*(rhs.p_param_modifier));
	}
      null_param=rhs.null_param;
      return *this;
    }
    

    /**
       destructure function
     */
    virtual ~model()
    {
      if(p_param_modifier!=NULL_PTR)
	{
	  //delete p_param_modifier;
	  p_param_modifier->destroy();
	}
    }

    /**
       \return the cloned object
     */
    model<Tdata,Tp,Tstr>* clone()const
    {
      return do_clone();
    }

    /**
       destroy the cloned object
     */
    void destroy()
    {
      do_destroy();
    }

  public:
    /**
       Get the type name
       Usually used to return the name of this model, which is often 
       used as a key when implementing the prototype pattern
       \return the type name
     */
    
    const char* get_type_name()const
    {
      return this->do_get_type_name();
    }



    /**
       \return the param_modifier
     */
    param_modifier<Tdata,Tp,Tstr>& get_param_modifier()
    {
      if(p_param_modifier==NULL_PTR)
	{
	  throw param_modifier_not_defined();
	}
      return *p_param_modifier;
    }

    /**
       \return the param_modifier
    */
    const param_modifier<Tdata,Tp,Tstr>& get_param_modifier()const
    {
      if(p_param_modifier==NULL_PTR)
	{
	  throw param_modifier_not_defined();
	}
      return *p_param_modifier;
    }

    /**
       report the param status
       \return the param status
     */
    Tstr report_param_status(const Tstr& s)const
    {
      if(p_param_modifier==NULL_PTR)
	{
	  return Tstr();
	}
      
      return p_param_modifier->report_param_status(s);
	
    }
    

    /**
       \param pname the name of the param
       \return the param info
    */
    const param_info<Tp,Tstr>& get_param_info(const Tstr& pname)const
    {
      for(typename std::vector<param_info<Tp,Tstr> >::const_iterator i=param_info_list.begin();
	  i!=param_info_list.end();++i)
	{
	  if(i->get_name()==pname)
	    {
	      return *i;
	    }
	}
      std::cerr<<"Param unfound!"<<std::endl;
      //assert(false);
      throw param_not_found();
      //return null_param;
    }


    /**
       \param n the order of the parameter
       \return the param info
     */
    const param_info<Tp,Tstr>& get_param_info(size_t n)const
    {
      return param_info_list[n%get_num_params()];
    }

    
    /**
       \return the full parameter vector
     */
    Tp get_all_params()const
    {
      Tp result;
      resize(result,param_info_list.size());
      for(size_t i=0;i<param_info_list.size();++i)
	{
	  //opt_assign(get_element(result,i),param_info_list[i].get_value());
	  set_element(result,i,param_info_list[i].get_value());
	  //get_element((Tp)result,i);
	}
      return result;
    }


    /**
       \return the lower limit
     */
    Tp get_all_lower_limits()const
    {
      Tp result;
      resize(result,param_info_list.size());
      for(size_t i=0;i<param_info_list.size();++i)
	{
	  //opt_assign(get_element(result,i),param_info_list[i].get_value());
	  set_element(result,i,param_info_list[i].get_lower_limit());
	  //get_element((Tp)result,i);
	}
      return result;
    }

    /**
       \return the upper limit
     */
    Tp get_all_upper_limits()const
    {
      Tp result;
      resize(result,param_info_list.size());
      for(size_t i=0;i<param_info_list.size();++i)
	{
	  //opt_assign(get_element(result,i),param_info_list[i].get_value());
	  set_element(result,i,param_info_list[i].get_upper_limit());
	  //get_element((Tp)result,i);
	}
      return result;
    }

  
    /**
       \return the number of parameters
     */
    size_t get_num_params()const
    {
      return param_info_list.size();
    }


    /**
       \return the number of free parameters
     */
    size_t get_num_free_params()const
    {
      if(p_param_modifier!=NULL_PTR)
	{
	  return p_param_modifier->get_num_free_params();
	}
      return get_num_params();
    }

    /**
       get the order of a parameter
       \param pname the name of the parameter
       \return the order of the parameter
     */
    size_t get_param_order(const Tstr& pname)const
    {
      for(size_t i=0;i<param_info_list.size();++i)
	{
	  if(param_info_list[i].get_name()==pname)
	    {
	      return i;
	    }
	}
      // assert(false);
      throw param_not_found();
      //return (size_t)-1;
    }
    

    /**
       \return the description of the model
     */
    Tstr get_information()const
    {
      return do_get_information();
    }

    /**
       \param p input deformed parameter
       \return if the given param meets the constraint of the model
    */
    bool meets_constraint(const Tp& p)const
    {
      return do_meets_constraint(p);
    }

  public:
    /**
       set the param modifier
       \param pm param modifier
     */
    void set_param_modifier(const param_modifier<Tdata,Tp,Tstr>& pm)
    {
      if(p_param_modifier!=NULL_PTR)
	{
	  //delete p_param_modifier;
	  p_param_modifier->destroy();
	}
      p_param_modifier=pm.clone();
      p_param_modifier->set_model(*this);
    }

    /**
       clear the param modifier
    */
    void clear_param_modifier()
    {
      if(p_param_modifier!=NULL_PTR)
	{
	  //delete p_param_modifier;
	  p_param_modifier->destroy();
	}
      p_param_modifier=NULL_PTR;
    }
    
    /**
       \param pinfo param information being set
     */
    void set_param_info(const param_info<Tp,Tstr>& pinfo)
    {
      for(typename std::vector<param_info<Tp,Tstr> >::iterator i=param_info_list.begin();
	  i!=param_info_list.end();++i)
	{
	  if(i->get_name()==pinfo.get_name())
	    {
	      i->set_value(pinfo.get_value());
	      i->set_lower_limit(pinfo.get_lower_limit());
	      i->set_upper_limit(pinfo.get_upper_limit());
	      return;
	    }
	}
      throw param_not_found();
    }
    

    /**
       \param pname the name of the parameter
       \param v the value of the pearameter
    */
    void set_param_value(const Tstr& pname,
			 const typename element_type_trait<Tp>::element_type& v)
    {
      //int porder=0;
      for(typename std::vector<param_info<Tp,Tstr> >::iterator i=param_info_list.begin();
	  i!=param_info_list.end();++i)
	{
	  if(i->get_name()==pname)
	    {
	      i->set_value(v);
	      return;
	    }
	}
      throw param_not_found();
    }

    
    /**
       set the lower limit
       \param pname the parameter name
       \param v the value of the lower limit
     */
    void set_param_lower_limit(const Tstr& pname,
			       const typename element_type_trait<Tp>::element_type& v)
    {
      //int porder=0;
      for(typename std::vector<param_info<Tp,Tstr> >::iterator i=param_info_list.begin();
	  i!=param_info_list.end();++i)
	{
	  if(i->get_name()==pname)
	    {
	      i->set_lower_limit(v);
	      return;
	    }
	}
      std::cerr<<"param "<<pname<<" unfound"<<std::endl;
      throw param_not_found();
    }


    /**
       set the upper limit
       \param pname the parameter name
       \param v the value of the upper limit
    */
    void set_param_upper_limit(const Tstr& pname,
			       const typename element_type_trait<Tp>::element_type& v)
    {
      //int porder=0;
      for(typename std::vector<param_info<Tp,Tstr> >::iterator i=param_info_list.begin();
	  i!=param_info_list.end();++i)
	{
	  if(i->get_name()==pname)
	    {
	      i->set_upper_limit(v);
	      return;
	    }
	}
      std::cerr<<"param "<<pname<<" unfound"<<std::endl;
      throw param_not_found();
    }
    

    /**
       set param 
       \param param the values of the parameter
     */
    void set_param_value(const Tp& param)
    {
      for(size_t i=0;i<param_info_list.size();++i)
	{
	  param_info_list[i].set_value(get_element(param,i));
	}
    }


    /**
       set lower limit 
       \param param the lower limit of the parameter
    */
    void set_param_lower_limit(const Tp& param)
    {
      for(size_t i=0;i<param_info_list.size();++i)
	{
	  param_info_list[i].set_lower_limit(get_element(param,i));
	}
    }
    

    /**
       set upper limit 
       \param param the upper limit of the parameter
    */
    void set_param_upper_limit(const Tp& param)
    {
      for(size_t i=0;i<param_info_list.size();++i)
	{
	  param_info_list[i].set_upper_limit(get_element(param,i));
	}
    }
		
  protected:

    /**
       add param info
       \param pinfo the param info to be added
     */
    void push_param_info(const param_info<Tp,Tstr>& pinfo)
    {
      param_info_list.push_back(pinfo);
      //      this->num_free_params++;
    }
    
    /**
       clear the param information list
     */
    void clear_param_info()
    {
      //      this->num_free_params=0;
      param_info_list.clear();
      //clear the memory
      std::vector<param_info<Tp,Tstr> >().swap(param_info_list);
    }



  public:   
    /**
       When the parameter modifier exists,
       use it to form the complete parameter list to be fed to the model.
       The basic idea is that say we want to freeze some parameter(s),
       we will assign a param_modifier to this model, then some parameters
       vanishes in the fitting, but when in the core of the model, i.e., the
       do_eval member function, it only accepts a complete list of paramters.
       Thus we need a function to complete the parameter list from the
       vanished list.
       \param p the input incomplete parameter list
       \return the output complete parameter list to be fed to the do_eval.
     */
    Tp reform_param(const Tp& p)const
    {
      if(p_param_modifier==NULL_PTR)
	{
	  return p;
	}
      return p_param_modifier->reform(p);
    }
    /**
       Perform the inverse operator of reform_param
       \param p the complete parameter list 
       \param p the vanished parameter list
     */
    Tp deform_param(const Tp& p)const
    {
      if(p_param_modifier==NULL_PTR)
	{
	  return p;
	}
      return p_param_modifier->deform(p);
    }

    /**
       evaluate the model
       \param x the self var
       \param p the parameter
       \return the model value
     */
    Ty eval(const Tx& x,const Tp& p)
    {
      return do_eval(x,reform_param(p));
    }

    /**
       evaluate the model, and ignore the param_modifier.
       \param p the parameter
       \return the model value
     */
    Ty eval_raw(const Tx& x,const Tp& p)
    {
      //return do_eval(x,reform_param(p));
      return do_eval(x,p);
    }
  };


  /**
     \brief class to perform the model fitting
     \tparam Ty the type of the model return type
     \tparam Tx the type of the model self-var
     \tparam Tp the type of the model param
     \tparam Ts statistic type
     \tparam Tstr the type of string used
   */
  template<typename Tdata,typename Tp,typename Ts,typename Tstr=std::string>
  class fitter
  {
  public:
    typedef typename Tdata::Ty Ty;
    typedef typename Tdata::Tx Tx;
  private:
    model<Tdata,Tp,Tstr>* p_model;
    statistic<Tdata,Tp,Ts,Tstr>* p_statistic;
    data_set<Tdata>* p_data_set;
    optimizer<Ts,Tp> optengine;
  public:
    
    /**
       default construct function
    */
    fitter()
      :p_model(NULL_PTR),p_statistic(NULL_PTR),p_data_set(NULL_PTR),optengine()
    {}

    
    /**
       copy construct function
     */
    fitter(const fitter& rhs)
      :p_model(NULL_PTR),p_statistic(NULL_PTR),p_data_set(NULL_PTR),optengine()
    {
      if(rhs.p_model!=NULL_PTR)
	{
	  set_model(*(rhs.p_model));
	}
      if(rhs.p_statistic!=NULL_PTR)
	{
	  set_statistic(*(rhs.p_statistic));
	  //assert(p_statistic->p_fitter!=0);
	}
      if(rhs.p_data_set!=NULL_PTR)
	{
	  set_data_set(*(rhs.p_data_set));
	}
      optengine=rhs.optengine;
    }
    

    /**
       assignment operator
     */
    fitter& operator=(const fitter& rhs)
    {
      if(this==&rhs)
	{
	  return *this;
	}
      if(rhs.p_model!=NULL_PTR)
	{
	  set_model(*(rhs.p_model));
	}
      if(rhs.p_statistic!=NULL_PTR)
	{
	  set_statistic(*(rhs.p_statistic));
	}
      if(rhs.p_data_set!=NULL_PTR)
	{
	  set_data_set(*(rhs.p_data_set));
	}

      optengine=rhs.optengine;
      return *this;
    }
    

    /**
       destruct function
     */
    virtual ~fitter()
    {
      if(p_model!=NULL_PTR)
	{
	  //delete p_model;
	  p_model->destroy();
	}
      if(p_statistic!=NULL_PTR)
	{
	  //delete p_statistic;
	  p_statistic->destroy();
	}
      if(p_data_set!=NULL_PTR)
	{
	  //delete p_data_set;
	  p_data_set->destroy();
	}
    }
    
  public:
    /**
       evaluate the model
       \param x the varible
       \param p the parameter
       \return the model value
     */
    Ty eval_model(const Tx& x,const Tp& p)
    {
      if(p_model==NULL_PTR)
	{
	  throw model_not_defined();
	}
      return p_model->eval(x,p);
    }

    /**
       evaluate the model, ignore the param_modifier
       \param x the varible
       \param p the parameter
       \return the model value       
     */
    
    Ty eval_model_raw(const Tx& x,const Tp& p)
    {
      if(p_model==NULL_PTR)
	{
	  throw model_not_defined();
	}
      return p_model->eval_raw(x,p);
    }
  public:
    /**
       get the data set that have been loaded
       \return the const reference of inner data_set
    */
    data_set<Tdata>& get_data_set()
    {
      if(p_data_set==NULL_PTR)
	{
	  throw data_not_loaded();
	}
      return *(this->p_data_set);
    }


    /**
       get the data set that have been loaded
       \return the const reference of inner data_set
     */
    const data_set<Tdata>& get_data_set()const
    {
      if(p_data_set==NULL_PTR)
	{
	  throw data_not_loaded();
	}
      return *(this->p_data_set);
    }

    /**
       Get the model used
       \return the reference of model used
     */
    model<Tdata,Tp,Tstr>& get_model()
    {
      if(p_model==NULL_PTR)
	{
	  throw model_not_defined();
	}
      return *(this->p_model);
    }

    /**
       Get the model used
       \return the reference of model used
    */
    const model<Tdata,Tp,Tstr>& get_model()const
    {
      if(p_model==NULL_PTR)
	{
	  throw model_not_defined();
	}
      return *(this->p_model);
    }

    /**
       Get the statistic used
       \return the reference of the statistic used
     */
    statistic<Tdata,Tp,Ts,Tstr>& get_statistic()
    {
      if(p_statistic==NULL_PTR)
	{
	  throw statistic_not_defined();
	}
      return *(this->p_statistic);
    }

    /**
       Get the statistic used
       \return the reference of the statistic used
     */
    const statistic<Tdata,Tp,Ts,Tstr>& get_statistic()const
    {
      if(p_statistic==NULL_PTR)
	{
	  throw statistic_not_defined();
	}
      return *(this->p_statistic);
    }

    /**
       \return current statstic value
     */
    Ts get_statistic_value()
    {
      Tp current_params(get_model().get_all_params());
      return get_statistic().eval(get_model().deform_param(current_params));
    }
    
    /**
       Get the optimization method that used
       \return the reference of the opt_method
     */
    opt_method<Ts,Tp>& get_opt_method()
    {
      return optengine.get_opt_method();
    }
    
    /**
       Get the optimization method that used
       \return the reference of the opt_method
     */
    const opt_method<Ts,Tp>& get_opt_method()const
    {
      return optengine.get_opt_method();
    }


    /**
       Get the inner kept param modifier
       \return the reference of param_modifier
    */
    param_modifier<Tdata,Tp,Tstr>& get_param_modifier()
    {
      if(p_model==NULL_PTR)
	{
	  throw model_not_defined();
	}
      return p_model->get_param_modifier();
    }

    /**
       Get the inner kept param modifier
       \return the reference of param_modifier
     */
    const param_modifier<Tdata,Tp,Tstr>& get_param_modifier()const
    {
      if(p_model==NULL_PTR)
	{
	  throw model_not_defined();
	}
      return p_model->get_param_modifier();
    }

    /**
       report the status of a parameter, e.g., freezed, thaw, etc.
       \param s the name of a parameter
       \return string used to describe the parameter
     */
    Tstr report_param_status(const Tstr& s)const
    {
      if(p_model==NULL_PTR)
	{
	  throw model_not_defined();
	}
      return p_model->report_param_status(s);
    }

  public:

    /**
       get the parameter value
       \param pname the name of the parameter
       \return the value of the parameter
     */
    typename element_type_trait<Tp>::element_type get_param_value(const Tstr& pname)const
    {
      if(p_model==NULL_PTR)
	{
	  throw model_not_defined();
	}
      return p_model->get_param_info(pname).get_value();
    }

    /**
       get the lower limit of a parameter
       \param pname the name of a parameter
       \return the lower limit of a parameter
     */
    typename element_type_trait<Tp>::element_type get_param_lower_limit(const Tstr& pname)const
    {
      if(p_model==NULL_PTR)
	{
	  throw model_not_defined();
	}
      return p_model->get_param_info(pname).get_lower_limit();
    }
    
    /**
       get the upper limit of a parameter
       \param pname the name of a parameter
       \return the upper limit of a parameter
     */
    typename element_type_trait<Tp>::element_type get_param_upper_limit(const Tstr& pname)const
    {
      if(p_model==NULL_PTR)
	{
	  throw model_not_defined();
	}
      return p_model->get_param_info(pname).get_upper_limit();
    }

    /**
       get the param_info of a parameter
       \param pname the name of the parameter
       \return the const reference of a param_info object
     */
    const param_info<Tp,Tstr>& get_param_info(const Tstr& pname)const
    {
      if(p_model==NULL_PTR)
	{
	  throw model_not_defined();
	}
      return p_model->get_param_info(pname);
    }
    
    /**
       get the param_info of a parameter by its order
       \param n the order of the parameter
       \return the const reference of a param_info object
     */
    const param_info<Tp,Tstr>& get_param_info(size_t n)const
    {
      if(p_model==NULL_PTR)
	{
	  throw model_not_defined();
	}
      return p_model->get_param_info(n);
    }

    /**
       get the order of a parameter by its name
       \param pname the name of the parameter
       \return the order of the parameter
     */
    size_t get_param_order(const Tstr& pname)const
    {
      if(p_model==NULL_PTR)
	{
	  throw model_not_defined();
	}
      return p_model->get_param_order(pname);
    }

    /**
       get the number of parameters
       \return the number of parameters
     */
    size_t get_num_params()const
    {
      if(p_model==NULL_PTR)
	{
	  throw model_not_defined();
	}
      return p_model->get_num_params();
    }

    /**
       get all params
       \return the vector containing the values of all parameters
     */
    Tp get_all_params()const
    {
      if(p_model==NULL_PTR)
	{
	  throw model_not_defined();
	}
      //return current_param;
      return p_model->get_all_params();
    }


  public:

    /**
       set the model
       \param m model to be used
     */
    void set_model(const model<Tdata,Tp,Tstr>& m)
    {
      if(p_model!=NULL_PTR)
	{
	  //delete p_model;
	  p_model->destroy();
	}
      p_model=m.clone();
      //p_model=&m;
      //  current_param.resize(m.get_num_params());
    }


    /**
       set the statistic (e.g., chi square, least square c-statistic etc.)
       \param s statistic to be used
     */
    void set_statistic(const statistic<Tdata,Tp,Ts,Tstr>& s)
    {
      if(p_statistic!=NULL_PTR)
	{
	  //delete p_statistic;
	  p_statistic->destroy();
	}
      p_statistic=s.clone();
      //p_statistic=&s;
      p_statistic->set_fitter(*this);
    }

    /**
       set parameter modifier
       \param pm parameter modifier to be used
     */
    void set_param_modifier(const param_modifier<Tdata,Tp,Tstr>& pm)
    {
      if(p_model==NULL_PTR)
	{
	  throw model_not_defined();
	}
      p_model->set_param_modifier(pm);
    }

    /**
       clear the param modifier
    */
    void clear_param_modifier()
    {
      if(p_model==NULL_PTR)
	{
	  throw model_not_defined();
	}
      p_model->clear_param_modifier();
    }
    
    /**
       load the data set
       \param da a data set
    */
    void load_data(const data_set<Tdata>& da)
    {
      if(p_data_set!=NULL_PTR)
	{
	  //delete p_data_set;
	  p_data_set->destroy();
	}
      p_data_set=da.clone();
      if(p_statistic!=NULL_PTR)
	{
	  p_statistic->set_fitter(*this);
	}
    }
    
    /**
       Same as load_data
       \param da the data to be set
     */
    void set_data_set(const data_set<Tdata>& da)
    {
      load_data(da);
    }

  public:
    /**
       set the value of a parameter
       \param pname the name of the parameter
       \param v the value of the parameter
    */
    void set_param_value(const Tstr& pname,
			 const typename element_type_trait<Tp>::element_type& v)
    {
      if(p_model==NULL_PTR)
	{
	  throw model_not_defined();
	}
      p_model->set_param_value(pname,v);
    }

    /**
       set the lower limit of a parameter
       \param pname the name of the parameter
       \param v the lower limit of the parameter       
     */
    void set_param_lower_limit(const Tstr& pname,
			       const typename element_type_trait<Tp>::element_type& v)
    {
      if(p_model==NULL_PTR)
	{
	  throw model_not_defined();
	}
      p_model->set_param_lower_limit(pname,v);
    }

    /**
       set the upper limit of a parameter
       \param pname the name of the parameter
       \param v the upper limit of the parameter       
     */
    void set_param_upper_limit(const Tstr& pname,
			       const typename element_type_trait<Tp>::element_type& v)
    {
      if(p_model==NULL_PTR)
	{
	  throw model_not_defined();
	}
      p_model->set_param_upper_limit(pname,v);
    }
    

    /**
       set the values of all parameters
       \param param the vector containing the value of all parameters
     */

    void set_param_value(const Tp& param)
    {
      if(p_model==NULL_PTR)
	{
	  throw model_not_defined();
	}
      p_model->set_param_value(param);
    }

    /**
       set the lower limits of all parameters
       \param param the vector containing the lower limits of all parameters
    */
    void set_param_lower_limit(const Tp& param)
    {
      if(p_model==NULL_PTR)
	{
	  throw model_not_defined();
	}
      p_model->set_param_lower_limit(param);
    }

    /**
       set the upper limits of all parameters
       \param param the vector containing the upper limits of all parameters
    */
    void set_param_upper_limit(const Tp& param)
    {
      if(p_model==NULL_PTR)
	{
	  throw model_not_defined();
	}
      p_model->set_param_upper_limit(param);
    }


    /**
       Set the param information
       \param pinfo the param information being set
     */

    void set_param_info(const param_info<Tp,Tstr>& pinfo)
    {
      if(p_model==NULL_PTR)
	{
	  throw model_not_defined();
	}
      p_model->set_param_info(pinfo);
    }

    

    /**
       set the optimization method used to perform the model fitting
       \param pm the opt_method to be used
    */
    void set_opt_method(const opt_method<Ts,Tp>& pm)
    {
      //assert(p_optimizer!=NULL_PTR);
      optengine.set_opt_method(pm);
    }

    /**
       set the precision
       \param y the precision
     */
    void set_precision(typename element_type_trait<Tp>::element_type y)
    {
      optengine.set_precision(y);
    }


    /**
       perform the fitting
     */
    Tp fit()
    {
      //      assert(p_model!=NULL_PTR);
      if(p_model==NULL_PTR)
	{
	  throw model_not_defined();
	}
      if(p_data_set==NULL_PTR)
	{
	  throw data_not_loaded();
	}
      //assert(p_optimizer!=NULL_PTR);
      //assert(p_data_set!=NULL_PTR);
      //assert(p_statistic!=NULL_PTR);
      if(p_statistic==NULL_PTR)
	{
	  throw statistic_not_defined();
	}

      optengine.set_func_obj(*p_statistic);
      Tp current_param;
      Tp current_lower_limits;
      Tp current_upper_limits;
      opt_assign(current_param,p_model->get_all_params());
      opt_assign(current_lower_limits,p_model->get_all_lower_limits());
      opt_assign(current_upper_limits,p_model->get_all_upper_limits());
      Tp start_point;
      Tp upper_limits;
      Tp lower_limits;
      opt_assign(start_point,p_model->deform_param(current_param));
      opt_assign(upper_limits,p_model->deform_param(current_upper_limits));
      opt_assign(lower_limits,p_model->deform_param(current_lower_limits));

      //      std::cout<<start_point.size()<<std::endl;
      
      
      //for(int i=NULL_PTR;i<(int)start_point.size();++i)
      //	{
      //	  std::cout<<start_point[i]<<",";
      //	}
      //std::cout<<std::endl;
      //assert(start_point.size()!=NULL_PTR);
      if(get_size(start_point)==0)
	{
	  //return start_point;
	  return p_model->get_all_params();
	}
      optengine.set_lower_limit(lower_limits);
      optengine.set_upper_limit(upper_limits);
      optengine.set_start_point(start_point);
      
      Tp result;
      opt_assign(result,optengine.optimize());

      Tp decurrent_param;
      opt_assign(decurrent_param,p_model->reform_param(result));
      //current_param.resize(decurrent_param.size());
      resize(current_param,get_size(decurrent_param));
      opt_assign(current_param,decurrent_param);
      p_model->set_param_value(current_param);
      //   return current_param;
      return p_model->get_all_params();
    }

    /**
       stop the fitting
    */
    void stop()
    {
      optengine.stop();
    }
    
  };


  /**
     \brief virtual class representing a statistic
     \tparam Ty the type of the model return type
     \tparam Tx the type of the model self-var
     \tparam Tp the type of the model param
     \tparam Ts statistic type
     \tparam Tstr the type of string used
  */
  template<typename Tdata,typename Tp,typename Ts,typename Tstr=std::string>
  class statistic
    :public func_obj<Ts,Tp>
  {
  public:
    typedef typename Tdata::Ty Ty;
    typedef typename Tdata::Tx Tx;
  private:
    fitter<Tdata,Tp,Ts,Tstr>* p_fitter;

  private:
    virtual statistic<Tdata,Tp,Ts,Tstr>* do_clone()const=0;

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
       default construct
    */
    statistic()
      :p_fitter(NULL_PTR)
    {}
    
    /**
       copy construct
    */
    statistic(const statistic& rhs)
      :func_obj<Ts,Tp>(static_cast<const func_obj<Ts,Tp>& >(rhs))
      ,p_fitter(rhs.p_fitter)
    {}

    /**
       assignment operator
     */
    statistic& operator=(const statistic& rhs)
    {
      if(this==&rhs)
	{
	  return *this;
	}
      p_fitter=rhs.p_fitter;
      return *this;
    }
    

    /**
       destructure function
     */
    virtual ~statistic()
    {}


  public:
    /**
       clone the existing object
       \return the clone of self
     */
    statistic<Tdata,Tp,Ts,Tstr>* clone()const
    {
      return this->do_clone();
    }

    /**
       destroy the cloned object
     */
    void destroy()
    {
      return do_destroy();
    }

    /**
       Return the type name, used in a prototype pattern
       \return the type name
     */
    const char* get_type_name()const
    {
      return this->do_get_type_name();
    }
  public:    
    /**
       set the fitter
       \param pfitter the fitter to be linked
    */
    virtual void set_fitter(fitter<Tdata,Tp,Ts,Tstr>& pfitter)
    {
      p_fitter=&pfitter;
    }


    /**
       get the attached fitter
       \return the const reference of the fitter object
     */
    virtual const fitter<Tdata,Tp,Ts,Tstr>& get_fitter()const
    {
      if(p_fitter==NULL_PTR)
	{
	  throw fitter_not_set();
	}
      return *p_fitter;
    }
		
    /**
       evaluating the model
       \param x the self-var
       \param p the parameter
       \return the evaluated model value
     */
    Ty eval_model(const Tx& x,const Tp& p)
    {
      if(p_fitter==NULL_PTR)
	{
	  throw fitter_not_set();
	}
      return p_fitter->eval_model(x,p);
    }

    /**
       get the data_set object managed by the fitter object
       \return the const reference of the data_set object
     */
    const data_set<Tdata>& get_data_set()const
    {
      if(p_fitter==NULL_PTR)
	{
	  throw fitter_not_set();
	}
      return p_fitter->get_data_set();
    }

  };


  /**
     \brief Used to modify the parameter, e.g., freezing, bind
     \tparam Ty the type of the model return type
     \tparam Tx the type of the model self-var
     \tparam Tp the type of the model param
     \tparam Tstr the type of string used
  */
  template <typename Tdata,typename Tp,typename Tstr=std::string>
  class param_modifier
  {
  private:
    model<Tdata,Tp,Tstr>* p_model;
  private:
    /**
       \return the type name of self
    */
    
    virtual const char* do_get_type_name()const
    {
      return typeid(*this).name();
    }

    /**
       Form the complete parameter list from a vanished parameter list
       \param p the vanished parameter list
       \return the complete parameter list
     */
    virtual Tp do_reform(const Tp& p)const=0;
    /**
       The inverse operator of do_reform
       \param p the complete parameter list
       \return the vanished parameter list
     */
    virtual Tp do_deform(const Tp& p)const=0;
    virtual size_t do_get_num_free_params()const=0;
    virtual Tstr do_report_param_status(const Tstr&)const=0;
    virtual void update(){}

    virtual param_modifier<Tdata,Tp,Tstr>* do_clone()const=0;
    
    virtual void do_destroy()
    {
      delete this;
    }
  public:
    /**
       the default construct function
     */
    param_modifier()
      :p_model(NULL_PTR)
    {}
    
    /**
       copy construct function
     */
    param_modifier(const param_modifier& rhs)
      :p_model(rhs.p_model)
    {}

    /**
       assignment operator
     */
    param_modifier& operator=(const param_modifier& rhs)
    {
      if(this==&rhs)
	{
	  return *this;
	}
      p_model=rhs.p_model;
      return *this;
    }
    /**
       destruct function
     */
    virtual ~param_modifier(){}

  public:

    /**
       return the clone of self
       \return the clone of self
     */
    param_modifier<Tdata,Tp,Tstr>* clone()const
    {
      return do_clone();
    }

    /**
       destroy the cloned object
    */
    void destroy()
    {
      do_destroy();
    }

    /**
       Return the type name
       \return type name
     */
    const char* get_type_name()const
    {
      return this->do_get_type_name();
    }    
  public:
    /**
       constructing full parameter list from the free parameters
     */
    Tp reform(const Tp& p)const
    {
      return do_reform(p);
    }

    /**
       constructing the free parameter from the full parameters
     */
    Tp deform(const Tp& p)const
    {
      return do_deform(p);
    }


  public:
    
    /**
       Attach the fitter object
       \param pf the fitter to be attached
     */
    void set_model(model<Tdata,Tp,Tstr>& pf)
    {
      p_model=&pf;
      update();
    }

    /**
       get the model attached
       \return the const reference of the model
     */
    const model<Tdata,Tp,Tstr>& get_model()const
    {
      if(p_model==NULL_PTR)
	{
	  std::cout<<"dajf;asdjfk;";
	  throw model_not_defined();
	}
      return *(this->p_model);
    }

    /**
       calculate the number of free parameters
     */
    size_t get_num_free_params()const
    {
      return do_get_num_free_params();
    }

    /**
       report the status of parameters
       \param pname parameter name
       \return the string used to describe the parameter
     */
    Tstr report_param_status(const Tstr& name)const
    {
      return do_report_param_status(name);
    }
  };
}


#endif
//EOF
