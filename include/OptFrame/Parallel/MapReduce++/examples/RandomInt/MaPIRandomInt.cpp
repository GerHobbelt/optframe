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

/* MapReduce Interface
Project: 
Author : 
Date   : 
*/

#include "../../MaPI/MaPI.h"
#include "../../util/Timer.hpp"
#include <cstdlib>
#include <iostream>

class MySerializer : public MaPI_Serializer<int, int, int, int, int>
{
public:
   virtual int KeyA_fromString(string s) { return atoi(s.c_str()); };
   virtual string KeyA_toString(int o)
   {
      stringstream s;
      s << o;
      return s.str();
   };

   virtual int A_fromString(string s) { return atoi(s.c_str()); };
   virtual string A_toString(int o)
   {
      stringstream s;
      s << o;
      return s.str();
   };

   virtual int KeyB_fromString(string s) { return atoi(s.c_str()); };
   virtual string KeyB_toString(int o)
   {
      stringstream s;
      s << o;
      return s.str();
   };

   virtual int B_fromString(string s) { return atoi(s.c_str()); };
   virtual string B_toString(int o)
   {
      stringstream s;
      s << o;
      return s.str();
   };

   virtual int C_fromString(string s) { return atoi(s.c_str()); };
   virtual string C_toString(int o)
   {
      stringstream s;
      s << o;
      return s.str();
   };
};

class MyMapper : public MaPI_Mapper<int, int, int, int, int>
{
public:
   MyMapper(MaPI_MapReduce<int, int, int, int, int>* mr, MaPI_Serializer<int, int, int, int, int>* s)
     : MaPI_Mapper<int, int, int, int, int>(mr, s){};
   virtual std::vector<std::pair<int, int>> map(pair<int, int> a)
   {
      std::vector<std::pair<int, int>> m;
      //cout << "\tMapping..\n";
      m.push_back(std::make_pair(a.first, a.second));
      for (int i = 0; i < 100000000; i++)
         ;
      //cout << "\tOk\n";
      return m;
   };
};

class MyReducer : public MaPI_Reducer<int, int, int, int, int>
{
public:
   MyReducer(MaPI_MapReduce<int, int, int, int, int>* mr, MaPI_Serializer<int, int, int, int, int>* s)
     : MaPI_Reducer<int, int, int, int, int>(mr, s){};
   virtual pair<int, int> reduce(pair<int, vector<int>> bs)
   {
      int reduced = 0;
      //cout << "\tReducing..\n";
      for (int i = 0; i < bs.second.size(); i++)
         reduced += bs.second[i];
      for (int i = 0; i < 100000000; i++)
         ;
      //cout << "\tOk\n";
      return pair<int, int>(bs.first, reduced);
   };
};

int
main(int argc, char** argv)
{
   std::cout << "MaPI test" << std::endl;

   Timer timer;

   MaPI_MapReduce<int, int, int, int, int> mapReduce;
   MySerializer serializer;
   MyMapper mapper(&mapReduce, &serializer);
   MyReducer reducer(&mapReduce, &serializer);
   mapReduce.initServers(argc, argv);

   int n = 10;
   srand(n);

   std::vector<std::pair<int, int>> input;
   for (int i = 0; i < n; i++)
      input.push_back(std::make_pair(rand() % n, rand() % n));

   std::vector<std::pair<int, int>> output = mapReduce.run(mapper, reducer, input);

   return 0;
}
