#ifndef OPT_IO
#define OPT_IO
#include <iostream>
#include <cmath>
#include "core/fitter.hpp"


namespace opt_utilities
{
  template <typename Ty,typename Tx>
  std::ostream& operator<<(std::ostream& os,const data<Ty,Tx>& d)
  {
    os<<d.get_x()<<"("<<-std::abs(d.get_x_lower_err())<<",+"<<std::abs(d.get_x_upper_err())<<") "<<d.get_y()<<"("<<-std::abs(d.get_y_lower_err())<<",+"<<std::abs(d.get_y_upper_err())<<") ";
    return os;
  }
  
  template <typename Ty,typename Tx>
  std::ostream& operator<<(std::ostream& os,const data_set<Ty,Tx>& ds)
  {
    os<<"data set with size of "<<ds.size()<<"\n"
      <<"[\n";
    int s=5>ds.size()?ds.size():5;
    for(int i=0;i<s;++i)
      {
	os<<ds.get_data(i)<<"\n";
      }
    os<<"...\n";
    os<<"...\n";
    os<<"...\n";
    os<<"]";
    return os;
  }

  template<typename Ty,typename Tx,typename Tp,typename Ts,typename Tstr>
  std::ostream& operator<<(std::ostream& os,const fitter<Ty,Tx,Tp,Ts,Tstr>& f)
  {
    os<<"Fitting session:\n";
    os<<"Model:";
    try
      {
	os<<f.get_model().get_type_name()<<"\n";
      }
    catch(const model_not_defined& e)
      {
	os<<"Undefined\n";
      }
    os<<"Statistic:";
    try
      {
	os<<f.get_statistic().get_type_name()<<"\n";
      }
    catch(const statistic_not_defined& e)
      {
	os<<"Undefined\n";
      }

    os<<"Opt method:";
    try
      {
	os<<f.get_opt_method().get_type_name()<<"\n";
      }
    catch(const opt_method_not_defined& e)
      {
	os<<"Undefined\n";
      }

    os<<"Data set:";
    try
      {
	os<<f.get_data_set().get_type_name()<<"\n";
	os<<f.get_data_set()<<"\n";
      }
    catch(const data_not_loaded& e)
      {
	os<<"Undefined\n";
      }
    return os;
  }
}
#endif
