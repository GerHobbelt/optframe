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

#ifndef OPTFRAME_WORDS_FUNCTION_HPP_
#define OPTFRAME_WORDS_FUNCTION_HPP_

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
class ListWordsFunction : public PreprocessFunction<R,ADS,DS>
{
public:

	virtual ~ListWordsFunction()
	{
	}

	virtual string id()
	{
		return "list.words";
	}

	virtual string usage()
	{
		return "list.words( text ) : return list_of_words";
	}

	virtual string* run(std::vector<PreprocessFunction<R,ADS,DS>*>& allFunctions, HeuristicFactory<R, ADS, DS>& factory, const map< string, string >& dictionary, const map< string,vector<string> >& ldictionary, string body)
	{
		//cout << "function words run: '" << body << "'" << std::endl;

		Scanner scanner(body);

		vector<string> list;

		while(scanner.hasNext())
			list.push_back(scanner.next());

		return new string(OptFrameList::listToString(list));
	}
};

}

#endif /* OPTFRAME_WORDS_FUNCTION_HPP_ */
