#ifndef PAIR_H
#define PAIR_H

template <typename L, typename R>
struct Pair {
public:
  L left;
  R right;

  Pair(L l, R r) {
    left = l;
    right = r;
  }

};

#endif
