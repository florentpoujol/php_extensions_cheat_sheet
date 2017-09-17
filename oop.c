


// method
PHP_METHOD(ClassName, methodName) /* {{{ */
{
    if (zend_parse_parameters_none() == FAILURE) { // or zend_parse_parameters
        return;
    }

    RETURN_STRING("Hello World\n", 1);
}
/* }}} */


ZEND_BEGIN_ARG_INFO_EX(arginfo_void, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_methodName, 0, 0, 0)
    // bla
ZEND_END_ARG_INFO()

const zend_function_entry className_functions[] = {
    PHP_ME(ClassName, methodName, arginfo_methodName, ZEND_ACC_PUBLIC)
    PHP_ABSTRACT_ME(ClassName, abstractMethod, arginfo_abc) // for abstract method
    PHP_FE_END
};
// last param of PHP ME is a flag:
// ZEND_ACC_PUBLIC  ZEND_ACC_PRIVATE  ZEND_ACC_PROTECTED
// ZEND_ACC_STATIC  ZEND_ACC_FINAL    ZEND_ACC_ABSTRACT
// ie   ZEND_ACC_PUBLIC | ZEND_ACC_FINAL


// global variable that hodls the class
zend_class_entry *className_ce; // respresents the scope

PHP_MINIT_FUNCTION(extensionname)
{
    zend_class_entry tmp_ce;
    INIT_CLASS_ENTRY(tmp_ce, "ClassName", className_functions);

    className_ce = zend_register_internal_class(&tmp_ce TSRMLS_CC);

    // add here the properties declaration
    zend_declare_property(...);




    return SUCCESS;
}

// inside a method :

zval *obj;
obj = getThis();

Z_OBJ_HANDLE_P(obj) // returns a long


// declare properties and default values
zend_declare_property(class_entry, "propertyname", sizeof("propertyName") - 1, value, visibility_flag TSRMLS_CC);
zend_declare_property_[type]();
zend_declare_property_null(test_ce, "foo", sizeof("foo") - 1, ZEND_ACC_PUBLIC TSRMLS_CC);
...
zend_declare_property_string(test_ce, "foo", sizeof("foo") - 1, char *value, ZEND_ACC_PUBLIC TSRMLS_CC);
// add these inside MININT after the class declaration


// get property
zval *value_to_get;
zval *obj;
obj = getThis();
value_to_get = zend_read_property(className_ce, obj, "propertyName", sizeof("propertyName") - 1, 1 TSRMLS_CC) // last params is "silent", define if the "Undefined property" notice should be thown

// set property
zval *obj, *value_to_set;
obj = getThis();
if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &value_to_set) == FAILURE) {
    return;
}
zend_update_property(className_ce, obj, "propertyName", sizeof("propertyName") - 1, value_to_set TSRMLS_CC);
// the first arg is actually optional when the property visibility is public

zend_update_property_null(... TSRMLS_DC)
zend_update_property_bool(..., long value TSRMLS_DC)
zend_update_property_long(..., long value TSRMLS_DC)
zend_update_property_double(..., double value TSRMLS_DC)
zend_update_property_string(..., const char *value TSRMLS_DC)
zend_update_property_stringl(..., const char *value, int value_len TSRMLS_DC)
// ...  is the same as:   classEntry, obj, propertyname, sizeof propertyName

// read/write static properties
zend_read_static_property()
zend_update_static_property()


// ==========================
// inheritance and exception


zend_class_entry *custom_exception_ce;

PHP_MINIT_FUNCTION(extensionname)
{
    zend_class_entry tmp_ce;
    INIT_CLASS_ENTRY(tmp_ce, "CustomException", NULL); // the NULL argument here could have been the function list, here the new class is created empty

    // inherits from Exception
    custom_exception_ce = zend_register_internal_class_ex(
        &tmp_ce, zend_exception_get_default(TSRMLS_C), NULL TSRMLS_CC 
    );

    // without inheritance
    zend_class_entry tmp_ce;
    INIT_CLASS_ENTRY(tmp_ce, "ClassName", className_functions);
    className_ce = zend_register_internal_class(&tmp_ce TSRMLS_CC);


    return SUCCESS;
}

// inherit from a class defined elsewhere
#ifdef HAVE_SPL
#include "ext/spl/spl_exceptions.h"
#endif

zend_class_entry *custom_exception_ce;

PHP_MINIT_FUNCTION(Test)
{
    zend_class_entry tmp_ce;
    INIT_CLASS_ENTRY(tmp_ce, "CustomException", NULL);

    custom_exception_ce = zend_register_internal_class_ex(
        &tmp_ce, spl_ce_RuntimeException, NULL TSRMLS_CC
    );
    // can also be written like so if the class_entry is not accesible
    custom_exception_ce = zend_register_internal_class_ex(
        &tmp_ce, NULL, "RuntimeException" TSRMLS_CC
    );

    return SUCCESS;
}


// interface

#include "ext/spl/spl_iterators.h"
#include "zend_interfaces.h"

zend_class_entry *data_class_ce;

PHP_METHOD(DataClass, count) { /* ... */ }

const zend_function_entry data_class_functions[] = {
    PHP_ME(DataClass, count, arginfo_void, ZEND_ACC_PUBLIC)
    /* ... */
    PHP_FE_END
};

PHP_MINIT_FUNCTION(extensionname)
{
    zend_class_entry tmp_ce;
    INIT_CLASS_ENTRY(tmp_ce, "DataClass", data_class_functions);
    data_class_ce = zend_register_internal_class(&tmp_ce TSRMLS_CC);

    /* DataClass implements Countable, ArrayAccess, IteratorAggregate */
    zend_class_implements(
        data_class_ce TSRMLS_CC, 3, spl_ce_Countable, zend_ce_arrayaccess, zend_ce_aggregate
    );
    // the number is the number of following argument, which are the interfaces class_entry

    return SUCCESS;
}

// register interface
// all method are declared abstract

#include "zend_interfaces.h"

zend_class_entry *reversible_iterator_ce;

const zend_function_entry reversible_iterator_functions[] = {
    PHP_ABSTRACT_ME(ReversibleIterator, prev, arginfo_void)
    PHP_FE_END
};

PHP_MINIT_FUNCTION(test)
{
    zend_class_entry tmp_ce;
    INIT_CLASS_ENTRY(tmp_ce, "ReversibleIterator", reversible_iterator_functions);
    reversible_iterator_ce = zend_register_internal_interface(&tmp_ce TSRMLS_CC);

    /* ReversibleIterator extends Iterator. For interface inheritance the zend_class_implements()
     * function is used. */
    zend_class_implements(reversible_iterator_ce TSRMLS_CC, 1, zend_ce_iterator);

    return SUCCESS;
}

