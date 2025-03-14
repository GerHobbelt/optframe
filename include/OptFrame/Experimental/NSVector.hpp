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

/*
Author: Sabir Ribas
Date: 2010-03-10
Change Log:
	2010-03-10	NSVector idealization and development of safe insertion, remotion and Shiftk neighborhood structure for one or more vectors (e.g. vector<T> or vector< vector<T> >).
	2010-03-16	Development of Swapk neighborhood structure for template vector of vector.
*/

#ifndef NSVECTOR_HPP
#define NSVECTOR_HPP

#include <cmath>
#include <cstdio>
#include <iostream>
#include <vector>

using namespace std;

template<class T>
class NSVector
{
public:
   /* safe insertion and remotion - 2010-03-10 */

   static bool insert(std::vector<T>& v, unsigned int rank, T o)
   {
      if (rank < 0 || rank > v.size()) {
         cerr << "Warning [NSVector]: out of range insertion" << std::endl;
         return false;
      }
      v.insert(v.begin() + rank, o);
      return true;
   };

   static bool remove(std::vector<T>& v, unsigned int rank)
   {
      if (rank < 0 || rank >= v.size()) {
         cerr << "Warning [NSVector]: out of range remotion" << std::endl;
         return false;
      }
      v.erase(v.begin() + rank);
      return true;
   };

   /* Shiftk - 2010-03-10 */

   static bool shift1_apply(std::vector<T>& v, unsigned int rank1, unsigned int rank2)
   {
      if (rank1 < 0 || rank1 >= v.size() || rank2 < 0 || rank2 >= v.size()) {
         cerr << "Warning [NSVector]: out of range shift1" << std::endl;
         return false;
      }
      /*
		if (rank1 < rank2){
			v.insert(v.begin()+rank2+1,v[rank1]);
			v.erase(v.begin()+rank1);
		} else 
		if (rank1 > rank2){
			v.insert(v.begin()+rank2,v[rank1]);
			v.erase(v.begin()+rank1+1);
		}
		*/
      T aux;
      if (rank1 < rank2)
         for (int i = rank1; i < rank2; i++) {
            aux = v[i];
            v[i] = v[i + 1];
            v[i + 1] = aux;
         }
      else
         for (int i = rank1; i > rank2; i--) {
            aux = v[i];
            v[i] = v[i - 1];
            v[i - 1] = aux;
         }
      return true;
   };

   static bool shiftk_apply(std::vector<T>& v, unsigned int k, unsigned int rank1, unsigned int rank2)
   {
      if (rank1 < 0 || rank1 + k > v.size() || rank2 < 0 || rank2 + k > v.size()) {
         cerr << "Warning [NSVector]: out of range shiftk" << std::endl;
         return false;
      }
      if (rank1 < rank2)
         for (int i = k - 1; i + 1 > 0; i--)
            shift1_apply(v, rank1 + i, rank2 + i);
      else if (rank1 > rank2)
         for (int i = 0; i < k; i++)
            shift1_apply(v, rank1 + i, rank2 + i);
      return true;
   };

   static pair<int, pair<int, int>> shiftk_apply(std::vector<T>& v, pair<int, pair<int, int>>& m)
   {
      int k = m.first, rank1 = m.second.first, rank2 = m.second.second;
      if (shiftk_apply(v, k, rank1, rank2))
         return pair<int, pair<int, int>>(k, pair<int, int>(rank2, rank1));
   };

   static int shift1_size(const vector<T>& v) { return v.size() * v.size(); };

   static pair<int, pair<int, int>> shiftk_move(const vector<T>& v, int k, int i)
   {
      return pair<int, pair<int, int>>(k, pair<int, int>((i / v.size()), (i % v.size())));
   };

   static bool shiftk_canBeApplied(const vector<T>& v, pair<int, pair<int, int>>& m)
   {
      int k = m.first, rank1 = m.second.first, rank2 = m.second.second;
      return !(rank1 < 0 || rank1 + k > v.size() || rank2 < 0 || rank2 + k > v.size() || rank1 == rank2);
   };

   static std::vector<std::pair<int, pair<int, int>>>* shiftk_appliableMoves(const vector<T>& v, int k)
   {
      std::vector<std::pair<int, pair<int, int>>>* moves = new std::vector<std::pair<int, pair<int, int>>>;
      for (int i = 0; i < NSVector<int>::shift1_size(v); i++) {
         pair<int, pair<int, int>> m = NSVector<int>::shiftk_move(v, k, i);
         if (NSVector<int>::shiftk_canBeApplied(v, m))
            moves->push_back(m);
      }
      return moves;
   };

   /* Shiftk with Two Vectors - 2010-03-10 */

   static bool shift1_apply(std::vector<T>& v1, vector<T>& v2, unsigned int rank1, unsigned int rank2)
   {
      if (rank1 < 0 || rank1 >= v1.size() || rank2 < 0 || rank2 > v2.size()) {
         cerr << "Warning [NSVector]: out of range shift1 (v1,v2)" << std::endl;
         return false;
      }
      insert(v2, rank2, v1[rank1]);
      remove(v1, rank1);
      return true;
   };

   static bool shiftk_apply(std::vector<T>& v1, vector<T>& v2, unsigned int k, unsigned int rank1, unsigned int rank2)
   {
      if (rank1 < 0 || rank1 + k > v1.size() || rank2 < 0 || rank2 > v2.size()) {
         cerr << "Warning [NSVector]: out of range shiftk (v1,v2)" << std::endl;
         return false;
      }
      for (int i = k - 1; i + 1 > 0; i--)
         shift1_apply(v1, v2, rank1 + i, rank2);
      return true;
   };

   static pair<int, pair<int, int>> shiftk_apply(std::vector<T>& v1, vector<T>& v2, pair<int, pair<int, int>>& m)
   {
      int k = m.first, rank1 = m.second.first, rank2 = m.second.second;
      if (shiftk_apply(v1, v2, k, rank1, rank2))
         return pair<int, pair<int, int>>(k, pair<int, int>(rank2, rank1));
   };

   static int shift1_size(const vector<T>& v1, const vector<T>& v2) { return v1.size() * (v2.size() + 1); };

   static pair<int, pair<int, int>> shiftk_move(const vector<T>& v1, const vector<T>& v2, int k, int i)
   {
      return pair<int, pair<int, int>>(k, pair<int, int>((i / (v2.size() + 1)), (i % (v2.size() + 1))));
   };

   static bool shiftk_canBeApplied(std::vector<T>& v1, vector<T>& v2, pair<int, pair<int, int>>& m)
   {
      int k = m.first, rank1 = m.second.first, rank2 = m.second.second;
      return !(rank1 < 0 || rank1 + k > v1.size() || rank2 < 0 || rank2 > v2.size());
   };

   static std::vector<std::pair<int, pair<int, int>>>* shiftk_appliableMoves(std::vector<T>& v1, vector<T>& v2, int k)
   {
      std::vector<std::pair<int, pair<int, int>>>* moves = new std::vector<std::pair<int, pair<int, int>>>;
      for (int i = 0; i < NSVector<int>::shift1_size(v1, v2); i++) {
         pair<int, pair<int, int>> m = NSVector<int>::shiftk_move(v1, v2, k, i);
         if (NSVector<int>::shiftk_canBeApplied(v1, v2, m))
            moves->push_back(m);
      }
      return moves;
   };

   static bool shiftk_canBeApplied(const vector<vector<T>>& v, pair<int, pair<pair<int, int>, pair<int, int>>>& m)
   {
      int k = m.first, a = m.second.first.first, rank1 = m.second.first.second, b = m.second.second.first, rank2 = m.second.second.second;
      return !(rank1 < 0 || rank1 + k > v[a].size() || rank2 < 0 || rank2 > v[b].size() || a == b);
   };

   static std::vector<std::pair<int, pair<pair<int, int>, pair<int, int>>>>* shiftk_appliableMoves(const vector<vector<T>>& v, int k)
   {
      std::vector<std::pair<int, pair<pair<int, int>, pair<int, int>>>>* moves = new std::vector<std::pair<int, pair<pair<int, int>, pair<int, int>>>>;

      for (int a = 0; a < v.size(); a++) {
         for (int b = 0; b < v.size(); b++) {
            if (a == b)
               continue;

            int size = NSVector<int>::shift1_size(v[a], v[b]);

            for (int i = 0; i < size; i++) {
               pair<int, pair<int, int>> mLinha = NSVector<int>::shiftk_move(v[a], v[b], k, i);
               pair<int, pair<pair<int, int>, pair<int, int>>> m;
               m.first = mLinha.first;
               m.second.first.first = a;
               m.second.first.second = mLinha.second.first;
               m.second.second.first = b;
               m.second.second.second = mLinha.second.second;
               if (NSVector<int>::shiftk_canBeApplied(v, m))
                  moves->push_back(m);
            }
         }
      }

      return moves;
   };

   static pair<int, pair<pair<int, int>, pair<int, int>>>
   shiftk_apply(std::vector<vector<T>>& v, pair<int, pair<pair<int, int>, pair<int, int>>>& m)
   {
      int k = m.first, a = m.second.first.first, rank1 = m.second.first.second, b = m.second.second.first, rank2 = m.second.second.second;
      if (shiftk_apply(v[a], v[b], k, rank1, rank2))
         return pair<int, pair<pair<int, int>, pair<int, int>>>(k,
                                                                pair<pair<int, int>, pair<int, int>>(pair<int, int>(b, rank2), pair<int, int>(a, rank1)));
   };

   /* Swapk */

   //...

   /* Swapk with Two Vectors - 2010-03-16 */

   static bool swap1_apply(std::vector<T>& v1, vector<T>& v2, unsigned int rank1, unsigned int rank2)
   {
      if (rank1 < 0 || rank1 >= v1.size() || rank2 < 0 || rank2 >= v2.size()) {
         cerr << "Warning [NSVector]: out of range swap1 (v1,v2)" << std::endl;
         return false;
      }
      T aux = v1[rank1];
      v1[rank1] = v2[rank2];
      v2[rank2] = aux;
      return true;
   };

   static bool swapk_apply(std::vector<T>& v1, vector<T>& v2, unsigned int k1, unsigned int k2, unsigned int rank1, unsigned int rank2)
   {
      if (rank1 < 0 || rank1 + k1 > v1.size() || rank2 < 0 || rank2 + k2 > v2.size()) {
         cerr << "Warning [NSVector]: out of range swapk (v1,v2)" << std::endl;
         return false;
      }
      if (k1 == k2)
         for (int i = 0; i < k1; i++)
            swap1_apply(v1, v2, rank1 + i, rank2 + i);
      else if (k1 > k2) {
         for (int i = 0; i < k1 - k2; i++)
            swap1_apply(v1, v2, rank1 + i, rank2 + i);
         shiftk_apply(v1, v2, k1 - k2, rank1 + k2, rank2 + k2);
      } else if (k1 < k2) {
         for (int i = 0; i < k2 - k1; i++)
            swap1_apply(v1, v2, rank1 + i, rank2 + i);
         shiftk_apply(v2, v1, k2 - k1, rank2 + k1, rank1 + k1);
      }
      return true;
   };

   static pair<pair<int, int>, pair<int, int>> shiftk_apply(std::vector<T>& v1, vector<T>& v2, pair<pair<int, int>, pair<int, int>>& m)
   {
      int k1 = m.first.first, k2 = m.first.second, rank1 = m.second.first, rank2 = m.second.second;
      if (swapk_apply(v1, v2, k1, k2, rank1, rank2))
         return pair<pair<int, int>, pair<int, int>>(pair<int, int>(k2, k1), pair<int, int>(rank2, rank1));
   };

   static int swap1_size(const vector<T>& v1, const vector<T>& v2) { return v1.size() * v2.size(); };

   static pair<pair<int, int>, pair<int, int>> swapk_move(const vector<T>& v1, const vector<T>& v2, int k1, int k2, int i)
   {
      return pair<pair<int, int>, pair<int, int>>(pair<int, int>(k1, k2), pair<int, int>(i / v2.size(), i % v2.size()));
   };

   static bool swapk_canBeApplied(const vector<T>& v1, const vector<T>& v2, pair<pair<int, int>, pair<int, int>>& m)
   {
      int k1 = m.first.first, k2 = m.first.second, rank1 = m.second.first, rank2 = m.second.second;
      return !(rank1 < 0 || rank1 + k1 > v1.size() || rank2 < 0 || rank2 + k2 > v2.size());
   };

   static std::vector<std::pair<pair<int, int>, pair<int, int>>>* swapk_appliableMoves(std::vector<T>& v1, vector<T>& v2, int k1, int k2)
   {
      std::vector<std::pair<pair<int, int>, pair<int, int>>>* moves = new std::vector<std::pair<pair<int, int>, pair<int, int>>>;
      for (int i = 0; i < NSVector<int>::swap1_size(v1, v2); i++) {
         pair<pair<int, int>, pair<int, int>> m = NSVector<int>::swapk_move(v1, v2, k1, k2, i);
         if (NSVector<int>::swapk_canBeApplied(v1, v2, m))
            moves->push_back(m);
      }
      return moves;
   };

   static bool swapk_canBeApplied(const vector<vector<T>>& v, pair<pair<int, int>, pair<pair<int, int>, pair<int, int>>>& m)
   {
      int k1 = m.first.first, k2 = m.first.second,
          a = m.second.first.first, rank1 = m.second.first.second, b = m.second.second.first, rank2 = m.second.second.second;
      return !(rank1 < 0 || rank1 + k1 > v[a].size() || rank2 < 0 || rank2 + k2 > v[b].size() || a == b);
   };

   static std::vector<std::pair<pair<int, int>, pair<pair<int, int>, pair<int, int>>>>* swapk_appliableMoves(const vector<vector<T>>& v, int k1, int k2)
   {
      std::vector<std::pair<pair<int, int>, pair<pair<int, int>, pair<int, int>>>>* moves =
        new std::vector<std::pair<pair<int, int>, pair<pair<int, int>, pair<int, int>>>>;

      for (int a = 0; a < v.size(); a++) {
         //for (int b = 0 ; b < v.size() ; b++)
         for (int b = a + 1; b < v.size(); b++) {
            //if (a==b) continue;

            int size = NSVector<int>::swap1_size(v[a], v[b]);

            for (int i = 0; i < size; i++) {
               pair<pair<int, int>, pair<int, int>> mLinha = NSVector<int>::swapk_move(v[a], v[b], k1, k2, i);
               pair<pair<int, int>, pair<pair<int, int>, pair<int, int>>> m;
               m.first = mLinha.first; // pair
               m.second.first.first = a;
               m.second.first.second = mLinha.second.first;
               m.second.second.first = b;
               m.second.second.second = mLinha.second.second;
               if (NSVector<int>::swapk_canBeApplied(v, m))
                  moves->push_back(m);
            }
         }
      }

      return moves;
   };

   static pair<pair<int, int>, pair<pair<int, int>, pair<int, int>>>
   swapk_apply(std::vector<vector<T>>& v, pair<pair<int, int>, pair<pair<int, int>, pair<int, int>>>& m)
   {
      int k1 = m.first.first, k2 = m.first.second,
          a = m.second.first.first, rank1 = m.second.first.second, b = m.second.second.first, rank2 = m.second.second.second;
      if (swapk_apply(v[a], v[b], k1, k2, rank1, rank2))
         return pair<pair<int, int>, pair<pair<int, int>, pair<int, int>>>(pair<int, int>(k2, k1),
                                                                           pair<pair<int, int>, pair<int, int>>(pair<int, int>(b, rank2), pair<int, int>(a, rank1)));
   };

   /* Swapk - 2010-02-11 */

   static bool swap1_apply(std::vector<T>& v, unsigned int rank1, unsigned int rank2)
   {
      if (rank1 < 0 || rank1 >= v.size() || rank2 < 0 || rank2 >= v.size()) {
         cerr << "Warning [NSVector]: out of range swap1" << std::endl;
         return false;
      }

      T aux = v[rank1];
      v[rank1] = v[rank2];
      v[rank2] = aux;

      return true;
   };

   static bool swapk_apply(std::vector<T>& v, unsigned int k1, unsigned int k2, unsigned int rank1, unsigned int rank2)
   {
      if (rank1 < 0 || rank1 + k1 > v.size() || rank2 < 0 || rank2 + k2 > v.size()) {
         cerr << "Warning [NSVector]: out of range swapk" << std::endl;
         return false;
      }
      int k = min(k1, k2);
      for (int i = 0; i < k; i++)
         swap1_apply(v, rank1 + i, rank2 + i);
      if (k2 > k1)
         shiftk_apply(v, k2 - k, rank2 + k, rank1 + k);
      else if (k1 > k2)
         shiftk_apply(v, k1 - k, rank1 + k, rank2 + k);
      return true;
   };

   static pair<pair<int, int>, pair<int, int>> swapk_apply(std::vector<T>& v, pair<pair<int, int>, pair<int, int>>& m)
   {
      int k1 = m.first.first, k2 = m.first.second, rank1 = m.second.first, rank2 = m.second.second;
      if (swapk_apply(v, k1, k2, rank1, rank2))
         return pair<pair<int, int>, pair<int, int>>(pair<int, int>(k2, k1), pair<int, int>(rank2, rank1));
   };

   static int swap1_size(const vector<T>& v) { return v.size() * v.size(); };

   static pair<pair<int, int>, pair<int, int>> swapk_move(const vector<T>& v, int k1, int k2, int i)
   {
      return pair<pair<int, int>, pair<int, int>>(pair<int, int>(k1, k2), pair<int, int>((i / v.size()), (i % v.size())));
   };

   static bool swapk_canBeApplied(const vector<T>& v, pair<pair<int, int>, pair<int, int>>& m)
   {
      int k1 = m.first.first, k2 = m.first.second, rank1 = m.second.first, rank2 = m.second.second;
      return !(rank1 < 0 || rank1 + k1 > v.size() || rank2 < 0 || rank2 + k2 > v.size() || rank1 == rank2 ||
               !(rank2 + k2 <= rank1 && !rank1 + k1 <= rank2 || !rank2 + k2 <= rank1 && rank1 + k1 <= rank2));
   };

   static std::vector<std::pair<pair<int, int>, pair<int, int>>>* swapk_appliableMoves(const vector<T>& v, int k1, int k2)
   {
      std::vector<std::pair<pair<int, int>, pair<int, int>>>* moves = new std::vector<std::pair<pair<int, int>, pair<int, int>>>;
      for (int i = 0; i < NSVector<int>::swap1_size(v); i++) {
         pair<pair<int, int>, pair<int, int>> m = NSVector<int>::swapk_move(v, k1, k2, i);
         if (NSVector<int>::swapk_canBeApplied(v, m))
            moves->push_back(m);
      }
      return moves;
   };
};

#endif /* NSVECTOR_HPP */
