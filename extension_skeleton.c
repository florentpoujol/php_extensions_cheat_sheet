


// declare function body
PHP_FUNCTION(extname_function_name)
{
    // each function (and method) have an actual argument which is zval *return_value
    // it needs to be set to something for the function to "return" a value


    // declare expected params
    // see below for where the parameters are defined
    // (the ZEND_BEGIN_ARG_INFO_EX() part)
    Hashtable *array;
    size_t size;

    // get the expected params
    // see http://www.phpinternalsbook.com/php7/extensions_design/php_functions.html#parsing-parameters-zend-parse-parameters
    // for full explanation
    // the string as second argument define the expected types
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "hl", &left, &size) == FAILURE) {
        return;
    }

    // if the function does not except any parameter, you can do like in the method below

    return_value = *merge_sort(left, size);

    //array_init_size(return_value, zend_hash_num_elements(left));

}



// method
PHP_METHOD(ClassName, methodName) /* {{{ */
{
    // when the function does no expect any params
    // use  zend_parse_parameters_none() instead of zend_parse_parameters())
    if (zend_parse_parameters_none() == FAILURE) {
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


    // this sets the return_value to the string
    RETURN_STRING("Hello World\n", 1);
}
/* }}} */


// ======================================================================


// declare function arguments
// this is mostly cosmetic, but used for Reflection and the engine
// this function expect one array then one optional int
// see below for the method for all the macro to use with other arg type
ZEND_BEGIN_ARG_INFO_EX(arginfo_function_name, 0, 0, 1) 
    // last number is the number of required args

    ZEND_ARG_ARRAY_INFO(0, "theArray", 0)
    ZEND_ARG_TYPE_INFO(0, "theSize", "int", 1)
    // pass_by_ref and allow_nulll are 0 or 1
    // name and type_hit are string
ZEND_END_ARG_INFO()

// same things for methods
// if you want to return a value, the beggining macro differs
// ie this method returns a string and expect no  required argument (the content is just to show other argument macros)
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_methodName, 0, "s", ClassName, 0)
//ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_methodName, return_ref, required_num_args, type, class_name, allow_null)
    // can we use this macro with function instead of method ? I don't know

    ZEND_ARG_INFO(pass_by_ref, name) // doesn't define a particular type

    ZEND_ARG_TYPE_INFO(pass_by_ref, name, type_hint, allow_null)
    ZEND_ARG_OBJ_INFO(pass_by_ref, name, classname, allow_null)
    ZEND_ARG_ARRAY_INFO(pass_by_ref, name, allow_null)
    ZEND_ARG_CALLABLE_INFO(pass_by_ref, name, allow_null)

    ZEND_ARG_VARIADIC_INFO(pass_by_ref, name)
    // pass_by_ref and allow_nulll are 0 or 1
    // name and type_hit are string
ZEND_END_ARG_INFO()

// arguments don't need to be defined per-function or per-method
// these macros define signature so i can be reused for multiple func/methods
// ie:
ZEND_BEGIN_ARG_INFO_EX(arginfo_void, 0, 0, 0)
ZEND_END_ARG_INFO()
// the name of each signature is used below when registering 
// functions to the extname_functions array
// methods inside the MINIT function


// ======================================================================


// register functions
static const zend_function_entry extname_functions[] =
{
    // register here the functions names and their arguments
    PHP_FE(extname_function_name, arginfo_function_name)
    PHP_FE_END // leave that at the end
}


// register method for a particuler class
const zend_function_entry className_functions[] = {
     // non abstract method, in that case a simple public one
    PHP_ME(ClassName, methodName, arginfo_methodName, ZEND_ACC_PUBLIC)

    // for abstract method
    PHP_ABSTRACT_ME(ClassName, abstractMethod, arginfo_abc) 

    PHP_FE_END
};
// last param of PHP ME is a flag (can be combined):
// ZEND_ACC_PUBLIC  ZEND_ACC_PRIVATE  ZEND_ACC_PROTECTED
// ZEND_ACC_STATIC  ZEND_ACC_FINAL    ZEND_ACC_ABSTRACT
// ie   ZEND_ACC_PUBLIC | ZEND_ACC_FINAL

// interfaces only have abstract classes
const zend_function_entry myInterface_functions[] = {
    PHP_ABSTRACT_ME(MyInterface, interfaceMethod, arginfo_interfaceMethod)
    PHP_FE_END
};


// ======================================================================


// declare module
zend_module_entry extnamemodule_entry = {
    STANDARD_MODULE_HEADER,
    "extname", // anem of your extension
    extname_functions, // register here the list of the functions created just above
    NULL, // MINIT
    NULL, // MSHUTDOWN
    NULL, // RINIT
    NULL, // RSHUTDOWN
    NULL,
    "0.0.1",
    STANDARD_MODULE_PROPERTIES
};



// define a constant
#define EXTNAME_CONSTANT_NAME 1
// also use


// global variable that hodls a class
zend_class_entry *className_ce; // respresents the scope
zend_class_entry *customException_ce; // respresents the scope
zend_class_entry *runtimeChildren_ce; // respresents the scope
// interfaces also use zend_class_entry
zend_class_entry *myInterface_ce;




// then register it in the MINIT hook
PHP_MINIT_FUNCTION(extname)
{

    // register constants
    REGISTER_LONG_CONSTANT("EXTNAME_CONSTANT_NAME", EXTNAME_CONSTANT_NAME, CONST_CS|CONST_PERSISTENT);
    // the second argument is the value, but here it is an actual contant defined with #define


    // register classes
    zend_class_entry tmp_ce;

    INIT_CLASS_ENTRY(tmp_ce, "ClassName", className_functions);
    // the last argument is the list of functions, it can be NULL
    className_ce = zend_register_internal_class(&tmp_ce TSRMLS_CC); // without inheritance

    // ClassName implements Countable
    // the relevant .h must also have been included
    zend_class_implements(
        className_ce TSRMLS_CC, 1, spl_ce_Countable
    );
    // the number is the number of following argument, which are the interfaces class_entry


    INIT_CLASS_ENTRY(tmp_ce, "CustomException", NULL); // empty Class
    customException_ce = zend_register_internal_class_ex(
        // inherits from Exception
        &tmp_ce, zend_exception_get_default(TSRMLS_C), NULL TSRMLS_CC 
    );


    INIT_CLASS_ENTRY(tmp_ce, "RuntimeChildren", NULL); //
    runtimeChildren_ce = zend_register_internal_class_ex(
        // inherist from the RuntimeException
        &tmp_ce, spl_ce_RuntimeException, NULL TSRMLS_CC
        // the .h where the class in defined must have been included at the top of the file
    );


    // declare properties and default values
    // here for the runtimeChildren_ce class
    // zend_declare_property(class_entry, pro_name, sizeof(propName)-1, value, visibility_flag TSRMLS_CC);
    zend_declare_property_null(runtimeChildren_ce, "foo", sizeof("foo") - 1, ZEND_ACC_PUBLIC TSRMLS_CC);
    // other functions zend_declare_property_[type]();
    zend_declare_property_string(runtimeChildren_ce, "bar", sizeof("bar") - 1, "the default value", ZEND_ACC_PROTECTED TSRMLS_CC);
    // see above (in the ClassName::method) for how to get/set property within a method

    // same idea for class constants
    zend_declare_class_constant_[type](class_entry, const_name, sizeof(const_name) - 1, value TSRMLS_CC);

    // register interface
    INIT_CLASS_ENTRY(tmp_ce, "MyInterface", myInterface_functions);
    myInterface_ce = zend_register_internal_interface(&tmp_ce TSRMLS_CC);

    
    return SUCCESS;
}



// ======================================================================
// working with zval





// create a ZVAL
zval *zv_ptr;

ALLOC_ZVAL(zv_ptr); // alloc
INIT_PZVAL(zv_ptr); // then init (the zval value is not initialized)
// is the same as
MAKE_STD_ZVAL(zv_ptr);

// or

ALLOC_ZVAL(zv_ptr); // alloc
INIT_ZVAL(*zv_ptr); // then init. Note that you pass the zval value, not it's pointer.
// the zval's type is now IS_NULL (its internal value zval.v is NULL)
// this is the same as
ALLOC_INIT_ZVAL(zv_ptr);

Z_REFCOUNT_P(zv_ptr);      /* Get refcount */
Z_ADDREF_P(zv_ptr);        /* Increment refcount */
Z_DELREF_P(zv_ptr);        /* Decrement refcount */
Z_SET_REFCOUNT_P(zv_ptr, 1); /* Set refcount to some particular value (here 1) */
// same macros are available without _P suffix or with _PP suffix


// decrement ref count:
Z_DELREF_P(zv_ptr);
if (Z_REFCOUNT_P(zv_ptr) == 0) { // then check if needs to be freed
    zval_dtor(zv_ptr);
    efree(zv_ptr);
}
// is the same as
zval_ptr_dtor(&zv_ptr);
// also
zval_dtor(zv_ptr);


// copy value
zval *zv_src;
MAKE_STD_ZVAL(zv_src);
ZVAL_STRING(zv_src, "test", 1);

zval *zv_dest;
ALLOC_ZVAL(zv_dest);

INIT_PZVAL_COPY(zv_dest, zv_src);
// same as 
INIT_PZVAL(zv_dest); // as before, init refcount and is_ref
ZVAL_COPY_VALUE(zv_dest, zv_src); // copie the zval

// then
zval_copy_ctor(zv_dest); // actually copies the zval's value

// the couple
INIT_PZVAL_COPY(zv_dest, zv_src);
zval_copy_ctor(zv_dest);
// is the same as
MAKE_COPY_ZVAL(&zv_src, zv_dest);

// so shortest way to copy a zval
zval *zv_src;
MAKE_STD_ZVAL(zv_src);
ZVAL_STRING(zv_src, "test", 1);
zval *zv_dest;
ALLOC_ZVAL(zv_dest);
MAKE_COPY_ZVAL(&zv_src, zv_dest);

// another way
ZVAL_ZVAL(zv_dest, zv_src, copy, dtor); // copy and dtor are int

ZVAL_ZVAL(zv_dest, zv_src, 0, 1);
/* equivalent to: */
ZVAL_COPY_VALUE(zv_dest, zv_src);
ZVAL_NULL(zv_src);
zval_ptr_dtor(&zv_src);


// is_ref flag manip
Z_ISREF_P(zv_ptr); // get is_ref value

Z_SET_ISREF_P(zv_ptr); 
// same as
Z_SET_ISREF_TO_P(zv_ptr, 1);

Z_UNSET_ISREF_P(zv_ptr);
// same as
Z_SET_ISREF_TO_P(zv_ptr, 0);
// variants without suffix _P or with _PP


// CASTING
void convert_to_[type](zv_ptr);
void convert_to_[type]_ex(zv_ptr_ptr); // separate the value first, if it has refcount>1

// check if string is numeric and extract value as long or double
long lval;
double dval;

switch (is_numeric_string(Z_STRVAL_P(zv_ptr), Z_STRLEN_P(zv_ptr), &lval, &dval, allow_error)) {
    case IS_LONG:
        /* String is an integer those value was put into `lval` */
        break;
    case IS_DOUBLE:
        /* String is a double those value was put into `dval` */
        break;
    default:
        /* String is not numeric */
}
// alllow_error param :
// 0 "123abc" throws error
// 1 "123abc" > 123
// -1 "123abc" > 123 + PHP Notice


// =======================================================================
// SYMTABLE (array)
// handle numerical string keys as int
http://www.phpinternalsbook.com/hashtables/array_api.html

int zend_symtable_find(                 HashTable *ht, const char *arKey, uint nKeyLength, void **pData)
int zend_symtable_exists(               HashTable *ht, const char *arKey, uint nKeyLength);
int zend_symtable_del(                  HashTable *ht, const char *arKey, uint nKeyLength);
int zend_symtable_update(               HashTable *ht, const char *arKey, uint nKeyLength, void *pData, uint nDataSize, void **pDest);
int zend_symtable_update_current_key_ex(HashTable *ht, const char *arKey, uint nKeyLength, int mode, HashPosition *pos);

ZEND_INIT_SYMTABLE(ht);
ZEND_INIT_SYMTABLE_EX(ht, size, persistent);


// =======================================================================
// ARRAYS
http://www.phpinternalsbook.com/hashtables/array_api.html

// create a zval with value 42 (type long)
zval *zv_ptr;
MAKE_STD_ZVAL(zv_ptr);
ZVAL_LONG(zv_ptr, 42);

// set it to the 0 key of the array
add_index_zval(some_array, 0, zv_ptr);
// but also to the "num" key
add_assoc_zval(some_array, "num", zv_ptr);
Z_ADDREF_P(zv_ptr); // so the refcount must be incremented (because the zval is used in two places)

// get value from key
zval *zv_ptr_array = /* get array from somewhere */;

// Fetch array index 42 into zv_dest (how this works is not relevant here)
zval **zv_dest;
zend_hash_index_find(Z_ARRVAL_P(zv_ptr_array), 42, (void **) &zv_dest)

// do not convert zv_dest right away, unles you want it
// better copy it then convert the copy
zval zv_tmp;

ZVAL_COPY_VALUE(&zv_tmp, *zv_dest);
zval_copy_ctor(&zv_tmp);

convert_to_long(&zv_tmp);

// get array length
zend_hash_num_elements(Z_ARRVAL_P(zv_ptr))


// create array inside return_value
array_init(return_value);
array_init_size(return_value, size);

/* Insert at next index */
int add_next_index_[type](zval *arg, [additional args]);
/* Insert at specific index */
int add_index_[type](zval *arg, ulong idx, [additional args]);
/* Insert at specific key */
int add_assoc_[type](zval *arg, const char *key, [additional args]);
/* Insert at specific key of length key_len (for binary safety) */
int add_assoc_[type]_ex(zval *arg, const char *key, uint key_len, [additional args]);

/*
Type    Additional arguments
null    none
bool    int b
long    long n
double  double d
string  const char *str, int duplicate
stringl const char *str, uint length, int duplicate
resource    int r
zval    zval *value


*/
// ==============================================
// HASHTABLE
http://www.phpinternalsbook.com/hashtables/hashtable_api.html

HashTable *myht;

/* Same as myht = emalloc(sizeof(HashTable)); */
ALLOC_HASHTABLE(myht);

zend_hash_init(myht, size, NULL, dtor, persistant); 
// size is rounded up toward the next power of 2
// dtor is the destructor function: typedef void (*dtor_func_t)(void *pDest);
// this is often ZVAL_PTR_DTOR when the stored values are zval
// persistant should be int 0

// destroy:
zend_hash_destroy(myth);
FREE_HASHTABLE(myth);

// empty the hastable but not destroy
zend_hash_clean(myth);

// ----------------------------------
// NUM KEYS

// insert numerical key
zval *zv_ptr;
MAKE_STD_ZVAL(zv_ptr);
ZVAL_STRING(zv_ptr, "foo", 1);

zend_hash_index_update(myht, 42, &zv_ptr, sizeof(zval*), NULL);

// the last param can be used to retrieve the value stored
zval **zv_dest;
zend_hash_index_update(myht, 42, &zv_ptr, sizeof(zval*), (void **) &zv_dest);
// **zv_dest is a memory adress, different than **zv_ptr
// but both holds the same value: the memory adress of zv_ptr

// append a value in the hastable
if (zend_hash_next_index_insert(myht, &zv, sizeof(zval *), NULL) == SUCCESS) {
    // needs to check the return value for failure
    Z_ADDREF_P(zv);
}


// gets the next free integer key and number of elements
zend_hash_next_free_element(hash);
zend_hash_num_elements(hash);

zend_hash_index_find(myht, 42, (void **) &zv_dest); // returns success or failure
zend_hash_index_exists(hash, index); // returns 1 or 0
zend_hash_index_del(hash, index); // returns success or failure

// -----------------------------------
// STRING keys






