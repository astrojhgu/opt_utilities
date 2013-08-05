/**
   \file optdl.hpp
   \brief dynamically loadable modules
   \author Junhua Gu
 */


#ifndef OPTDL_HPP
#define OPTDL_HPP
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
//#include <dlfcn.h>
#include <ltdl.h>

namespace opt_utilities
{

  static class dl_init
  {
  public:
    dl_init()
    {
      lt_dlinit();
    }
    ~dl_init()
    {
      //      lt_dlexit();
    }
  }_dl_init;


  template <typename Tdata,
	    typename Tp,
	    typename Tstr>
  model<Tdata,Tp,Tstr>* load_model(const char* fname)
  {
    lt_dlhandle handle;
    handle=lt_dlopen(fname);
    if(!handle)
      {
	throw opt_exception("faild loading object");
      }
    
    
    model<Tdata,Tp,Tstr>* (*func_create)();
    
    func_create=(model<Tdata,Tp,Tstr>* (*)())lt_dlsym(handle,"create_model_object");
    
    if(!func_create)
      {
	throw opt_exception("symble undefined");
      }
    return func_create();
  }
  
  template <typename Ty,
	    typename Tp>
  opt_method<Ty,Tp>* load_opt_method(const char* fname)
  {
    lt_dlhandle handle;
    
    handle=lt_dlopen(fname);
    
    if(!handle)
      {
	throw opt_exception("faild loading object");
      }
    
    
    opt_method<Ty,Tp>* (*func_create)();
    
    func_create=(opt_method<Ty,Tp>* (*)())lt_dlsym(handle,"create_opt_method_object");
    
    if(!func_create)
      {
	throw opt_exception("symble undefined");
      }
    return func_create();
  }

  template <typename Ty,
	    typename Tp>
  func_obj<Ty,Tp>* load_func_obj(const char* fname)
  {
    lt_dlhandle handle;
    
    handle=lt_dlopen(fname);
    
    if(!handle)
      {
	throw opt_exception("faild loading object");
      }
    
    
    func_obj<Ty,Tp>* (*func_create)();
    
    func_create=(func_obj<Ty,Tp>* (*)())lt_dlsym(handle,"create_func_obj_object");
    
    if(!func_create)
      {
	throw opt_exception("symble undefined");
      }
    return func_create();
  }

  template <typename Tdata,
	    typename Tp,
	    typename Ts,
	    typename Tstr>
  statistic<Tdata,Tp,Ts,Tstr>* load_statistic(const char* fname)
  {
    lt_dlhandle handle;
    
    handle=lt_dlopen(fname);
    
    if(!handle)
      {
	throw opt_exception("faild loading object");
      }
    
    
    statistic<Tdata,Tp,Ts,Tstr>* (*func_create)();
    
    func_create=(statistic<Tdata,Tp,Ts,Tstr>* (*)())lt_dlsym(handle,"create_statistic_object");
    
    if(!func_create)
      {
	throw opt_exception("symble undefined");
      }
    return func_create();
  }

}



#endif
//EOF
