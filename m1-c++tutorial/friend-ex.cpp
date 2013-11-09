/*
  Example friend declarations.  One common usage is for the output
  operator for standard streams.
  
  Try removing the friend keywords from the declaration of class A and
  see what happens!
*/

#include <iostream>


class A {
public:
  A (double x) : x_(x) {}
  
private:
  double x_;
  
  /*
    This line gives access to private members of class A to a specific
    function.  In this case, the function is slightly special because
    it will be written as an operator with the same syntax as
    shift-left.
  */
  friend std::ostream & operator << (std::ostream &, A const &);
  
  /*
    This line gives access to private members of class A to all
    methods of another class, in this case class B.q
  */
  friend class B;
};


std::ostream & operator << (std::ostream & os, A const & a) {
  os << "operator version! " << a.x_;
  return os;
}


class B {
public:
  B (size_t n) : n_(n) {}
  
  void print (A const & a)
  {
    std::cout << a.x_;
    for (size_t i (1); i < n_; ++i) {
      std::cout << " " << a.x_;
    }
    std::cout << "\n";
  }

private:
  size_t n_;
};


int main ()
{
  A a (42);
  std::cout << a << "\n";
  
  B b (3);
  b.print (a);
}
