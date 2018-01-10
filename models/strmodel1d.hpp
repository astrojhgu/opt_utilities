/**
   \file strmodel1d.hpp
   \brief evaluating model from string, by usig muparser
   \author Junhua Gu
 */


#ifndef STRMODEL1D_HPP
#define STRMODEL1D_HPP
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>
#include <sstream>
#include <cassert>
#include <muParser.h>
#include <vector>
#include <string>

class strmodel1d : public opt_utilities::model<data<double, double>, std::vector<double>, std::string>
{
  private:
    mu::Parser mp;
    strmodel1d *do_clone () const;
    std::vector<double> par_vec;
    std::vector<std::string> par_names;
    std::string expr;
    std::string var_name;
    double x;
    void set_buildin_fun ();

    const char *do_get_type_name () const
    {
        return "parse a string as a model";
    }

  public:
    double do_eval (const double &x, const std::vector<double> &p);
    strmodel1d ();
    strmodel1d (const strmodel1d &rhs);
    strmodel1d &operator= (const strmodel1d &rhs);


    void set_expr (const std::string &_expr, const std::vector<std::string> &_par_names, const std::string &_var_name);
};


#endif
// EOF
