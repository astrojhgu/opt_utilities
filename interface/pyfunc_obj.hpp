/**
   \file pyfunc_obj.hpp
   \brief function object wrappers for python functions
   \author Junhua Gu
 */


#ifndef PYFUNC_OBJ_HPP
#define PYFUNC_OBJ_HPP
#include <boost/python.hpp>
#include <core/optimizer.hpp>
#include <core/opt_traits.hpp>


namespace opt_utilities
{

    template <typename Ty, typename Tx> class pyfunc_obj : public func_obj<Ty, Tx>
    {
      private:
        boost::python::object pyfunc;

      public:
        pyfunc_obj ()
        {
            if (!Py_IsInitialized ())
                {
                    Py_Initialize ();
                }
        }

        pyfunc_obj (const pyfunc_obj &rhs) : pyfunc (rhs.pyfunc)
        {
        }

        pyfunc_obj &operator= (const pyfunc_obj &rhs)
        {
            pyfunc = rhs.pyfunc;
        }

        ~pyfunc_obj ()
        {
        }

      public:
        void attach (const std::string module_name, const std::string func_name)
        {
            boost::python::object mod (boost::python::import (module_name.c_str ()));
            pyfunc = mod.attr (func_name.c_str ());
        }

      private:
        func_obj<Ty, Tx> *do_clone () const
        {
            return new pyfunc_obj (*this);
        }

        void do_destroy ()
        {
            delete this;
        }

        Ty do_eval (const Tx &x)
        {
            boost::python::list args;
            for (size_t i = 0; i < get_size (x); ++i)
                {
                    args.append (get_element (x, i));
                }
            return boost::python::extract<Ty> (pyfunc (args));
        }
    };
}


#endif
// EOF
