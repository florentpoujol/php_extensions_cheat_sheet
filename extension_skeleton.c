
// define constant
#define EXTENSIONNAME_THE_CONSTANT 1

// then register it in the MINIT hook
PHP_MINIT_FUNCTION(extensionname)
{
    REGISTER_LONG_CONSTANT("EXTENSIONNAME_THE_CONSTANT", EXTENSIONNAME_THE_CONSTANT, CONST_CS|CONST_PERSISTENT);

    return SUCCESS;
}



// declare function body
PHP_FUNCTION(ext_merge_sort)
{
    // params
    Hashtable *left;
    size_t size;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "hl", &left, &size) == FAILURE) {
        return;
    }
    // end params

    return_value = *merge_sort(left, size);

    //array_init_size(return_value, zend_hash_num_elements(left));

}

// declare function arguments
ZEND_BEGIN_ARG_INFO_EX(arginfo_ext_merge_sort, 0, 0, 1)
    ZEND_ARG_ARRAY_INFO(0, "left", 0)
    ZEND_ARG_TYPE_INFO(0, "size", "int", 0)
ZEND_END_ARG_INFO()


// register functions
static const zend_function_entry extensionnamefunctions[] =
{
    PHP_FE(nmerge_sort, arginfo_nmerge_sort)
    PHP_FE_END
}


// declare module
zend_module_entry extensionnamemodule_entry = {
    STANDARD_MODULE_HEADER,
    "extsort",
    extensionnamefunctions,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "0.0.1",
    STANDARD_MODULE_PROPERTIES
};



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


// ===============================================
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


// ===============================================
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


// ------------------
// Iteration

zval **data;

for (zend_hash_internal_pointer_reset(myht);
     zend_hash_get_current_data(myht, (void **) &data) == SUCCESS;
     zend_hash_move_forward(myht)
) {
    /* Do something with data */
}