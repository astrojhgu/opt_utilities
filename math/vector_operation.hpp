/**
   \file vector_operation.hpp
   \brief defing vector operators
   \author Junhua Gu
 */


#ifndef VECTOR_OPERATION_HPP
#define VECTOR_OPERATION_HPP
#include <core/opt_traits.hpp>
namespace opt_utilities
{

    template <typename pT>
    typename element_type_trait<pT>::element_type inner_product (const pT &v1, const pT &v2)
    {
        typename element_type_trait<pT>::element_type result (0);
        for (int i = 0; i < get_size (v1); ++i)
            {
                result += get_element (v1, i) * get_element (v2, i);
            }
        return result;
    }

    template <typename T> T contract1 (const T &x1, const T &x2, ...)
    {
        return x1 * x2;
    }

    template <typename T> T contract (const T &x1, const T &x2, ...)
    {
        return x1 * x2;
    }

    template <typename pT>
    typename element_type_trait<pT>::element_type
    contract (const pT &v1, const pT &v2, const typename element_type_trait<pT>::element_type &)
    {
        typename element_type_trait<pT>::element_type result (0);
        for (int i = 0; i < get_size (v1); ++i)
            {
                result += get_element (v1, i) * get_element (v2, i);
            }
        return result;
    }
}
#endif
