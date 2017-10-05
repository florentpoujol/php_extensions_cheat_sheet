
// All examples here are taken from the official documentation:
// http://www.php-cpp.com/documentation


// output text:
void sameAsEcho()
{
    Php::out << "example output" << std::endl;

    // generate output without a newline, and ensure that it is flushed
    Php::out << "example output" << std::flush;

    // or call the flush() method
    Php::out << "example output";
    Php::out.flush();

    // just like all PHP functions, you can call the echo() function 
    // from C++ code as well
    Php::echo("Example output\n");
}


// notice, warning, deprecated, fatal error
void errorOutput()
{
    // generate a PHP notice
    Php::notice << "this is a notice" << std::flush;

    // generate a PHP warning
    Php::warning << "this is a warning" << std::flush;

    // inform the user that a call to a deprecated function was made
    Php::deprecated << "this method is deprecated" << std::flush;

    // generate a fatal error
    Php::error << "fatal error" << std::flush;

    // this code will no longer be called
    Php::out << "regular output" << std::endl;
}  


// signatures for functions exposed to the userland
void example1();
void example2(Php::Parameters &params);
Php::Value example3();
Php::Value example4(Php::Parameters &params);


// working with params
void funcParams(Php::Parameters &params)
{
    // loop through params 
    for (auto &param : parameters) {}

    // getting a value from the params
    Php::Value theFirstParam = params[0];;

    // checking how many params there is
    if (params.size == 0) { }
    if (params.empty()) { }
}



// calling userland or other native (core or extensions) functions:
// http://www.php-cpp.com/documentation/calling-functions-and-methods
void example(Php::Parameters &params)
{
    // first parameter is an array
    Php::Value array = params[0];

    std::vector<std::string> keys = Php::array_keys(array);
    // other native functions are available this way


    // call a function from user space
    Php::Value data = Php::call("some_function", "some_parameter");

    // create an object of a particular class
    Php::Object time("DateTime", "now");
    // Same as   $time = new DateTime("now");

    // call a method on the datetime object
    Php::out << time.call("format", "Y-m-d H:i:s") << std::endl;


    // second parameter is a callback function
    Php::Value callback = params[1];

    // call the callback function
    callback("some", "parameter");


    // create a callable as array with object and method name
    Php::Array time_format({time, "format"});
    // same as    $time_format = [$time, "format"];

    // call the method that is stored in the array
    time_format("Y-m-d H:i:s")
    // same as    call_user_func($time_format, "Y-m-d H:i:s");
}


// working with annon function (closure)
// http://www.php-cpp.com/documentation/lambda-functions
void closure(Php::Parameters &params)
{
    // get a clusure or any Callable from the params
    Php::Value func = params[0];

    // the Php::Value class has implemented the operator (), which allows
    // us to use the object just as if it is a real function
    Php::Value result = func(3, 4);


    // create a closure
    Php::Function myClosureName(
        [](Php::Parameters &params) -> Php::Value 
        {
            // closure body
        }
    );

    // the signature of the closure can be one of these two:
    // Php::Value function();
    // Php::Value function(Php::Parameters &params);

    // can be assigned to other var, called or passed to other functions
    

    Php::Value stuff = myClosureName();

    Php::Value otherClosureName = myClosureName;
    otherClosureName();

    Php::Value array;
    Php::array_map(array, myClosureName);
}


// classes must just extend from Php::Base (even classes that, in the userland, "extends" another)
// http://www.php-cpp.com/documentation/classes-and-objects
class Counter : public Php::Base
{
private:
    int _value = 0;

public:
    // C++ constructor and destructor
    Counter() = default; // called before the PHP constructor
    virtual ~Counter() = default;

    // copy constructor
    Counter(const Counter &counter) = default;
    // to prevent copy of the PH object, either remove the c++ copy constructor
    // or register a private __clone() method with    classDesc.method("__clone", Php::Private);  (in the get_module method below)

    // PHP construc tor (called AFTER the c++ cntstructor)
    void __construct(Php::Parameters &params)
    {
        // do something
    }


    Php::Value increment() { return ++_value; }
    Php::Value decrement() { return --_value; }
    Php::Value value() const { return _value; }
    
    static void staticMethod(Php::Parameters &params) { }

    // all magic methods are supported
    // http://www.php-cpp.com/documentation/magic-methods
    // these method DO NOT need to be regsitered in get_module() below (except for __construct)
    
    bool __isset(const Php::Value &name) 
    {
        // true for name and email address
        if (name == "name" || name == "email") return true;

        // fallback to default
        return Php::Base::__isset(name);
    }

    void __set(const Php::Value &name, const Php::Value &value) 
    {
        // bla bla
        else
        {
            // call default
            Php::Base::__set(name, value);
        }
    }

    Php::Value __toString()
    {
        return "abcd";
    }

    // you can also implement special magic methods :
    // http://www.php-cpp.com/documentation/special-features
    // __toInteger()  __toFloat()  __toBool()  __compare()


    // getter and setter for properties
    Php::Value getValue() const
    {
        return _value;
    }

    void setValue(const Php::Value &value)
    {
        // overwrite property
        _value = value;

        // sanity check: the value should never exceed 100
        if (_value > 100) _value = 100;
    }

    // no setter (read-only prop)
    Php::Value getDouble() const
    {
        return _value * 2;
    }
};


// methods to be exposed to userland supports these signatures
// same as for function (+ const version)
void        YourClass::example1();
void        YourClass::example2(Php::Parameters &params);
Php::Value  YourClass::example3();
Php::Value  YourClass::example4(Php::Parameters &params);
void        YourClass::example5() const;
void        YourClass::example6(Php::Parameters &params) const;
Php::Value  YourClass::example7() const;
Php::Value  YourClass::example8(Php::Parameters &params) const;



void exceptions() 
{
    // throw
    Php::Exception("Grr! bad thing!");

    // catch
    try
    {
        //
    }
    catch (Php::Exception &exception)
    {
        //
    }
}


void ini()
{
    // get configuration ini param
    int output_buffering =        Php::ini_get("output_buffering");
    std::string someStuff = Php::ini_get("my_extension.var2");

    // see below for registering extension's ini settings
}


// Working with variables / zval / Php::Value
// http://www.php-cpp.com/documentation/variables

// acces superglobals :
// Php::GET, Php::POST, Php::COOKIE, Php::FILES, Php::SERVER, Php::REQUEST and Php::ENV. 
// string avalue = Php::GET["aKey"];


extern "C" {
    /**
     *  Startup function that is called by the Zend engine 
     *  to retrieve all information about the extension
     *  @return void*
     */
    PHPCPP_EXPORT void *get_module() {
        // create static instance of the extension object
        static Php::Extension myExtension("my_extension", "1.0");

        // register function
        myExtension.add<aCppFunction>("aFunction"); 
        // the aCppFunction will be called "aFunction" in PHP Userland
        
        // "function" can also actually be methods
        myExtension.add<&AClass::aMethod>("aFunction");


        // parameters can be specified
        // http://www.php-cpp.com/documentation/parameters
        myExtension.add<aCppFunction>("aFunction", {
            Php::ByVal("a", Php::Type::Numeric), // a required, numeric param named "a"
            Php::ByVal("b", "ExampleClass", true, false) // and optional, nullable param nammed "b" of type ExampleClass
            Php::ByRef("c", Php::Type::Array, false) // and optional array nammed "c" passed by reference
        });
        /*
        signature of ByVal when the type is inPhp::Type
        Php::ByVal(const char *name, Php::Type type, bool required = true);

        Php::Type::Null
        Php::Type::Numeric
        Php::Type::Float
        Php::Type::Bool
        Php::Type::Array
        Php::Type::Object
        Php::Type::String
        Php::Type::Resource
        Php::Type::Constant
        Php::Type::ConstantArray
        Php::Type::Callable

        signature of ByVal when the type is class
        ByVal(const char *name, const char *classname, bool nullable = false, bool required = true);

        ByRef() has the two same signatures
        */



        // description of interfaces
        Php::Interface interfaceDesc("MyInterface");

        // define an interface method
        interfaceDesc.method("myInterfaceMethod", { 
            Php::ByVal("value", Php::Type::String, true) 
        });

        // add the interface to the extension
        myExtension.add(std::move(interfaceDesc));



        // description of the class so that PHP knows which methods are accessible
        // http://www.php-cpp.com/documentation/classes-and-objects
        Php::Class<Counter> classDesc("Counter");

        // for final or abstract classes:
        Php::Class<Counter> classDesc("Counter", Php::Final);
        Php::Class<Counter> classDesc("Counter", Php::Abstract);


        // if the class implement interfaces
        classDesc.implements(interfaceDesc);

        // if the class extends from another
        classDesc.extends(parentClassDesc);


        // register methods
        classDesc.method<&Counter::__construct>("__construct");
        // the constructor can be made private or protected, to prevent instanciation of that class
        // classDesc.method<&Counter::__construct>("__construct", Php::Private);
        classDesc.method<&Counter::value>("value");
        // note the & character that does not exists for functions

        // specifiy parameters as for functions:
        classDesc.method<&Counter::increment>("increment", { 
            Php::ByVal("change", Php::Type::Numeric, false)  // optional numeric "change" param
        });

        // specify access modifier between method name and params
        classDesc.method<&Counter::decrement>("decrement", Php::Protected);
        // or
        classDesc.method<&Counter::decrement>("decrement", Php::Protected | Php::Final, { blabla });

        // static methodd are registered as regular method
        classDesc.method<&PublicClass::staticMethod>("static1");


        // register an abstract method
        // no need to pass in a pointer to the method or to pass in a flag: 
        // the method automatically becomes abstract if no address of a C++
        // method is supplied
        classDesc.method("myAbstractMethod", { /* params (optional as always) */ });


        // register public property
        // http://www.php-cpp.com/documentation/properties
        classDesc.property("property1", "default value", Php::Public);

        // register the "value" property, with the methods to get and set it
        example.property("value", &Counter::getValue, &Counter::setValue);

        // register a read-only "double" property, with a method to get it
        example.property("double", &Counter::getDouble);

        // static property
        classDesc.property("my_property", "initial value", Php::Public | Php::Static);


        // add the class descriptor to the extension
        // (or move it into the extension, which is faster)
        myExtension.add(std::move(classDesc));


        // constants
        // http://www.php-cpp.com/documentation/constants

        // class constant
        classDesc.constant("MY_CONSTANT", "some value");
        // same as 
        classDesc.property("MY_CONSTANT", "some value", Php::Const);
        classDesc.add(Php::Constant("MY_CONSTANT", "some value"));

        // module constants
        myExtension.add(Php::Constant("MY_CONSTANT_1", 1));
        myExtension.add(Php::Constant("MY_CONSTANT_2", 4.932843));
        myExtension.add(Php::Constant("MY_CONSTANT_3", "foobar"));

        // youc an check or define constants at runtime with define() and defined()
        Php::constant("ANOTHER_CONSTANT"); // get value
        Php::defined("DYNAMIC_CONSTANT"); // check if defined
        Php::define("DYNAMIC_CONSTANT", 12345); // define at runtime
        // note that these functions should be used outside the get_module() function



        // register ini settings
        myExtension.add(Php::Ini("my_extension.var1", "default-value"));
        myExtension.add(Php::Ini("my_extension.var2", 12345));



        // Extension lifetime hooks/callbacks
        extension.onRequest([]() {

        });
        /* available hooks
        void onStartup(const std::function<void()> &callback);    MODULE STARTUP (actually after the zend engine has loaded the extension)
        void onRequest(const std::function<void()> &callback);    REQUEST STARTUOP
        void onIdle(const std::function<void()> &callback);       REQUEST SHUTDOWN
        void onShutdown(const std::function<void()> &callback);   MODULE SHUTDOWN
        */


        // return the extension
        return myExtension;


        // use namespaces
        // http://www.php-cpp.com/documentation/namespaces
        // you can simply prefix classes and function names with the namespaces
        // or use Php::namespace (add func and classes to the namespace instead of the extension, then add the namespace to the extension)

        // create extension object
        static Php::Extension extension("my_extension", "1.0");

        // create a namespace
        Php::Namespace myNamespace("myNamespace");

        // add the myFunction function to the namespace
        myNamespace.add("myFunction", myFunction);

        // @todo add more functions and classes to the namespace

        // create a nested namespace
        Php::Namespace nestedNamespace("nestedNamespace");

        // @todo add functions and classes to the nested namespace

        // add the nested namespace to the first namespace
        myNamespace.add(std::move(nestedNamespace));

        // add the namespace to the extension
        extension.add(std::move(myNamespace));

        // return the extension
        return extension;
    }
}
