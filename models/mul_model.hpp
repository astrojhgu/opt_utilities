/**
   \file mul_model.hpp
   \brief the multiply of two models
   \author Junhua Gu
 */

#ifndef MUL_MODEL_HPP
#define MUL_MODEL_HPP
#define OPT_HEADER
#include <core/fitter.hpp>
#include <cmath>

namespace opt_utilities
{
    template <typename Tdata, typename Tp, typename Tstr>
    class mul_model : public model<Tdata, Tp, Tstr>
    {
      public:
        typedef typename Tdata::Ty Ty;
        typedef typename Tdata::Tx Tx;

      private:
        model<Tdata, Tp, Tstr> *do_clone () const
        {
            return new mul_model<Tdata, Tp, Tstr> (*this);
        }

      private:
        mul_model ()
        {
        }

      private:
        model<Tdata, Tp, Tstr> *pm1;
        model<Tdata, Tp, Tstr> *pm2;

      public:
        mul_model (const model<Tdata, Tp, Tstr> &m1, const model<Tdata, Tp, Tstr> &m2)
        : pm1 (m1.clone ()), pm2 (m2.clone ())
        {
            int np1 = m1.get_num_params ();
            int np2 = m2.get_num_params ();
            for (int i = 0; i < np1; ++i)
                {
                    param_info<Tp, Tstr> p (m1.get_param_info (i));
                    param_info<Tp, Tstr> p1 (p.get_name () + "1", p.get_value ());
                    this->push_param_info (p1);
                }
            for (int i = 0; i < np2; ++i)
                {
                    param_info<Tp, Tstr> p (m2.get_param_info (i));
                    param_info<Tp, Tstr> p2 (p.get_name () + "2", p.get_value ());
                    this->push_param_info (p2);
                }
        }

        mul_model (const mul_model &rhs) : pm1 (NULL), pm2 (NULL)
        {
            int np1 (0), np2 (0);
            if (rhs.pm1)
                {
                    pm1 = rhs.pm1->clone ();
                    np1 = rhs.pm1->get_num_params ();
                    for (int i = 0; i < np1; ++i)
                        {
                            param_info<Tp, Tstr> p (rhs.pm1->get_param_info (i));
                            param_info<Tp, Tstr> p1 (p.get_name () + "1", p.get_value ());
                            this->push_param_info (p1);
                        }
                }
            if (rhs.pm2)
                {
                    pm2 = rhs.pm2->clone ();
                    np2 = rhs.pm2->get_num_params ();
                    for (int i = 0; i < np2; ++i)
                        {
                            param_info<Tp, Tstr> p (rhs.pm2->get_param_info (i));
                            param_info<Tp, Tstr> p2 (p.get_name () + "2", p.get_value ());
                            this->push_param_info (p2);
                        }
                }
        }

        mul_model &operator= (const mul_model &rhs)
        {
            if (this == &rhs)
                {
                    return *this;
                }
            if (!pm1)
                {
                    // delete pm1;
                    pm1->destroy ();
                }
            if (!pm2)
                {
                    // delete pm2;
                    pm2->destroy ();
                }
            int np1 (0), np2 (0);
            if (rhs.pm1)
                {
                    pm1 = rhs.pm1->clone ();
                    np1 = rhs.pm1->get_num_params ();
                    for (int i = 0; i < np1; ++i)
                        {
                            param_info<Tp, Tstr> p (rhs.pm1->get_param_info (i));
                            param_info<Tp, Tstr> p1 (p.get_name () + "1", p.get_value ());
                            this->push_param_info (p1);
                        }
                }
            if (rhs.pm2)
                {
                    pm2 = rhs.pm2->clone ();
                    np2 = rhs.pm2->get_num_params ();
                    for (int i = 0; i < np2; ++i)
                        {
                            param_info<Tp, Tstr> p (rhs.pm2->get_param_info (i));
                            param_info<Tp, Tstr> p2 (p.get_name () + "2", p.get_value ());
                            this->push_param_info (p2);
                        }
                }
            return *this;
        }


        ~mul_model ()
        {
            if (!pm1)
                {
                    // delete pm1;
                    pm1->destroy ();
                }
            if (!pm2)
                {
                    // delete pm2;
                    pm2->destroy ();
                }
        }

      public:
        Ty do_eval (const Tx &x, const Tp &param)
        {
            if (!pm1)
                {
                    throw opt_exception ("incomplete model!");
                }
            if (!pm2)
                {
                    throw opt_exception ("incomplete model!");
                }
            Tp p1 (pm1->get_num_params ());
            Tp p2 (pm2->get_num_params ());
            int i = 0;
            int j = 0;
            for (i = 0; i < pm1->get_num_params (); ++i, ++j)
                {
                    set_element (p1, i, get_element (param, j));
                }
            for (i = 0; i < pm2->get_num_params (); ++i, ++j)
                {
                    set_element (p2, i, get_element (param, j));
                }
            return pm1->eval (x, p1) * pm2->eval (x, p2);
        }
    };

    template <typename Tdata, typename Tp, typename Tstr>
    mul_model<Tdata, Tp, Tstr> operator* (const model<Tdata, Tp, Tstr> &m1, const model<Tdata, Tp, Tstr> &m2)
    {
        return mul_model<Tdata, Tp, Tstr> (m1, m2);
    }
}


#endif
// EOF
