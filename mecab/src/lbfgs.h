//  MeCab -- Yet Another Part-of-Speech and Morphological Analyzer
//
//
//  Copyright(C) 2001-2006 Taku Kudo <taku@chasen.org>
//  Copyright(C) 2004-2006 Nippon Telegraph and Telephone Corporation
#ifndef MECAB_LBFGS_H_
#define MECAB_LBFGS_H_

#include <vector>
#include <iostream>

namespace MeCab {

class LBFGS {
 public:
  explicit LBFGS(): iflag_(0), iscn(0), nfev(0), iycn(0),
                    point(0), npt(0), iter(0), info(0),
                    ispt(0), isyt(0), iypt(0),
                    stp(0.0), stp1(0.0), mcsrch_(0) {}
  virtual ~LBFGS() { clear(); }

  void clear();

  int optimize(size_t size, double *x, double f, double *g,
               bool orthant, double C) {
    static const int msize = 5;
    if (w_.empty()) {
      iflag_ = 0;
      w_.resize(size * (2 * msize + 1) + 2 * msize);
      diag_.resize(size);
    } else if (diag_.size() != size) {
      std::cerr << "size of array is different" << std::endl;
      return -1;
    }

    lbfgs_optimize(static_cast<int>(size),
                   msize, x, f, g, &diag_[0], &w_[0], orthant, C, &iflag_);

    if (iflag_ < 0) {
      std::cerr << "routine stops with unexpected error" << std::endl;
      return -1;
    }

    if (iflag_ == 0) {
      clear();
      return 0;   // terminate
    }

    return 1;   // evaluate next f and g
  }

 private:
  class Mcsrch;
  int iflag_, iscn, nfev, iycn, point, npt;
  int iter, info, ispt, isyt, iypt;
  double stp, stp1;
  std::vector <double> diag_;
  std::vector <double> w_;
  Mcsrch *mcsrch_;

  void lbfgs_optimize(int size,
                      int msize,
                      double *x,
                      double f,
                      const double *g,
                      double *diag,
                      double *w, bool orthant, double C, int *iflag);
};
}

#endif
