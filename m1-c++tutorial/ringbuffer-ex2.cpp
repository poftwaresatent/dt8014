/*
  Example of a typical class template which wraps an existing STL
  container.
*/

#include <iostream>
#include <list>

using namespace std;


template<typename value_t>
class Ringbuffer
{
public:
  /*
    Here, the preferred initializer-list format is used for
    implementing the constructor: a constructor for each attribute (or
    "field") of the class is directly called.
  */
  Ringbuffer (size_t cap)
    : cap_(cap)
  {
  }
  
  /*
    Notice that we do not need a custom destructor anymore.
  */
  
  void insert (value_t val)
  {
    values_.push_back (val);
    while (values_.size() > cap_) {
      values_.pop_front ();
    }
  }
  
  /*
    Again, do not call this when empty() returns true!  In this case,
    std::list<>::front() would return an invalid reference.  Again, an
    exception could be thrown or a special item returned to indicate
    that the buffer is empty.
  */
  value_t get ()
  {
    value_t val (values_.front ());
    values_.pop_front ();
    return val;
  }
  
  bool empty ()
  {
    return values_.empty ();
  }
  
private:
  size_t cap_;
  list<value_t> values_;
};


int main ()
{
  for (int chunk (1); chunk < 10; chunk += 2) {
    Ringbuffer<int> buf (5);
    int val (10);
    for (int jj (0); jj < 3; ++jj) {
      cout << "i";
      for (int ii (0); ii < chunk; ++ii, ++val) {
	buf.insert (val);
	cout << " " << val;
      }
      cout << "\ng";
      for (int ii (0); ii < 4 && ! buf.empty (); ++ii) {
	cout << " " << buf.get ();
      }
      cout << "\n";
    }
  }
}
