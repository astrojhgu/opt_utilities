#include "models.hpp"
#include <core/opt_exception.hpp>
#include "gauss1d.hpp"
#include "bl1d.hpp"
#include "nfw1d.hpp"
#include "bpl1d.hpp"
#include "beta1d.hpp"
#include "nbeta1d.hpp"
#include "dbeta1d.hpp"
#include "lin1d.hpp"
#include "pl1d.hpp"
#include "poly1d.hpp"
#include "bremss.hpp"
#include <iostream>
using namespace std;


namespace opt_utilities
{
  std::map<std::string,model<data<double,double>,std::vector<double>,std::string>* > model_map;

  std::list<std::string> get_model_name_list()
  {
    std::list<std::string> result;
    for(std::map<std::string,model<data<double,double>,std::vector<double>,std::string>* >::iterator i=model_map.begin();
	i!=model_map.end();++i)
      {
	result.push_back(i->first);
      }
	return result;
  }

  const char* get_model_name(int n)
  {
    int cnt=0;
    for(std::map<std::string,model<data<double,double>,std::vector<double>,std::string>* >::iterator i=model_map.begin();
	i!=model_map.end();++i)
      {
	if(cnt++==n)
	  {
	    return i->first.c_str();
	  }
      }
  }

  model<data<double,double>,std::vector<double>,std::string>& get_1dmodel_by_name(const char* name)
  {
    std::map<std::string,model<data<double,double>,std::vector<double>,std::string >* >::iterator iter;
    iter=model_map.find(name);
    if(iter==model_map.end()||iter->second==0)
      {
	throw opt_exception("model does not exist");
      }
    return *(iter->second);
  }


  int get_n_1dmodels()
  {
    return model_map.size();
  }

  class model_map_keeper_class
  {
  private:
    void init_model_map()
    {

      //#define DECL_POLY(n) model_map["poly1d##n"]=new poly1d<double,n>;


      model_map["lin1d"]=new lin1d<double>;
      model_map["pl1d"]=new pl1d<double>;
      model_map["bl1d"]=new bl1d<double>;
      model_map["bpl1d"]=new bpl1d<double>;
      model_map["beta1d"]=new beta1d<double>;  
      model_map["bremss"]=new bremss<double>;
      model_map["nbeta1d"]=new nbeta1d<double>;  
      model_map["2beta1d"]=new dbeta1d<double>;
      model_map["nfw"]=new nfw1d<double>;
      model_map["gauss1d"]=new gauss1d<double>;  
      model_map["poly1d2"]=new poly1d<double,2>;
      model_map["poly1d3"]=new poly1d<double,3>;
      model_map["poly1d4"]=new poly1d<double,4>;
      model_map["poly1d5"]=new poly1d<double,5>;
      //DECL_POLY(7)
    }
    
    void release_model_map()
    {
      for(std::map<std::string,model<data<double,double>,std::vector<double>,std::string>* >::iterator i=model_map.begin();
	  i!=model_map.end();++i)
	{
	  delete i->second;
	}
    }
  public:
    model_map_keeper_class()
    {
      init_model_map();
	  std::cerr<<"1d models Initialized"<<std::endl;
    }
    
    ~model_map_keeper_class()
    {
      release_model_map();
	  std::cerr<<"1d models Released"<<std::endl;
    }

  }model_map_keeper;

}

