/**
   \file opt_types
   \brief define some usually used template
   \author Junhua Gu
 */


#ifndef OPT_TYPES_HPP
#define OPT_TYPES_HPP
#define OPT_HEADER
#include <core/optimizer.hpp>
#include <core/fitter.hpp>
#include <data_sets/default_data_set.hpp>
#include <methods/powell/powell_method.hpp>
#include <statistics/chisq.hpp>
#include <statistics/leastsq.hpp>
#include <vector>

namespace opt_utilities
{
  
  template <typename Ty,typename Tx,typename Tp,typename Ts,typename Tstr>
  class opt_types
  {
  public:
    typedef ::opt_utilities::fitter<data<Ty,Tx>,Tp,Ts,Tstr> fitter;
    typedef ::opt_utilities::chisq<data<Ty,Tx>,Tp,Ts,Tstr> chisq;
    typedef ::opt_utilities::leastsq<data<Ty,Tx>,Tp,Ts,Tstr> leastsq;
    typedef ::opt_utilities::powell_method<Ty,Tp> powell_method;
    typedef ::opt_utilities::model<data<Ty,Tx>,Tp,Tstr> model;
    typedef ::opt_utilities::default_data_set<data<Ty,Tx> > data_set;
    typedef ::opt_utilities::optimizer<Ty,Tp> optimizer;
    typedef ::opt_utilities::func_obj<Ty,Tp> func_obj;
    typedef ::opt_utilities::opt_method<Ty,Tp> opt_method;
  };

  typedef opt_types<double,double,std::vector<double>,double,std::string> dopt;
  
}




#endif
//EOF
