/*
  Example of a typical class template.  Here, we rely on a raw array
  to store up to a given constant number of items.  The template has a
  single parameter which specifies the type of the stored items.
*/

#include <iostream>

using namespace std;


template<typename value_t>
class Ringbuffer
{
public:
  Ringbuffer (size_t cap)
  {
    values_ = new value_t[cap];
    len_ = 0;
    idx_ = 0;
    cap_ = cap;
  }
  
  ~Ringbuffer ()
  {
    delete[] values_;
  }
  
  void insert (value_t val)
  {
    values_[idx_] = val;
    idx_ = (idx_ + 1) % cap_;
    if (len_ < cap_) {
      ++len_;
    }
  }
  
  /*
    Do not call this when empty() returns true!  Indeed, when len_ is
    zero, get() will not only return something that is wrong, it will
    also underflow the len_ variable and thus possibly lead to some
    kind of corruption. A more robust implementation would maybe throw
    an exception when len_ is zero, or maybe return a special item to
    indicate that the buffer is empty.
  */
  value_t get ()
  {
    return values_[(cap_ + idx_ - len_--) % cap_];
  }
  
  bool empty ()
  {
    return 0 == len_;
  }
  
private:
  size_t cap_, len_, idx_;
  value_t * values_;
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
