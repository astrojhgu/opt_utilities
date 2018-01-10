/**
   \file default_data_set.hpp
   \brief default implement of data set
   \author Junhua Gu
 */

#ifndef DEFAULT_DATA_SET
#define DEFAULT_DATA_SET
#define OPT_HEADER
#include "core/fitter.hpp"
#include <vector>


namespace opt_utilities
{

    /**
       \brief default implement of the data set
       \tparam Ty type of y
       \tparam Tx type of x
     */
    template <typename Tdata> class default_data_set : public data_set<Tdata>
    {
      public:
        typedef typename Tdata::Tx Tx;
        typedef typename Tdata::Ty Ty;
        //  private:
      public:
        std::vector<Tdata> data_vec;

        data_set<Tdata> *do_clone () const
        {
            return new default_data_set<Tdata> (*this);
        }


        const Tdata &do_get_data (size_t i) const
        {
            return data_vec.at (i);
        }

        size_t do_size () const
        {
            return data_vec.size ();
        }

        void do_add_data (const Tdata &d)
        {
            data_vec.push_back (d);
        }

        void do_clear ()
        {
            data_vec.clear ();
        }

      public:
        default_data_set ()
        {
        }

        default_data_set (const default_data_set<Tdata> &rhs) : data_vec (rhs.data_vec)
        {
        }

        default_data_set (const data_set<Tdata> &rhs)
        {
            data_vec.resize (rhs.size ());
            for (int i = 0; i < data_vec.size (); ++i)
                {
                    data_vec[i] = (rhs.get_data (i));
                }
        }

        default_data_set &operator= (const default_data_set<Tdata> &rhs)
        {
            data_vec = rhs.data_vec;
            return *this;
        }

        default_data_set &operator= (const data_set<Tdata> &rhs)
        {
            data_vec.resize (rhs.size ());
            for (int i = 0; i < data_vec.size (); ++i)
                {
                    data_vec[i](rhs.get_data (i));
                }
            return *this;
        }
    };
}

#endif
// EOF
