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

#ifndef REDUCER_HPP
#define REDUCER_HPP

///SeqMR_Reducer.
template<class KeyA, class A, class KeyB, class B, class C>
class SeqMR_Reducer : public Reducer<KeyA, A, KeyB, B, C>
{
public:
   ///Constructor.
   SeqMR_Reducer(MapReduce<KeyA, A, KeyB, B, C>* _mapReduce)
     : mapReduce(_mapReduce){};
   ///Iterator reducing execution (implemented by library).
#ifndef MRI_USE_MULTIMAP
   virtual std::vector<std::pair<KeyB, C>> run(std::vector<pair<KeyB, B>>& _mapped)
   {
      multimap<KeyB, B> mapped;
      for (int i = 0; i < _mapped.size(); i++)
         mapped.insert(_mapped[i]);
#else
   virtual std::vector<std::pair<KeyB, C>> run(multimap<KeyB, B>& mapped)
   {
#endif
      std::vector<std::pair<KeyB, C>> reduced;

      typename std::multimap<KeyB, B>::iterator it = mapped.begin();
      KeyB lastKey = (*it).first;
      vector<B> toReduce;
      for (; it != mapped.end(); ++it) {
         if ((*it).first != lastKey) {
            pair<KeyB, C> r = reduce(pair<KeyB, vector<B>>(lastKey, toReduce));
            toReduce.clear();
            reduced.push_back(r);
            lastKey = (*it).first;
         }
         toReduce.push_back((*it).second);
      }

      pair<KeyB, C> r = reduce(pair<KeyB, vector<B>>(lastKey, toReduce));
      toReduce.clear();
      reduced.push_back(r);

      return reduced;
   };
   ///Reduce function (implemented by user).
   virtual pair<KeyB, C> reduce(pair<KeyB, vector<B>>) = 0;

protected:
   MapReduce<KeyA, A, KeyB, B, C>* mapReduce;
};

///SeqMR_StrReducer.
class SeqMR_StrReducer : public SeqMR_Reducer<string, string, string, string, string>
{
public:
   ///Constructor.
   SeqMR_StrReducer(MapReduce<string, string, string, string, string>* _mapReduce)
     : SeqMR_Reducer<string, string, string, string, string>(_mapReduce){};
};

#endif /* REDUCER_HPP */
