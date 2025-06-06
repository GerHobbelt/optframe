// OptFrame - Optimization Framework

// Copyright (C) 2009, 2010, 2011
// http://optframe.sourceforge.net/
//
// This file is part of the OptFrame optimization framework. This framework
// is free software; you can redistribute it and/or modify it under the
// terms of the GNU Lesser General Public License v3 as published by the
// Free Software Foundation.

// This framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License v3 for more details.

// You should have received a copy of the GNU Lesser General Public License v3
// along with this library; see the file COPYING.  If not, write to the Free
// Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
// USA.

#ifndef OPTFRAME_ARG_MIN_FUNCTION_HPP_
#define OPTFRAME_ARG_MIN_FUNCTION_HPP_

#include <iostream>
#include <ostream>

#include <vector>
#include <map>

#include "../Scanner++/Scanner.h"

#include "../PreprocessFunction.hpp"

#include "../OptFrameList.hpp"

#include <algorithm>

namespace optframe
{

template<class R, class ADS = OPTFRAME_DEFAULT_ADS, class DS = OPTFRAME_DEFAULT_DS>
class StatisticsArgMinFunction : public PreprocessFunction<R,ADS,DS>
{
public:

	virtual ~StatisticsArgMinFunction()
	{
	}

	virtual string id()
	{
		return "statistics.argmin";
	}

	virtual string usage()
	{
		return "statistics.argmin( list ) : return index of list minimum";
	}

	virtual string* run(std::vector<PreprocessFunction<R,ADS,DS>*>& allFunctions, HeuristicFactory<R, ADS, DS>& factory, const map< string, string >& dictionary, const map< string,vector<string> >& ldictionary, string body)
	{
		Scanner scanner(body);

		vector<string>* plist = OptFrameList::readList(ldictionary, scanner);
		vector<string>  list;
		if(plist)
		{
			list = vector<string>(*plist);
			delete plist;
		}
		else
			return nullptr;

		if(list.size()==0)
		{
			cout << "function " << id() << " error: empty list!" << std::endl;
			return nullptr;
		}

		double min = Scanner::parseDouble(list[0]);
		unsigned imin = 0;
		for(unsigned i=1; i<list.size(); i++)
		{
			double v = Scanner::parseDouble(list[i]);
			if(v < min)
			{
				min = v;
				imin = i;
			}
		}

		imin++; // list range is [1..list_length]

		stringstream ss;
		ss << imin;

		string smin = ss.str();

		return new string(smin);
	}
};

}

#endif /* OPTFRAME_ARG_MIN_FUNCTION_HPP_ */
