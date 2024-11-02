# Some notes about writing better C++ according to the internet and GPT

C++ REPL courtesy of Claude AI:
Run with
`g++ -std=c++17 -o repl repl.cpp`


```
// floating point literals
1.0           - this is a double
1.0f          - this is a float
1.f           - this is a float
1             - this is an int
float a = 1   - this got implicitly converted from an int
```

```
int longNumber = 299'792'458 // you can break up larger numbers into easier
                             // to read sections with '
```

```
// Check the variable type with
#include <typeinfo>

std::cout << typeid(varInQuestion).name() << std::endl;
```


```
// Use constant expressions instead of defines
namespace config {
  constexpr size_t M = 2;
  constexpr float PI = 3.14f;
  constexpr int SAMPLE_RATE = 1'000'000;

  constexpr size_t fARRAY_SIZE(std::vector<std::complex<float>> vec) {
    return 2 * vec.capacity() * sizeof(float);
  } // Even inline functions to eval at compile time.
}

int main(){
printf("%d", config::)
}
```

```
// Best practices for declaring complex numbers

// DON'T DO THIS:
std::complex<float> z1 = 1.5f - 2.4f * 1j;

// DO:
std::complex<float> z2(1.5f, -2.4f);
// OR:
std::polar(std::sqrt(1.5f ^ 2 + 2.4f ^ 2), std::atan(-2.4f/1.5f));
```

```
// Vector size and memory operations
// Instead of using int, use size_t for .size() returns.
// And make your size computation explicit by using .size() * sizeof(std::complex<float>)
// To get the size in bytes.
```


A Note on Passing Arguments by Reference vs Value

  1) Pass by value when the args are small and you only need a local copy of that value.
  ```
    int writeIQToFile(size_t sizeToWrite);
  ```

  2) Pass by const reference when the parameter is HUGE (vectors, strings, arrays)
  *and* you don't need to modify the values of the parameter.

  3) Pass by reference when the parameter is HUGE (vectors, strings, arrays) *and*
  you need to modify the values of the parameter.

  ```
    // DON'T DO:
    void funcProcess(std::vector<std::complex<float>> signal)  // UNNECESSARY COPY CREATED
    size_t func2Proc(const float &numOfProcesses)   // Reference to a small data type is nyeeh

    // DO:
    void funcProcess(std::vector<std::complex<float>> &signal) 
    // OR, If you're not going to modify the data either,
    void funcProcess(const std::vector<std::complex<float>> &signal)
  ```

You can also use const pointers in for loops as such
```
// GOOD
for (const float &val : values){
  func2Proc(val);
}

```

You're not losing your mind, C++ and C have different ways of function declaration where you pass by reference.
In C, if you're passing a large variable by reference, you don't give the function the variable but instead pass a pointer to the variable as such:
```
void operateUrM0(char *name, unsigned int *arr, int* num_people){
  // ...
}

And then when you call the function on the variables player_name and player_array:
```
char player_name[20] = "John Conway";
unsigned int player_array[500] = {0};
int total_players = 692;
int *player_ref = &total_players;

operateUrM0(player_name, player_array, player_ref);
// Array's are only stored by the pointer of their first elements so no change there.
```

However, in C++ you use & in the function declaration to indicate reference to a variable in memory without making a copy.
There's no special syntax when you call the function afterwards.
```
void operateUrM0(char &name, unsigned int &arr){
  // ...
}


```









Appendix
---------
Var types:

int             |   2 bytes
short           |   2
long            |   4
long long       |   8
                +
float           |   4
double          |   8
long double     |  12
                +
char            |   1
wchar_t         | 2/4
char16_t        |
char32_t        |
                +
bool            |
                |
std::string     |

Data type modifiers:
  Prefixes to data types to change them to a different range/capacity.
long
short
signed
unsigned

