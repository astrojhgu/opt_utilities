/**
   \file sorted_data_set.hpp
   \brief a data set with data sorted by x
   \author Junhua Gu
*/

#ifndef SORTED_DATA_SET
#define SORTED_DATA_SET
#define OPT_HEADER
#include "core/fitter.hpp"
#include <vector>
#include <algorithm>

namespace opt_utilities
{
  
  template <typename Tdata>
  bool comp_data(const Tdata& d1,const Tdata& d2)
  {
    return d1.get_x()<d2.get_x();
  }



  /**
     \brief automatically sorting data set
     \tparam Ty type of y
     \tparam Tx type of x
   */
  template <typename Tdata>
  class sorted_data_set
    :public data_set<Tdata>
  {
  private:
    std::vector<Tdata > data_vec;
    
    data_set<Tdata>* do_clone()const
    {
      return new sorted_data_set<Tdata>(*this);
    }
    
    
    const Tdata& do_get_data(size_t i)const
    {
      return data_vec.at(i);
    }
    
    size_t do_size()const
    {
      return data_vec.size();
    }
    
    void do_add_data(const Tdata& d)
    {
      typename std::vector<Tdata >::iterator p
	=std::lower_bound(data_vec.begin(),data_vec.end(),d,comp_Tdata);
      //data_vec.push_back(d);
      data_vec.insert(p,d);
    }
    
    void do_clear()
    {
      data_vec.clear();
    }
    
  public:
    sorted_data_set()
    {}

    sorted_data_set(const sorted_data_set<Tdata>& rhs)
      :data_vec(rhs.data_vec)
    {}

    sorted_data_set& operator=(const sorted_data_set<Tdata>& rhs)
    {
      data_vec=rhs.data_vec;
      return *this;
    }

    sorted_data_set(const data_set<Tdata>& rhs)
    {
      for(int i=0;i<rhs.size();++i)
	{
	  typename std::vector<Tdata >::iterator p
	    =std::lower_bound(data_vec.begin(),data_vec.end(),rhs.get_data(i),comp_Tdata);
	  //data_vec.push_back(d);
	  data_vec.insert(p,rhs.get_data(i));
	}
    }

    sorted_data_set& operator=(const data_set<Tdata>& rhs)
    {
      data_vec.clear();
      for(int i=0;i<rhs.size();++i)
	{
	  typename std::vector<Tdata >::iterator p
	    =std::lower_bound(data_vec.begin(),data_vec.end(),rhs.get_data(i),comp_Tdata);
	  //data_vec.push_back(d);
	  data_vec.insert(p,rhs.get_data(i));
	}
      return *this;
    }
    
  };
}

#endif
//EOF
