/**
   \file data_loaders.hpp
   \brief a set of classes used to load data_set from std::istream
   \author Junhua Gu
 */

#ifndef DATA_LOADERS_H
#define DATA_LOADERS_H
#define OPT_HEADER
#include <core/fitter.hpp>
#include <data_sets/default_data_set.hpp>
#include <iostream>
#include <fstream>
#include <cmath>

namespace opt_utilities
{
  template <typename Ty,typename Tx>
  class dl_x_y_ye;

  template <typename Ty,typename Tx>
  std::istream& operator>>(std::istream& ifs,dl_x_y_ye<Ty,Tx>& dl);

  template <typename Ty,typename Tx>
  class dl_x_xe_y_ye;


  template <typename Ty,typename Tx>
  std::istream& operator>>(std::istream& ifs,dl_x_xe_y_ye<Ty,Tx>& dl);


  template <typename Ty,typename Tx>
  class dl_x_xu_xl_y_yu_yl;
  

  template <typename Ty,typename Tx>
  std::istream& operator>> (std::istream& ifs,dl_x_xu_xl_y_yu_yl<Ty,Tx>& dl);


  /**
     loading data from file in format [x] [y] [y error]
   */
  template <typename Ty,typename Tx>
  class dl_x_y_ye
  {
  private:
    default_data_set<data<Ty,Tx> > ds;
  public:
    data_set<data<Ty,Tx> >& get_data_set()
    {
      return ds;
    }

    void load_from(std::istream& ifs)
    {
      for(;;)
	{
	  Tx x;
	  Tx x_err;
	  Ty y;
	  Ty y_err(1);
	  
	  ifs>>x>>y>>y_err;
	  
	  if(!ifs.good())
	    {
	      break;
	    }
	  data<Ty,Tx> d(x,y,y_err,y_err,x_err,x_err);
	  ds.add_data(d);
	}
      //return ifs;
    }

    void load_from(const char* name)
    {
      std::ifstream ifs(name);
      load_from(ifs);

    }
    //friend std::istream& operator>> <>(std::istream& ifs,dl_x_y_ye<Ty,Tx>& dl);
  };



  /**
     stream operator
   */
  template <typename Ty,typename Tx>
  std::istream& operator>>(std::istream& ifs,dl_x_y_ye<Ty,Tx>& dl)
  {
    dl.load_from(ifs);
    return ifs;
  }


  /**
     loading data from stream with file 

     [x] [x error] [y] [y error]
   */
  template <typename Ty,typename Tx>
  class dl_x_xe_y_ye
  {
  private:
    default_data_set<data<Ty,Tx> > ds;
  public:
    data_set<data<Ty,Tx> >& get_data_set()
    {
      return ds;
    }

    void load_from(std::istream& ifs)
    {
      for(;;)
	{
	  Tx x;
	  Tx x_err;
	  Ty y;
	  Ty y_err(1);
	  
	  ifs>>x>>x_err>>y>>y_err;
	  
	  if(!ifs.good())
	    {
	      break;
	    }
	  data<Ty,Tx> d(x,y,y_err,y_err,x_err,x_err);
	  ds.add_data(d);
	}
    }

    void load_from(const char* name)
    {
      std::ifstream ifs(name);
      load_from(ifs); 
    }
  };


  /**
     stream operator
   */
  template <typename Ty,typename Tx>
  std::istream& operator>>(std::istream& ifs,dl_x_xe_y_ye<Ty,Tx>& dl)
  {
    dl.load_from(ifs);
    return ifs;
  }


  /**
     loading data from stream with format 

     [x] [x lower error] [x upper error] [y] [y lower error] [y upper error]
   */
  template <typename Ty,typename Tx>
  class dl_x_xu_xl_y_yu_yl
  {
  private:
  
    default_data_set<data<Ty,Tx> > ds;
  public:
    data_set<data<Ty,Tx> >& get_data_set()
    {
      return ds;
    }
    
    void load_from(std::istream& ifs)
    {
      for(;;)
	{
	  Tx x;
	  Tx xl,xu;
	  Ty y;
	  Ty yl(1),yu(1);
	  
	  ifs>>x>>xu>>xl>>y>>yu>>yl;
	  
	  xu=std::abs(xu);
	  xl=std::abs(xl);
	  yu=std::abs(yu);
	  yl=std::abs(yl);
	  
	  if(!ifs.good())
	    {
	      break;
	    }
	  data<Ty,Tx> d(x,y,yl,yu,xl,xu);
	  ds.add_data(d);
	}
    }

    void load_from(const char* name)
    {
      std::ifstream ifs(name);
      load_from(ifs);
    }
  };


  /**
     stream operator
   */
  template <typename Ty,typename Tx>
  std::istream& operator>> (std::istream& ifs,dl_x_xu_xl_y_yu_yl<Ty,Tx>& dl)
  {
    dl.load_from(ifs);
    return ifs;
  }

  

}


#endif
//EOF
