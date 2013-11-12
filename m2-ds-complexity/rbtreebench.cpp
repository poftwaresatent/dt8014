#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

#include <set>
#include <list>


/* Returns a random integer in the specified range. */
static int random_int (int minval, int maxval);

/* Returns the current time as milliseconds in double format. The first
   time this function is called, it resets an internal reference and
   returns zero. From then on it returns the number of milliseconds
   since that reset. */
static double clockms (void);


int main (int argc, char ** argv)
{
  static const int    dmin        (0);
  static const int    dmax   (100000);
  static const size_t nstart  (20000);
  static const size_t nmax  (2000000);
  static const size_t nprobe   (1000);
  static const double nfac (1.2);
  
  std::multiset<int> rbtree;
  int * dins (new int[nprobe]);
  int * dfind (new int[nprobe]);
  std::list<size_t> nn;
  std::list<double> tins, tfind;
  
  for (double nd (nstart); nd <= nmax; nd *= nfac) {
    
    nn.push_back (size_t (nd));
    fprintf (stderr, "%8zu\n", nn.back());
    
    while (rbtree.size() < nn.back()) {
      rbtree.insert (random_int (dmin, dmax));
    }
    for (size_t ii (0); ii < nprobe; ++ii) {
      dins[ii] = random_int (dmin, dmax);
      dfind[ii] = ii % 2 ? random_int (dmin, dmax) : dins[ii];
    }
    
    double const t0 (clockms());
    for (size_t ii (0); ii < nprobe; ++ii) {
      rbtree.insert (dins[ii]);
    }
    double const t1 (clockms());
    for (size_t ii (0); ii < nprobe; ++ii) {
      rbtree.find (dins[ii]);
    }
    double const t2 (clockms());
    
    tins.push_back (t1 - t0);
    tfind.push_back (t2 - t1);
  }
  
  delete[] dins;
  delete[] dfind;
  
  printf ("\n"
	  "set key left\n"
	  "set xlabel 'size N'\n"
	  "set ylabel 'time [ms]'\n"
	  "set title 'rbtree insert/find %zu items in range %d...%d'\n"
	  "plot '-' u 1:2 w l t 'insert', '-' u 1:2 w l t 'find'\n"
	  "# tins\n",
	  nprobe, dmin, dmax);
  auto in (nn.begin());
  for (auto ii (tins.begin()); ii != tins.end(); ++ii, ++in) {
    printf ("%8zu  %8g\n", *in, *ii);
  }
  printf ("e\n"
	  "# tfind\n");
  in = nn.begin();
  for (auto ii (tfind.begin()); ii != tfind.end(); ++ii, ++in) {
    printf ("%8zu  %8g\n", *in, *ii);
  }
}


int random_int (int minval, int maxval)
{
  static int fd = -1;
  unsigned int delta, tmp, mask;
  
  if (0 > fd && 0 > (fd = open ("/dev/urandom", O_RDONLY))) {
    err (EXIT_FAILURE, __FILE__": %s: open /dev/urandom", __func__);
  }
  
  if (maxval < minval) {
    tmp = minval;
    minval = maxval;
    maxval = tmp;
  }
  if (0 == (delta = maxval - minval)) {
    return minval;
  }
  
  for (tmp = 1 << (8 * sizeof(int) - 1); 0 == (tmp & delta); tmp >>= 1) {
    /* nop */;
  }
  for (mask = tmp; 0 != tmp; tmp >>= 1) {
    mask |= tmp;
  }
  
  for (;;) {
    if (sizeof(int) != read (fd, &tmp, sizeof(int))) {
      err (EXIT_FAILURE, __FILE__": %s: read /dev/urandom", __func__);
    }
    tmp &= mask;
    if (tmp <= delta) {
      break;
    }
  }
  
  return minval + tmp;
}


double clockms (void)
{
  static struct timeval t0 = { 0, 0 };
  struct timeval t1;
  
  if (0 == t0.tv_sec) {
    if (0 != gettimeofday (&t0, NULL)) {
      err (EXIT_FAILURE, __FILE__": %s: gettimeofday", __func__);
    }
  }
  if (0 != gettimeofday (&t1, NULL)) {
    err (EXIT_FAILURE, __FILE__": %s: gettimeofday", __func__);
  }
  
  return 1e3 * (t1.tv_sec - t0.tv_sec) + 1e-3 * (t1.tv_usec - t0.tv_usec);
}
