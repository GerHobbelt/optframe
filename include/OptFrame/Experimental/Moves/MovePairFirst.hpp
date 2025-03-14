// OptFrame 4.2 - Optimization Framework
// Copyright (C) 2009-2021 - MIT LICENSE
// https://github.com/optframe/optframe
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef OPTFRAME_MOVEPAIRFIRST_ADAPTER_HPP_
#define OPTFRAME_MOVEPAIRFIRST_ADAPTER_HPP_

// Framework includes
#include "../../Move.hpp"

using namespace std;

template <class T1, class T2, class DS = OPTFRAME_DEFAULT_EMEMORY>
class MovePairFirst : public Move<pair<T1, T2>, DS> {
 protected:
  Move<T1, DS>& m;

 public:
  MovePairFirst(Move<T1, DS>& _m) : m(_m) {}

  Move<T1, DS>& get_m() { return m; }

  virtual ~MovePairFirst() { delete &m; }

  bool canBeApplied(const pair<T1, T2>& rep) {
    return m.canBeApplied(rep.first);
  }

  Move<pair<T1, T2>, DS>& apply(pair<T1, T2>& rep) {
    return *new MovePairFirst<T1, T2, DS>(m.apply(rep.first));
  }

  virtual bool operator==(const Move<pair<T1, T2>, DS>& _m) const {
    const MovePairFirst<T1, T2, DS>& m1 = (const MovePairFirst<T1, T2, DS>&)_m;
    return m == m1.m;
  }

  void print() const override {
    std::cout << "MovePairFirst: move = ";
    m.print();
  }
};

#endif /*OPTFRAME_MOVEPAIRFIRST_ADAPTER_HPP_*/
