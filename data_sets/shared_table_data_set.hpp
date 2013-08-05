/**
   \file shared_table_data_set.hpp
   \brief A data set with high copy efficiency
   \author Junhua Gu
 */

#ifndef SHARED_TABLE_DATA_SET
#define SHARED_TABLE_DATA_SET
#define OPT_HEADER
#include "core/fitter.hpp"
#include <vector>


namespace opt_utilities
{

  /**
     \brief shared_table implement of the data set
     When the shared_table_data_set clones self, it doesn't
     allocate a new instance, but returns a pointer to self.
     \tparam Ty type of y
     \tparam Tx type of x
   */
  template <typename Tdata>
  class shared_table_data_set
    :public data_set<Tdata>
  {
    //  private:
  public:
    std::vector<data<Tdata> > data_vec;
    
    /**
       Only returns a pointer to self
     */
    data_set<Tdata>* do_clone()const
    {
      return (data_set<Tdata>*)(this);
    }

    /**
       We do nothing here.
     */
    void do_destroy()
    {
    }


    const data<Tdata>& do_get_data(size_t i)const
    {
      return data_vec.at(i);
    }

    void do_set_data(size_t i,const data<Tdata>& d)
    {
      data_vec.at(i)=d;
    }
  
    size_t do_size()const
    {
      return data_vec.size();
    }
  
    void do_add_data(const data<Tdata>& d)
    {
      data_vec.push_back(d);
    }
  
    void do_clear()
    {
      data_vec.clear();
    }

    bool insert_data(int idx,const data<Tdata>& d)
    {
      if(idx<0||idx>data_vec.size())
	{
	  return false;
	}
      data_vec.insert(data_vec.begin()+idx,d);
      return true;
    }

    bool insert_data(int idx,size_t n,const data<Tdata>& d)
    {
      if(idx<0||idx>data_vec.size())
	{
	  return false;
	}
      data_vec.insert(data_vec.begin()+idx,n,d);
      return true;
    }
    

    bool erase_data(size_t idx)
    {
      if(idx>=0&&idx<data_vec.size())
	{
	  data_vec.erase(data_vec.begin()+idx);
	  return true;
	}
      return false;
    }

    bool erase_data(size_t beg,size_t end)
    {
      if(beg>=0&&beg<=end&&end<=data_vec.size())
	{
	  data_vec.erase(data_vec.begin()+beg,data_vec.begin()+end);
	  return true;
	}
      return false;
    }

  public:
    shared_table_data_set()
    {}
    
    shared_table_data_set(const shared_table_data_set<Tdata>& rhs)
      :data_vec(rhs.data_vec)
    {}

    shared_table_data_set(const data_set<Tdata>& rhs)
    {
      data_vec.resize(rhs.size());
      for(int i=0;i<data_vec.size();++i)
	{
	  data_vec[i](rhs.get_data(i));
	}
    }

    shared_table_data_set& operator=(const shared_table_data_set<Tdata>& rhs)
    {
      data_vec=rhs.data_vec;
      return *this;
    }

    shared_table_data_set& operator=(const data_set<Tdata>& rhs)
    {
      data_vec.resize(rhs.size());
      for(int i=0;i<data_vec.size();++i)
	{
	  data_vec[i](rhs.get_data(i));
	}
      return *this;
    }

    void reserve(size_t n)
    {
      data_vec.reserve(n);
    }

    
  };
}

#endif
//EOF
