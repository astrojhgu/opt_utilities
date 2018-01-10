/**
   \file vecn.hpp
   \brief n-dim fixed size vector
   \author Junhua Gu
 */


#ifndef VECN_HPP
#define VECN_HPP
#define OPT_HEADER
#include <iostream>
namespace opt_utilities
{
    template <typename T, int n> class vecn
    {
      public:
        T data[n];

      public:
        T &operator[] (int i)
        {
            return data[i];
        }

        const T &operator[] (int i) const
        {
            return data[i];
        }

        vecn ()
        {
            for (int i = 0; i < n; ++i)
                {
                    data[i] = 0;
                }
        }
    };

    template <typename T, int n> std::istream &operator>> (std::istream &is, vecn<T, n> &p)
    {
        for (int i = 0; i < n; ++i)
            {
                is >> p[i];
                //	std::cout<<i<<std::endl;
            }
        return is;
    }


    template <typename T, int n> std::ostream &operator<< (std::ostream &os, const vecn<T, n> &p)
    {
        os << '[';
        for (int i = 0; i < n; ++i)
            {
                os << p[i] << ",";
                //	std::cout<<i<<std::endl;
            }
        os << ']';
        return os;
    }
}

#endif
// EOF
