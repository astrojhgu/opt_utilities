/**
   \file pymodel.hpp
   \brief model wrapper of python functions
   \author Junhua Gu
 */


#ifndef PYMODEL
#define PYMODEL
#include <boost/python.hpp>
#include <core/fitter.hpp>
#include <core/opt_traits.hpp>


namespace opt_utilities
{

    template <typename T> boost::python::object convert_to_object (const T &x)
    {
        boost::python::list result;
        for (size_t i = 0; i < get_size (x); ++i)
            {
                result.append (get_element (x, i));
            }
        return result;
    }

    static inline boost::python::object convert_to_object (double x)
    {
        return boost::python::object (x);
    }

    static inline boost::python::object convert_to_object (float x)
    {
        return boost::python::object (x);
    }


    template <typename T> T convert_from_object (const boost::python::object &o, const T &)
    {
        T result (boost::python::len (o));
        for (int i = 0; i < get_size (result); ++i)
            {
                result[i] = boost::python::extract<typename element_type_trait<T>::element_type> (o[i]);
            }
        return result;
    }

    static inline double convert_from_object (const boost::python::object &o, double)
    {
        return boost::python::extract<double> (o);
    }

    static inline double convert_from_object (const boost::python::object &o, float)
    {
        return boost::python::extract<double> (o);
    }


    template <typename Tdata, typename Tp> class pymodel : public model<Tdata, Tp, std::string>
    {
      public:
        typedef typename Tdata::Ty Ty;
        typedef typename Tdata::Tx Tx;

      private:
        boost::python::object pyfunc;
        std::string type_name;

      public:
        pymodel ()
        {
            if (!Py_IsInitialized ())
                {
                    Py_Initialize ();
                }
        }

        pymodel (const pymodel &rhs) : pyfunc (rhs.pyfunc)
        {
            for (int i = 0; i < rhs.get_num_params (); ++i)
                {
                    this->push_param_info (rhs.get_param_info (i));
                }
        }

        pymodel &operator= (const pymodel &rhs)
        {
            if (this == &rhs)
                {
                    return *this;
                }
            pyfunc = rhs.pyfunc;
            for (int i = 0; i < rhs.get_num_params (); ++i)
                {
                    this->push_param_info (rhs.get_param_info (i));
                }
            return *this;
        }

        ~pymodel ()
        {
        }

      public:
        void attach (const std::string module_name,
                     const std::string arg_name,
                     const std::string arg_value,
                     const std::string func_name)
        {
            type_name = module_name + "." + func_name;
            this->clear_param_info ();
            boost::python::object mod (boost::python::import (module_name.c_str ()));
            pyfunc = mod.attr (func_name.c_str ());
            boost::python::list args_names (mod.attr (arg_name.c_str ()));
            boost::python::list args_values (mod.attr (arg_value.c_str ()));

            int nparams = boost::python::len (args_names);
            for (size_t i = 0; i != nparams; ++i)
                {
                    boost::python::object pname_obj = args_names[i];
                    std::string pname = boost::python::extract<std::string> (pname_obj);
                    typename element_type_trait<Tp>::element_type pvalue =
                    boost::python::extract<typename element_type_trait<Tp>::element_type> (args_values[i]);

                    this->push_param_info (param_info<Tp, std::string> (pname, pvalue));
                }
        }

      private:
        model<Tdata, Tp, std::string> *do_clone () const
        {
            return new pymodel (*this);
        }

        void do_destroy ()
        {
            delete this;
        }

        Ty do_eval (const Tx &x, const Tp &p)
        {
            boost::python::list args;
            for (size_t i = 0; i < get_size (p); ++i)
                {
                    args.append (get_element (p, i));
                }
            return convert_from_object (pyfunc (convert_to_object (x), args), Ty ());
        }

        const char *do_get_type_name () const
        {
            return type_name.c_str ();
        }
    };
}


#endif
// EOF
