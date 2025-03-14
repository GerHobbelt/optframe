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

#ifndef OPTFRAME_NSENUMVVSHIFTK_HPP_
#define OPTFRAME_NSENUMVVSHIFTK_HPP_

#include <algorithm>
#include <vector>

// Framework includes
#include "../Move.hpp"
#include "../NSEnum.hpp"
#include "../NSEnumIterator.hpp"
#include "../NSIterator.hpp"
#include "Moves/MoveVVShiftk.hpp"

using namespace std;

//============================================================================
//                  Shift Neighborhood Structure
//============================================================================

template<class T, class DS>
class NSEnumVVShiftk : public NSEnum<vector<vector<T>>, DS>
{
protected:
   int k;
   std::vector<std::pair<int, pair<pair<int, int>, pair<int, int>>>>* moves;

public:
   using NSEnum<vector<vector<T>>, DS>::move; // prevents name hiding

   NSEnumVVShiftk(int k)
   {
      this->k = k;
      moves = new std::vector<std::pair<int, pair<pair<int, int>, pair<int, int>>>>;
   }

   /*virtual void init(Solution<vector<vector<int> > >* s)
	{
		init(s->getR());
	}

	virtual void init(std::vector<vector<int> >& rep)
	{
		delete moves;
		moves = NSVector<int>::shiftk_appliableMoves(rep,k);
	}*/

   virtual NSIterator<vector<vector<T>>, DS>& getIterator(const vector<vector<T>>& rep)
   {
      delete moves;
      moves = NSVector<int>::shiftk_appliableMoves(rep, k);
      random_shuffle(moves->begin(), moves->end());

      return *new NSEnumIterator<vector<vector<T>>, DS>(*this);
   }

   virtual Move<vector<vector<T>>, DS>& move(unsigned int _k)
   {
      if (_k > size()) {
         cerr << "Neighborhood Shift Error! Move " << _k << " does not exist! Valid Interval from 0 to " << (size() - 1) << "." << std::endl;
         exit(1);

         //return nullptr;
      }

      return *new MoveVVShiftk<T, DS>((*moves)[_k].first,
                                      (*moves)[_k].second.first.first,
                                      (*moves)[_k].second.first.second,
                                      (*moves)[_k].second.second.first,
                                      (*moves)[_k].second.second.second);
   }

   virtual Move<vector<vector<T>>, DS>& move(const vector<vector<T>>& rep)
   {
      //cout << "*";
      int v1;
      do
         v1 = rand() % rep.size();
      while (rep[v1].size() < k);

      int p1 = rand() % (rep[v1].size() - k + 1);

      int v2;
      do
         v2 = rand() % rep.size();
      while (v1 == v2);

      int p2 = rand() % (rep[v2].size() + 1);

      return *new MoveVVShiftk<T, DS>(k, v1, p1, v2, p2);
   };

   virtual unsigned int size()
   {
      return moves->size();
   }

   virtual void print()
   {
      std::cout << "NSEnum Vector Vector Shiftk (" << size() << ")" << std::endl;
   }
};

#endif /*OPTFRAME_NSENUMVVSHIFTK_HPP_*/
