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
Example: Word Counter with MapReduce Interface with MapMP
Author : Sabir Ribas
Date   : 2009-02-09
*/

#include "../MapMP/MapMP.h"
#include "../util/Scanner++/Scanner.h"
#include <iostream>
#include <time.h>

class MyMapper : public MapMP_Mapper<string, string, string, int, int>
{
public:
   MyMapper(MapReduce<string, string, string, int, int>* mr)
     : MapMP_Mapper<string, string, string, int, int>(mr){};
   virtual std::vector<std::pair<string, int>> map(pair<std::string, std::string> a) // Map function implementation
   {
      std::vector<std::pair<string, int>> mapped;
      Scanner scan(a.second);
      scan.useSeparators(string(" \n\r\t\\\'\"!@#^&*()+={[}]|:;,<>./?"));
      while (scan.hasNext())
         mapped.push_back(pair<string, int>(scan.next(), 1));
      return mapped;
   };
};

class MyReducer : public MapMP_Reducer<string, string, string, int, int>
{
public:
   MyReducer(MapReduce<string, string, string, int, int>* mr)
     : MapMP_Reducer<string, string, string, int, int>(mr){};
   virtual pair<string, int> reduce(pair<string, vector<int>> bs) // Reduce function implementation
   {
      int reduced = 0;
      for (std::vector<int>::iterator it = bs.second.begin(); it != bs.second.end(); ++it)
         reduced += *it;
      return pair<string, int>(bs.first, reduced);
   };
};

int
main()
{
   std::cout << "MapMP_WorldCounter test" << std::endl;

   // Reading text
   Scanner scan(new File("doc"));
   string text;
   while (scan.hasNext())
      text += scan.next() + " ";

   // MapReduce input
   std::vector<std::pair<std::string, std::string>> input;
   input.push_back(pair<std::string, std::string>("doc", text));

   // MapReduce declaration and application
   MapMP_MapReduce<string, string, string, int, int> mapReduce;
   MyMapper mapper(&mapReduce);
   MyReducer reducer(&mapReduce);

   std::vector<std::pair<string, int>> output = mapReduce.run(mapper, reducer, input);

   // Output exibition
   std::cout << "MapReduce word counter result" << std::endl;
   multimap<int, string> sortedOutput;
   for (std::vector<pair<string, int>>::iterator it = output.begin(); it != output.end(); it++)
      sortedOutput.insert(pair<int, string>((*it).second, (*it).first));
   for (multimap<int, string>::reverse_iterator it = sortedOutput.rbegin(); it != sortedOutput.rend(); it++)
      std::cout << *it << std::endl;

   return 0;
}
