/**
   \file component.hpp
   \brief rpresents a distribution composed of more than one components
   \author Junhua Gu
 */


#ifndef COMPONENT_MODEL_H_
#define COMPONENT_MODEL_H_
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>
#include <misc/optvec.hpp>
#include <sstream>
#include <iostream>



namespace opt_utilities
{
  template <typename T>
  class component
    :public model<data<optvec<T>,optvec<T> >,optvec<T>,std::string>
  {
  private:
    std::vector<model<data<optvec<T>,optvec<T> >,optvec<T>,std::string>*> components;
    std::vector<int> weight_num;
  private:
    component* do_clone()const
    {
      return new component<T>(*this);
    }

    const char* do_get_type_name()const
    {
      return "Multi-distribution";
    }
  public:
    component()
    {
      //this->push_param_info(param_info<optvec<T> >("x0",0));
      //this->push_param_info(param_info<optvec<T> >("sigma",1));
    }

    component(const component& rhs)
    {
      for(int i=0;i<rhs.components.size();++i)
	{
	  if(i>0)
	    {
	      add_component(*rhs.components[i],rhs.get_param_info(rhs.weight_num[i-1]).get_value());
	    }
	  else
	    {
	      add_component(*rhs.components[i]);
	    }
	}
      for(int i=0;i<rhs.get_num_params();++i)
	{
	  set_param_info(rhs.get_param_info(i));
	}
      
    }

    component& operator=(const component& rhs)
    {
      if(this==&rhs)
	{
	  return rhs;
	}
      for(int i=0;i<components.size();++i)
	{
	  components[i]->destroy();
	}
      components.clear();
      for(int i=0;i<rhs.components.size();++i)
	{
	  add_component(*rhs.components[i]);
	}
      for(int i=0;i<rhs.get_num_params();++i)
	{
	  set_param_info(rhs.get_param_info(i));
	}
      
    }

    ~component()
    {
      for(int i=0;i<components.size();++i)
	{
	  components[i]->destroy();
	}
    }


  public:
    void add_component(const model<data<optvec<T>,optvec<T> >,optvec<T>,std::string>& m,const T& w=0)
    {
      int morder=components.size();
      components.push_back(m.clone());
      std::ostringstream oss;
      oss<<morder;
      std::string smorder=oss.str();
      if(components.size()>1)
	{
	  param_info<optvec<T>,std::string> p1(std::string("_w")+smorder,w);	  
	  this->push_param_info(p1);
	  weight_num.push_back(this->get_num_params()-1);
	}

      int np=m.get_num_params();
      for(int i=0;i<np;++i)
	{
	  param_info<optvec<T>,std::string> p(m.get_param_info(i));
	  param_info<optvec<T>,std::string> p1(p.get_name()+smorder,p.get_value());
	  this->push_param_info(p1);
	}
    }

    optvec<T> convert_unit_sph(const optvec<T>& p)
    {
      int ndim=p.size()+1;
      optvec<double> result(ndim);
      result[0]=1;
      for(int i=0;i<p.size();++i)
	{
	  for(int j=0;j<=i;++j)
	    {
	      result[j]*=cos(p[i]);
	    }
	  result[i+1]=sin(p[i]);
	}
      for(int i=0;i<result.size();++i)
	{
	  result[i]=result[i]*result[i];
	}
      return result;
    }

    
    optvec<T> do_eval(const optvec<T>& x,const optvec<T>& param)
    {
      T result(0);
      optvec<T> weight_angle;
      for(int i=0;i<weight_num.size();++i)
	{
	  weight_angle.push_back(param[weight_num[i]]);
	}
      optvec<T> weight(convert_unit_sph(weight_angle));
      
      int pnum=0;
      for(int i=0;i<components.size();++i)
	{
	  T temp_result=0;
	  optvec<T> p(components[i]->get_num_params());
	  for(int j=0;j<p.size();++j)
	    {
	      p[j]=param[pnum++];
	    }
	  result+=(components[i]->eval(x,p)[0]*weight[i]);
	  ++pnum;
	}
      optvec<T> result1(1);
      result1[0]=result;;
      return result1;
    }

  private:
    std::string do_get_information()const
    {
      return "";
    }
  };
}



#endif
//EOF
