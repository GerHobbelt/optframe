/* MapReduce Interface
Example: Basic example using a sequential Mapper and Reducer with MapReduce Interface
Author : Sabir Ribas
Date   : 2009-02-09
*/

#include "../MaPI/MaPI.h"
#include <iostream>

class MySerializer : public MaPI_Serializer<int,char,int,char,string>
{
public:

	virtual int		KeyA_fromString(string s)	{return atoi(s.c_str());};
	virtual string	KeyA_toString(int o)		{stringstream s; s << o; return s.str();};

	virtual char	A_fromString(string s)		{return s[0];};
	virtual string	A_toString(char o)			{stringstream s; s << o; return s.str();};

	virtual int		KeyB_fromString(string s)	{return atoi(s.c_str());};
	virtual string	KeyB_toString(int o)		{stringstream s; s << o; return s.str();};

	virtual char	B_fromString(string s)		{return s[0];};
	virtual string	B_toString(char o)			{stringstream s; s << o; return s.str();};

	virtual string	C_fromString(string s)		{return s;};
	virtual string	C_toString(string o)		{return o;};

};

class MyMapper : public MaPI_Mapper<int,char,int,char,string> {
public:
	MyMapper(MaPI_MapReduce<int,char,int,char,string> * mr, MaPI_Serializer<int,char,int,char,string> * s)
		:MaPI_Mapper<int,char,int,char,string>(mr,s){};
	virtual multimap<int,char> map( pair<int,char> a ) 
	{ 
		multimap<int,char> m; 
		m.insert(a); 
		cout << "\tMapping..\n"; 
		sleep(1); 
		cout << "\tOk\n"; 
		return m; 
	};
};

class MyReducer : public MaPI_Reducer<int,char,int,char,string> {
public:
	MyReducer(MaPI_MapReduce<int,char,int,char,string> * mr, MaPI_Serializer<int,char,int,char,string> * s)
		:MaPI_Reducer<int,char,int,char,string>(mr,s){};
	virtual pair<int,string> reduce( pair<int, vector<char> > bs ) 
	{
		string reduced;
		for( vector<char>::iterator it = bs.second.begin() ; it != bs.second.end() ; ++it) reduced += *it;
		cout << "\tReducing..\n"; 
		sleep(1); 
		cout << "\tOk\n"; 
		return pair<int,string>(bs.first,reduced); 
	};
};

int main(int argc,char** argv)
{
	cout << "MaPI test" << endl;

	MaPI_MapReduce<int,char,int,char,string> mapReduce;
	MySerializer serializer;
	MyMapper mapper(&mapReduce,&serializer);
	MyReducer reducer(&mapReduce,&serializer);
	mapReduce.initServers(argc,argv);

	multimap<int,char> input;
	input.insert(pair<int,char>(1,'a'));
	input.insert(pair<int,char>(2,'b'));
	input.insert(pair<int,char>(1,'c'));

	map<int,string> output = mapReduce.run(mapper,reducer,input);

	return 0;
}
