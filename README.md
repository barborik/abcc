# abcc
Adam Barborík's C Compiler

### <ins>ABOUT</ins>
Adam Barborík's C Compiler is a compiler for a dialect of C called ABC - Adam Barborík C.<br>
I always wanted to write my own compiler, so here it is.

### <ins>MAJOR DIFFERENCES</ins>

#### Data types
Data types are more standardized, size is a part of the name.

signed | unsigned | C equivalent
-------|----------|-------------
I0     | U0       | void
I8     | U8       | char
I16    | U16      | short
I32    | U32      | int
I64    | U64      | long

#### Expressions
Expressions make more sense overall.<br>
For example, this is a valid C expression:
```c
array[4]++
```
This implies that the [] operator returns an address on which the increment operation is performed. By this logic something like
```c
++var++
```
should be valid too, but it isnt.<br>
In ABC every operator returns with a value regardless of context. So for the same behavior in ABC you would have to do something like:
```
(array + i)++
```
where the ```i``` variable is index of the element multiplied by the size of the type.

### <ins>THIRD PARTY</ins>
#### Preprocessor
Current implementation uses GrieferAtWork's [Tiny PreProcessor](https://github.com/GrieferAtWork/tpp), although im planning on implementing my own.

### <ins>ERRORS</ins>
At the moment, source code is not checked in any way during syntactical analysis and treats the code as valid even when it isn't. Errors are not reported in any way. If the compilation process takes too long its safe to assume the code has an error somewhere.<br><br>
There are only 2 outcomes when compiling ABC code:
+ Success
+ Undefined behavior

### <ins>BUILDING</ins>
The only user side dependency of this project is a compiler. Using gcc (mingw) you will be able to successfully compile this repository on Windows and *NIX systems. All other dependencies are bundled with the project.

To build the project simply run ```make``` in the parent directory.

### <ins>USAGE</ins>
```
$ ./abcc [flags] <input file> <output file>
flags:

  -h, --help display this help message
  
  -m <mode>
      64      64-bit mode
      16      16-bit mode
      
  -cc <calling convention>
       sysv   SystemV (Linux, most Unix variants)
       ms6$   Microsoft x64 (Windows)
```

### <ins>DRAWBACKS</ins>
Most of these will be implemented.

+ no array literals
+ no structures
+ no typedefs
+ no floating point types

### <ins>ISSUES</ins>
These **WILL** be fixed, mark my words.
+ (ERRORS)
+ mode not implemented, only supporting 64bit
+ calling functions with more than 4 arguments on Windows and 6 on Linux causes undefined behavior
