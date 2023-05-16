# Combined assignment 5

> This assignment is a combined tutorial/ sign-off. It consists of two parts.
> 
> You can do both parts either individually or in pairs.

You’ve been using the output file stream class `std::ofstream` for a while now. In this assignment you’ll implement your own replacement for it, learning how it works under the hood in the process and practicing the [**RAII idiom**](https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization).

## Part 1, demystifying the file output stream

The file output stream class [`std::ofstream`](https://en.cppreference.com/w/cpp/io/basic_ofstream) is used to write to files. Like this:

```
#include <fstream>

std::ofstream out{"output.txt"};
if (out) {
    out << "Hello, world!\n";
}
```

The same can be achieved using the C file i/o functions:

```
#include <cstdio>

FILE* out = std::fopen("output.txt", "w");
if (out != nullptr && std::ferror(out) == 0) {
    std::fprintf(out, "Hello, world!\n");
    std::fclose(out);
}
```

What distinguishes the `std::ofstream` class from the C file i/o functions is that it provides a more convenient interface to the user. Notably, writing to a file is using the _stream output operator_ `<<` instead of the C function `std::fprintf`. Moreover, the `std::ofstream` class takes care of closing the file automatically using the **RAII idiom**.

The `std::ofstream` class can be implemented as a _wrapper_ around the C file i/o functions [`std::fopen`](https://en.cppreference.com/w/cpp/io/c/fopen), [`std::fclose`](https://en.cppreference.com/w/cpp/io/c/fclose), [`std::fprintf`](https://en.cppreference.com/w/cpp/io/c/fprintf) etc. Here, you’ll do just that.

### How does `<<` work?

In programming, [_operators_](https://leren.saxion.nl/bbcswebdav/pid-3632757-dt-content-rid-145668573_2/courses/LED_TI_CFOUND_2223_3/assignments_2023/x) are functions that are called using special syntax. For example, the _binary operator_ `+` is a function that takes two operands and returns their sum. However, to use it, you cannot just write `int sum = +(1, 2)`. Instead, you must use the infix notation, where the operator is placed between the operands:

```
int sum = 1 + 2;
```

The stream output operator `<<` is also a _binary operator_. It takes two operands, a _stream_ and a _value_. In the example below the stream is `std::ofstream` variable `out` and the value is the string literal `"Hello, world!\n"`:

```
std::ofstream out{"output.txt"};
out << "Hello, world!\n";
```

You’d be surprised to learn that the `<<` operator is just a normal function that can be implemented by a user. It has, however, a special, pre-defined name, `operator<<` (so not just `<<`). If implemented as a member function of `NamedFile`, it could look like this:

```
class NamedFile {
public:
    // implementation of the operator<< for std::string 
    void operator<<(const std::string& value);
};
```

Because it’s just a function, you’d be able to call it using the usual function call syntax:

```
NamedFile out{"output.txt"};
out.operator<<("Hello, world!\n");
```

Or use the _infix notation_, that you’ve seen already:

```
NamedFile out{"output.txt"};

out << "Hello, world!\n";
```

The `<<` operator can be overloaded for any type that you want to write to a stream. For example, you could overload it for `std::string`, `const char *`, `char`, `int`, `double`, `bool` etc.

There is one more detail that you’ll need to know about the `<<` operator. When used with a _stream_ it can be chained to write multiple values to it. For example:

```
NamedFile out{"output.txt"};
out << "Hello, " << 42 << " worlds!" << '\n';
```

This is possible because the `<<` operator returns a reference to the stream it was called on:

```
class NamedFile {
public:
    // operator<< for std::string
    NamedFile& operator<<(const std::string& value);
    // operator<< for int
    NamedFile& operator<<(int value);
    // operator<< for char
    NamedFile& operator<<(char value);
};

NamedFile& NamedFile::operator<<(const std::string& value) {
    // write the string to the file
    // return reference to the current NamedFile object
    return *this;
}

NamedFile& NamedFile::operator<<(int value) {
    // write the int to the file
    // return reference to the current NamedFile object
    return *this;
}

NamedFile& NamedFile::operator<<(char value) {
    // write the char to the file
    // return reference to the current NamedFile object
    return *this;
}
```

Consequently, the following code is equivalent to the one above:

```
NamedFile out{"output.txt"};
out.operator<<("Hello, ").operator<<(42).operator<<(" worlds!").operator<<('\n');
```

Because each call to `operator<<` returns a reference to the `NamedFile` object `out`, you could decompose this chain call into multiple lines, but it’s an necessary burden:

```
NamedFile out{"output.txt"};
auto& result1 = out.operator<<("Hello, ");
auto& result2 = result1.operator<<(42);
auto& result3 = result2.operator<<(" worlds!");
auto& result4 = result3.operator<<('\n');
```

### Task, the `NamedFile` class

Implement a class `NamedFile` that will works like `std::ofstream`. This class should use the C file i/o functions `std::fopen`, `std::fclose`, `std::fprintf` and possibly `std::fputs` for managing a file and writing to it.

The `NamedFile` class has the synopsis:

```
class NamedFile {
public:
    // constructor, opens the file with the given name
    NamedFile(const std::string& file_name):    
    // destructor
    ~NamedFile();

    // opens the file with the given name, closes the previous file if it was open
    void open(const std::string& file_name);
    // closes the file
    void close();
    
    // writes the given value to the file if it is open
    // all those functions use std::fprintf or std::fputs
    NamedFile& operator<<(const std::string& value);
    NamedFile& operator<<(const char *value);
    NamedFile& operator<<(char value);
    NamedFile& operator<<(int number);
    NamedFile& operator<<(double number);
    // writes "true" or "false" to the file
    NamedFile& operator<<(bool value);
};
```

Next to it, `NamedFile` has the following accessors:

-   `is_open` that returns `true` if the file is open.
-   `error` that returns `true` if an error occurred (see [`std::ferror`](https://en.cppreference.com/w/cpp/io/c/ferror)).
-   `eof` that returns `true` if the end of file has been reached (see [`std::feof`](https://en.cppreference.com/w/cpp/io/c/feof)).
-   `lines` that returns the number of lines written so far. You’ll have to check how many newline characters `'\n'` each written string contains for this to work. Don’t forget about counting the last line if it doesn’t end with a newline character.
-   `size` that returns the number of characters written so far. (Tip: `std::fprintf` returns the count of chars it has written.)
-   `name` that returns the name of the file, that was passed to the constructor or to the `open` function.

You’ll need at least two `private` data members in your class, one for the file name and one for the file resource (`std::FILE *`).

**MUSTS**:

The `NamedFile` class must:

-   Be implemented in the `log` namespace.
-   Implement all the functions in the synopsis and all the accessors listed above.
-   Allow chaining of writes using the `<<` operator.
-   Use the C file i/o functions `std::fopen`, `std::fclose`, `std::fprintf` and possibly `std::fputs` for managing a file and writing to it.
-   Use the C [`FILE`](https://en.cppreference.com/w/cpp/io/c/FILE) type as a `private` data member for managing the file resource.
-   Use the [_RAII_] idiom to automatically close the file when a `NamedFile` object reaches the end of its lifetime.

**COULDS**:

The `NamedFile` class could possibly allow for the following:

```
log::NamedFile out{"output.txt"};

// appending newline 
out << "Hello, " << 42 << " worlds!" << log::endl;

// flushing with flush
out << log::flush;
```

To do this you’ll need to add two empty `struct`s to the `log` namespace: `endl_t` and`flush_t`. You can then add two `inline` variables of type `endl_t` and `flush_t` to the `log` namespace. These variables can then be used to call the `operator<<` function with the `endl_t` and `flush_t` types. The `operator<<` function should then call `std::fputc` with the newline character `'\n'` and `std::fflush` respectively.

An example of how the operator `<<` can be implemented for the `const char*` type:

```
NamedFile& NamedFile::write(const char* str){
    std::fputs(str, file_);
    // returns reference to the current instance
    return *this;
}
```

When fully implemented, the following code:

````
NamedFile fw{"test.txt"};
fw << "Alice has " << 3 << " rats.\n";

std::cout << "Written " << fw.size() << " chars and " 
          << fw.lines() << " lines.\n";

fw << "True of false: does Alice like rats?\n"
    << "Answer: " << true << "\n";

std::cout << "Written " << fw.size() << " chars and " 
          << fw.lines() << " lines.\n";


will produce the output:

```text
Written 18 chars and 1 lines.
Written 68 chars and 3 lines.
````

## Part 2, testing `NamedFile`

As a test for your `NamedFile` implementation use it in the `Logger` class from this week’s reader as a replacement for `std::ofstream`.

```
#include "file_writer.h"

class Logger:
public:
    // gets the singleton instance of Logger
    Logger& get();
    void log(const std::string& msg);
private:
    // private, default constructor
    Logger();

    // logging file
    NamedFile out_;
};  
```

Since `std::put_time` won’t work with `NamedFile` you’ll have to use [`std::strftime`](https://en.cppreference.com/w/cpp/chrono/c/strftime) to get the current time. A full example of how to do it is given in the documentation page. Once you have the time in a `char` buffer you can pass this buffer to the `NamedFile::operator<<` overload that takes `const char*` argument.

## Signing-off your work

We won’t sign-off your work unless:

-   All the **MUST** conditions are satisfied.
-   `NamedFile` has been fully replaced `std::ofstream` in the `Logger` class.
-   Warnings are enabled in the CMakeLists.txt file.
-   Your code compiles and runs without errors or warnings.
-   You demonstrate and test the working of your code in the `main` file.
-   You properly split your design into header and source files.
-   You are able to change your implementation on the spot, for instance to add support for writing a new type to the `NamedFile` class output stream operators.