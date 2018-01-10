/**
   \file kmm_stat.hpp
   \brief maximum-liklihood statistic
   \author Junhua Gu
 */

#ifndef KMM_STAT_HPP
#define KMM_STAT_HPP
#define OPT_HEADER
#include <core/fitter.hpp>
#include <math/vector_operation.hpp>
#include <distributions/kmm_component.hpp>
#include <iostream>
#include <cassert>


using std::cout;
using std::endl;
namespace opt_utilities
{

    /**
       \brief c-statistic, max-likelihood method
       \tparam Ty the return type of model
       \tparam Tx the type of the self-var
       \tparam Tp the type of model parameter
       \tparam Ts the type of the statistic
       \tparam Tstr the type of the string used
     */
    template <typename T, typename Ts, typename Tstr>
    class kmm_stat : public statistic<data<optvec<T>, optvec<T>>, optvec<T>, Ts, Tstr>
    {
      private:
        bool verb;
        int n;

      public:
        kmm_stat () : verb (true)
        {
        }

        void verbose (bool v)
        {
            verb = v;
        }

        const char *do_get_type_name () const
        {
            return "kmm likelihood";
        }

      public:
        statistic<data<optvec<T>, optvec<T>>, optvec<T>, Ts, Tstr> *do_clone () const
        {
            // return const_cast<statistic<Ty,Tx,Tp>*>(this);
            return new kmm_stat<T, Ts, Tstr> (*this);
        }

        Ts do_eval (const optvec<T> &p)
        {
            Ts result (0);

            kmm_component<T> *kmm = dynamic_cast<kmm_component<T> *> (
            &const_cast<model<data<optvec<T>, optvec<T>>, optvec<T>, std::string> &> (
            this->get_fitter ().get_model ()));

            if (kmm == 0)
                {
                    throw opt_exception ("model is not a kmm component");
                }

            for (int i = (this->get_data_set ()).size () - 1; i >= 0; --i)
                {
                    optvec<T> unsummed_possibility (
                    kmm->eval_unsumed (this->get_data_set ().get_data (i).get_x (), p));
                    T maxp = *max_element (unsummed_possibility.begin (), unsummed_possibility.end ());
                    T logp = std::log (maxp);
                    result -= this->get_data_set ().get_data (i).get_y ()[0] * logp;
                }

            if (verb)
                {
                    n++;
                    if (n % 10 == 0)
                        {
                            cout << "a:" << result << "\t";
                            for (size_t i = 0; i < get_size (p); ++i)
                                {
                                    cout << get_element (p, i) << ",";
                                }
                            cout << endl;
                        }
                }
            return result;
        }
    };
}

#endif
// EOF
