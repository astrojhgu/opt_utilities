/**
   \file logchisq.hpp
   \brief chi-square statistic
   \author Junhua Gu
 */

#ifndef LOG_CHI_SQ_HPP
#define LOG_CHI_SQ_HPP
#define OPT_HEADER
#include <core/fitter.hpp>
#include <iostream>
#include <vector>
#include <misc/optvec.hpp>
#include <cmath>
using std::cerr;
using std::endl;

namespace opt_utilities
{
    class negative_data_value : public opt_exception
    {
      public:
        negative_data_value ()
        : opt_exception ("log chisq statistics cannot be used when has negative data!")
        {
        }
    };


    /**
       \brief chi-square statistic
       \tparam Ty the return type of model
       \tparam Tx the type of the self-var
       \tparam Tp the type of model parameter
       \tparam Ts the type of the statistic
       \tparam Tstr the type of the string used
     */
    template <typename Tdata, typename Tp, typename Ts, typename Tstr>
    class logchisq : public statistic<Tdata, Tp, Ts, Tstr>
    {
      public:
        typedef typename Tdata::Ty Ty;
        typedef typename Tdata::Tx Tx;

      private:
        bool verb;
        int n;


        statistic<Tdata, Tp, Ts, Tstr> *do_clone () const
        {
            // return const_cast<statistic<Ty,Tx,Tp>*>(this);
            return new logchisq<Tdata, Tp, Ts, Tstr> (*this);
        }

        const char *do_get_type_name () const
        {
            return "chi^2 statistic";
        }

      public:
        void verbose (bool v)
        {
            verb = v;
        }

      public:
        logchisq () : verb (false)
        {
        }


        Ts do_eval (const Tp &p)
        {
            Ts result (0);
            for (int i = (this->get_data_set ()).size () - 1; i >= 0; --i)
                {
                    Ty y = std::log (this->get_data_set ().get_data (i).get_y ());
                    Ty ym = std::log (this->eval_model (this->get_data_set ().get_data (i).get_x (), p));
                    Ty ye1 = std::log (1 + this->get_data_set ().get_data (i).get_y_upper_err () /
                                           this->get_data_set ().get_data (i).get_y ());
                    Ty chi = (y - ym) / ye1;
                    result += chi * chi;
                }
            if (verb)
                {
                    n++;
                    if (n % 10 == 0)
                        {

                            cerr << result << "\t";
                            for (size_t i = 0; i < get_size (p); ++i)
                                {
                                    cerr << get_element (p, i) << ",";
                                }
                            cerr << endl;
                        }
                }

            return result;
        }
    };

#if 1

    template <>
    class logchisq<data<double, double>, std::vector<double>, double, std::string>
    : public statistic<data<double, double>, std::vector<double>, double, std::string>
    {
      public:
        typedef double Ty;
        typedef double Tx;
        typedef std::vector<double> Tp;
        typedef double Ts;
        typedef std::string Tstr;
        typedef data<double, double> Tdata;

      private:
        bool verb;
        int n;

        statistic<Tdata, Tp, Ts, Tstr> *do_clone () const
        {
            // return const_cast<statistic<Ty,Tx,Tp>*>(this);
            return new logchisq<Tdata, Tp, Ts, Tstr> (*this);
        }

        const char *do_get_type_name () const
        {
            return "chi^2 statistics (specialized for double)";
        }

      public:
        void verbose (bool v)
        {
            verb = v;
        }

      public:
        logchisq () : verb (false)
        {
        }


        Ty do_eval (const Tp &p)
        {
            Ty result (0);
            for (int i = 0; i != (this->get_data_set ()).size (); ++i)
                {

                    Ty y_model = this->eval_model (this->get_data_set ().get_data (i).get_x (), p);
                    Ty y_obs = this->get_data_set ().get_data (i).get_y ();
                    Ty y_err;

                    if (y_model > y_obs)
                        {
                            y_err = std::abs (this->get_data_set ().get_data (i).get_y_upper_err ());
                        }
                    else
                        {
                            y_err = -std::abs (this->get_data_set ().get_data (i).get_y_lower_err ());
                        }
                    if (y_obs + y_err < 0)
                        {
                            throw negative_data_value ();
                        }
                    Ty logy = std::log (y_obs);
                    Ty logym = std::log (y_model);
                    Ty logerr = std::log (y_obs + y_err) - log (y_obs);


                    Ty chi = (logy - logym) / logerr;

                    //	  Ty
                    //chi=(this->get_data_set().get_data(i).get_y()-this->eval_model(this->get_data_set().get_data(i).get_x(),p));
                    //	  cerr<<chi<<"\n";
                    result += chi * chi;
                    // std::cerr<<chi<<std::endl;
                    // cerr<<this->eval_model(this->get_data_set()[i].x,p)<<endl;
                    // cerr<<this->get_data_set()[i].y_upper_err<<endl;
                    //	  cerr<<this->get_data_set()[i].x<<"\t"<<this->get_data_set()[i].y<<"\t"<<this->eval_model(this->get_data_set()[i].x,p)<<endl;
                }
            if (verb)
                {
                    n++;
                    if (n % 10 == 0)
                        {
                            cerr << result << "\t";
                            for (int i = 0; i < (int)get_size (p); ++i)
                                {
                                    cerr << get_element (p, i) << ",";
                                }
                            cerr << endl;
                        }
                }

            return result;
        }
    };
#endif
}

#endif
// EOF
