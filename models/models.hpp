/**
   \file models.hpp
   \brief implement of the prototype pattern to get models from string keywords
   \author Junhua Gu
 */


#ifndef MODELS_HPP
#define MODELS_HPP
#define OPT_HEADER
#include <core/fitter.hpp>
#include <map>
#include <string>
#include <list>
#include "vecn.hpp"



namespace opt_utilities
{
  extern std::map<std::string,model<data<double,double>,std::vector<double>,std::string>* > model_map;

  extern std::list<std::string> get_model_name_list();
  extern int get_n_1dmodels();
  //  extern void init_model_map();
  //  extern void release_model_map();
  
  extern model<data<double,double>,std::vector<double>,std::string >& get_1dmodel_by_name(const char*);
  extern const char* get_model_name(int n);
}


#endif
