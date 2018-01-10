#ifndef PRE_ESTIMATER_HPP
#define PRE_ESTIMATER_HPP

#include <core/fitter.hpp>


namespace opt_utilities
{


    template <typename Tdata, typename Tp, typename Tstr = std::string> class pre_estimater
    {
      private:
        std::string model_id;

      private:
        virtual void do_estimate (const data_set<Tdata> &d, model<Tdata, Tp, Tstr> &m) const = 0;
        virtual pre_estimater *do_clone () const = 0;
        virtual void do_destroy ()
        {
            delete this;
        }

      public:
        void estimate (const data_set<Tdata> &d, model<Tdata, Tp, Tstr> &m) const
        {
            do_estimate (d, m);
        }

        virtual ~pre_estimater ()
        {
        }

        pre_estimater *clone () const
        {
            return this->do_clone ();
        }

        void destroy ()
        {
            do_destroy ();
        }

      public:
        std::string get_model_id () const
        {
            return model_id;
        }

        void set_model_id (const std::string &s)
        {
            model_id = s;
        }
    };

    template <typename Tdata, typename Tp, typename Tstr = std::string> class pre_estimatable
    {
      private:
        pre_estimater<Tdata, Tp, Tstr> *ppe;

      public:
        pre_estimatable () : ppe (0)
        {
        }

        pre_estimatable (const pre_estimatable<Tdata, Tp, Tstr> &rhs) : ppe (0)
        {
            if (rhs.ppe)
                {
                    ppe = rhs.ppe->clone ();
                }
        }

        pre_estimatable &operator= (const pre_estimatable<Tdata, Tp, Tstr> &rhs)
        {
            if (this == &rhs)
                {
                    return *this;
                }
            if (ppe)
                {
                    ppe->destroy ();
                }
            ppe = rhs.ppe->clone ();
        }

        void set_pre_estimater (const pre_estimater<Tdata, Tp, Tstr> &pe)
        {
            if (dynamic_cast<model<Tdata, Tp, Tstr> &> (*this).get_type_name () != pe.get_model_id ())
                {
                    return;
                }
            if (ppe)
                {
                    ppe->destroy ();
                }
            ppe = pe.clone ();
        }

        virtual ~pre_estimatable ()
        {
            if (ppe)
                {
                    ppe->destroy ();
                }
        }

      public:
        void estimate (const data_set<Tdata> &d)
        {
            if (ppe)
                {
                    ppe->estimate (d, dynamic_cast<model<Tdata, Tp, Tstr> &> (*this));
                }
        }
    };

    template <typename Tdata, typename Tp, typename Ts, typename Tstr>
    void pre_estimate (fitter<Tdata, Tp, Ts, Tstr> &fit)
    {
        dynamic_cast<pre_estimatable<Tdata, Tp, Tstr> &> (fit.get_model ()).estimate (fit.get_data_set ());
    }
}


#endif
