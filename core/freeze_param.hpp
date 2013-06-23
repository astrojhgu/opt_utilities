/**
   \file freeze_param.hpp
   \brief param modifer that used to freeze one or more parameters
   \author Junhua Gu
*/

#ifndef FREEZE_PARAM_HPP
#define FREEZE_PARAM_HPP
#define OPT_HEADER
#include "fitter.hpp"
#include <vector>
#include <set>
#include <algorithm>
namespace opt_utilities
{
  /**
     freeze a set of parameter in model fitting
     \tparam Ty the return type of a model
     \tparam Tx the type of the self-var
     \tparam Tp the type of the model parameter
     \tparam Tstr the type of string used
   */
  template <typename Ty,typename Tx,typename Tp,typename Tstr=std::string>
  class freeze_param
    :public param_modifier<Ty,Tx,Tp,Tstr>
  {
  private:
    std::set<Tstr> param_names;
    std::vector<size_t> param_num;
    size_t num_free;
    
  public:
    /**
       the default construct function
     */
    freeze_param()
    {
    }
    
    /**
       construct function
       \param name the name of the parameter to be frozen
     */
    freeze_param(const Tstr& name)
    {
      param_names.insert(name);
    }
    
  private:
    freeze_param<Ty,Tx,Tp,Tstr>* do_clone()const
    {
      return new freeze_param<Ty,Tx,Tp,Tstr>(*this);
    }
    
    
    /**
       update the parameter information
       should be called by the library, rather than the user
     */
    void update()
    {
      param_num.clear();
      for(typename std::set<Tstr>::const_iterator  i=param_names.begin();
	  i!=param_names.end();++i)
	{
	  try
	    {
	      param_num.push_back(this->get_model().get_param_order(*i));
	    }
	  catch(opt_exception& e)
	    {
	      param_names.erase(*i);
	      throw;
	    }
	  
	}
    }

    size_t do_get_num_free_params()const
    {
      return this->get_model().get_num_params()-param_num.size();
    }

    bool is_frozen(size_t i)const
    {
      if(std::find(param_num.begin(),param_num.end(),i)==param_num.end())
	{
	  return false;
	}
      return true;
    }


    Tp do_reform(const Tp& p)const
    {
      size_t nparams=(this->get_model().get_num_params());
      Tp reformed_p(nparams);
      size_t i=0,j=0;
      for(i=0;i<nparams;++i)
	{
	  if(this->is_frozen(i))
	    {
	      const param_info<Tp,Tstr>& pinf=this->get_model().get_param_info(i);
	      //std::cout<<"frozen:"<<pinf.get_name()
	      //	       <<i<<"\t"<<j
	      //       <<std::endl;
	      //opt_assign(get_element(reformed_p,i),pinf.get_value());
	      set_element(reformed_p,i,pinf.get_value());
	    }
	  else
	    {
	      //opt_assign(get_element(reformed_p,i),get_element(p,j));
	      set_element(reformed_p,i,get_element(p,j));
	      j++;
	    }
	}
      /*
      for(int i=0;i<reformed_p.size();++i)
      {
      std::cout<<get_element(reformed_p,i)<<",";
      }
      */
      //std::cout<<"\n";
      return reformed_p;
      // return p;
    }

    Tp do_deform(const Tp& p)const
    {
      Tp deformed_p(do_get_num_free_params());
      size_t i(0),j(0);
      assert(get_size(p)==this->get_model().get_num_params());
      for(;i<get_size(p);++i)
	{
	  //std::cout<<is_frozen(j)<<"\n";
	  if(!this->is_frozen(i))
	    {
	      //opt_assign(get_element(deformed_p,j),get_element(p,i));
	      set_element(deformed_p,j,get_element(p,i));
	      j++;
	    }
	}
      
      assert(j==do_get_num_free_params());
      return deformed_p;
    }


    Tstr do_report_param_status(const Tstr& name)const
    {
      if(param_names.find(name)==param_names.end())
	{
	  return Tstr("thawed");
	}
      return Tstr("frozen");
    }

  public:
    
    /**
       plus operator, used to combine a list of freeze_param objects
       \param fp another freeze_param object
       \return the combined freeze_param object
     */
    freeze_param operator+(const freeze_param& fp)const
    {
      freeze_param result(*this);
      for(typename std::set<Tstr>::const_iterator i=fp.param_names.begin();
	  i!=fp.param_names.end();
	  ++i)
	{
	  result.param_names.insert(*i);
	}
      return result;
    }

    /**
       Same as operator+
     */
    freeze_param plus(const freeze_param& fp)const
    {
      freeze_param result(*this);
      for(typename std::set<Tstr>::const_iterator i=fp.param_names.begin();
	  i!=fp.param_names.end();
	  ++i)
	{
	  result.param_names.insert(*i);
	}
      return result;
    }


    /**
       += operator
       like the plus operator
     */
    freeze_param& operator+=(const freeze_param& fp)
    {
      //param_names.insert(param_names.end(),
      //fp.param_names.begin(),
      //fp.param_names.end());
      for(typename std::set<Tstr>::const_iterator i=fp.param_names.begin();
	  i!=fp.param_names.end();
	  ++i)
	{
	  param_names.insert(*i);
	}
      try
	{
	  update();
	}
      catch(opt_exception& e)
	{
	  throw;
	}
      return *this;
    }

    /**
       Same as operator+
     */
    freeze_param& plus_eq(const freeze_param& fp)
    {
      //param_names.insert(param_names.end(),
      //fp.param_names.begin(),
      //fp.param_names.end());
      for(typename std::set<Tstr>::const_iterator i=fp.param_names.begin();
	  i!=fp.param_names.end();
	  ++i)
	{
	  param_names.insert(*i);
	}
      try
	{
	  update();
	}
      catch(opt_exception& e)
	{
	  throw;
	}
      return *this;
    }



    /**
       un-freeze a parameter to a list of pre-frozen parameter list
       \param fp the parameter to be un-frozen
       \return the reference to the operated freeze_param object
     */
    freeze_param& operator-=(const freeze_param& fp)
    {
      //param_names.insert(param_names.end(),
      //fp.param_names.begin(),
      //fp.param_names.end());
      for(typename std::set<Tstr>::const_iterator i=fp.param_names.begin();
	  i!=fp.param_names.end();
	  ++i)
	{
	  param_names.erase(*i);
	}
      try
	{
	  update();
	}
      catch(opt_exception& e)
	{
	  throw;
	}
      return *this;
    }

    /**
       Same as operator-=
     */
    freeze_param& minus_eq(const freeze_param& fp)
    {
      //param_names.insert(param_names.end(),
      //fp.param_names.begin(),
      //fp.param_names.end());
      for(typename std::set<Tstr>::const_iterator i=fp.param_names.begin();
	  i!=fp.param_names.end();
	  ++i)
	{
	  param_names.erase(*i);
	}
      try
	{
	  update();
	}
      catch(opt_exception& e)
	{
	  throw;
	}
      return *this;
    }
    
  };
  
  
  /**
     help function to create a freeze_param object
     \param name the name to be frozen
     \return the created freeze_param object
   */
  template <typename Ty,typename Tx,typename Tp,typename Tstr>
  freeze_param<Ty,Tx,Tp,Tstr> freeze(const Tstr& name)
  {
    return freeze_param<Ty,Tx,Tp,Tstr>(name);
  }

}


#endif
//EOF
