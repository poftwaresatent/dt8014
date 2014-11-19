/* Halmstad University Algorithms Course Code Examples.
 *
 * Copyright (C) 2013 Roland Philippsen. All rights reserved.
 *
 * BSD license:
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of
 *    contributors to this software may be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR THE CONTRIBUTORS TO THIS SOFTWARE BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
   \file rbtreebench.cpp
   
   Simple benchmark for STL red-black trees.
   
   \note To compile this file, you need to enable C++11 support. For
   example under Eclipse with CDT, you have to add "-std=c++11" to the
   GCC command line.
*/

#include <stdlib.h>
#include <sys/time.h>

#include <set>
#include <list>


/* Quickly hacked replacement for the UNIX err function which should
   work in this specific program also under Windows. */
static void err (int exit_code, char const * format, char const * rest);

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
  static struct timeval t0 = { 0, 0 };
  if (0 == t0.tv_sec) {
    if (0 != gettimeofday (&t0, NULL)) {
      err (EXIT_FAILURE, __FILE__": %s: gettimeofday", __func__);
    }
    srand (t0.tv_usec);
  }
  
  if (maxval < minval) {
    int tmp = minval;
    minval = maxval;
    maxval = tmp;
  }
  
  int delta;
  if (0 == (delta = maxval - minval)) {
    return minval;
  }
  
  return minval + rand () % (delta + 1);
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


void err (int exit_code, char const * format, char const * rest)
{
  fprintf (stderr, format, rest);
  exit (exit_code);
}
