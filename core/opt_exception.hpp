/**
   \file opt_exception.hpp
   \brief opt_utilities exceptions
   \author Junhua Gu
*/

#ifndef OPT_EXCEPTION
#define OPT_EXCEPTION
#define OPT_HEADER
#include <string>
#include <exception>
namespace opt_utilities
{
    /**
       \brief the root class of exception used in opt_utilities
     */
    class opt_exception : public std::exception
    {
      private:
        std::string _what;

      public:
        opt_exception (){};

        ~opt_exception () throw ()
        {
        }

        opt_exception (const std::string &str) : _what (str)
        {
        }

        const char *what () const throw ()
        {
            return _what.c_str ();
        }
    };

    /**
       When objection is not defined in optimizer, and optimizion is performing,
       this exception will be thrown
     */
    class object_function_not_defined : public opt_exception
    {
      public:
        object_function_not_defined () : opt_exception ("object function undefined")
        {
        }
    };

    /**
       When the opt_method is not defined before the optimization is performing,
       this exception will be thrown.
     */
    class opt_method_not_defined : public opt_exception
    {
      public:
        opt_method_not_defined () : opt_exception ("opt method undefined")
        {
        }
    };

    /**
       When fitter is not attached in a model, statistic, and other objects,
       this exception will be thrown.
     */
    class fitter_not_set : public opt_exception
    {
      public:
        fitter_not_set () : opt_exception ("fitter_unset")
        {
        }
    };


    /**
       When the model is not set before the model fitting,
       this exception will be thrown.
     */
    class model_not_defined : public opt_exception
    {
      public:
        model_not_defined () : opt_exception ("model_undefined")
        {
        }
    };


    /**
       When the data set is not loaded before the model fitting,
       this exception will be thrown.
     */
    class data_not_loaded : public opt_exception
    {
      public:
        data_not_loaded () : opt_exception ("data not loaded")
        {
        }
    };


    /**
       When the statistic is not set before the model fitting,
       this exception will be thrown.
     */
    class statistic_not_defined : public opt_exception
    {
      public:
        statistic_not_defined () : opt_exception ("statistic undefined")
        {
        }
    };


    /**
       If a parameter is not found by the name or other information,
       this exception will be thrown.
     */
    class param_not_found : public opt_exception
    {
      public:
        param_not_found () : opt_exception ("param name invalid")
        {
        }
    };


    /**
       If param_modifier is not defined, and the user tries to get the
       param_modifer, this exception will be thrown.
     */
    class param_modifier_not_defined : public opt_exception
    {
      public:
        param_modifier_not_defined () : opt_exception ("param modifier undefined")
        {
        }
    };

    class data_unsetable : public opt_exception
    {
      public:
        data_unsetable () : opt_exception ("data unsetable")
        {
        }
    };
}


#endif
// EOF
