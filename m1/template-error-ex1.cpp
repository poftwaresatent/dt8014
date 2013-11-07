/*
  Example of template error message when a given type cannot be used
  as desired by the implementation. Notice that only those methods
  which are actually used in the code trigger such errors: if you
  comment out the last line of the main() function, everything
  compiles.
  
  Notice that some compilers generate more understandable error
  messages than others. For example, g++ is notorious for difficult
  template error messages, while the clang++ developers focus on
  generating very helpful messages.
*/

#include <string>


template<typename value_t>
class Example {
public:
  Example (value_t val)
    : val_(val)
  {
  }
  
  value_t plus (value_t other)
  {
    return val_ + other;
  }
  
  value_t minus (value_t other)
  {
    return val_ - other;
  }
  
private:
  value_t val_;
};

int main ()
{
  Example<int> foo(42);
  foo.plus(17);
  foo.minus(17);
  
  Example<std::string> bar("coffee");
  bar.plus("beer");
  bar.minus("beer");
}
