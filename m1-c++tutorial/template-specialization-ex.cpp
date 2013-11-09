/*
  Example of template specialization.  If you comment-out the
  specialized verion of the bigger() template method, the output of
  the program changes.
*/

#include <iostream>
#include <cstring>


template<typename T>
bool bigger (T left, T right)
{
  if (left > right) {
    return true;
  }
  return false;
}


template<>
bool bigger<char const *> (char const * left, char const * right)
{
  if (strlen (left) > strlen (right)) {
    return true;
  }
  return false;
}


int main ()
{
  int a (5);
  int b (7);
  
  std::cout << "is " << a << " bigger than " << b << "?";
  if (bigger (a, b)) {
    std::cout << " yes!\n";
  }
  else {
    std::cout << " no!\n";
  }
  
  char const * c ("hello");
  char const * d ("bye");
  
  std::cout << "is " << c << " bigger than " << d << "?";
  if (bigger (c, d)) {
    std::cout << " yes!\n";
  }
  else {
    std::cout << " no!\n";
  }
}
