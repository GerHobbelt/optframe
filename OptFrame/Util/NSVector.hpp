/*
Author: Sabir Ribas
Date: 2010-03-10
Change Log:
	2010-03-10	NSVector idealization and development of safe insertion, remotion and Shiftk neighborhood structure for one or more vectors (e.g. vector<T> or vector< vector<T> >).
	2010-03-16	Development of Swapk neighborhood structure for template vector of vector.
*/

#ifndef NSVECTOR_HPP
#define NSVECTOR_HPP

#include <vector>
#include <cstdio>
#include <iostream>
#include <cmath>

using namespace std;

template <class T>
class NSVector
{
public:

	/* safe insertion and remotion - 2010-03-10 */

	static bool insert(vector<T> & v, unsigned int rank, T o)
	{
		if (rank < 0 || rank > v.size()) { cerr << "Warning [NSVector]: out of range insertion" << endl; return false; }
		v.insert(v.begin()+rank,o);
		return true;
	};

	static bool remove(vector<T> & v, unsigned int rank)
	{
		if (rank < 0 || rank >= v.size()) { cerr << "Warning [NSVector]: out of range remotion" << endl; return false; }
		v.erase(v.begin()+rank);
		return true;
	};

	/* Shiftk - 2010-03-10 */

	static bool shift1_apply(vector<T> & v, unsigned int rank1, unsigned int rank2)
	{
		if (rank1 < 0 || rank1 >= v.size() || rank2 < 0 || rank2 >= v.size()) 
		{ cerr << "Warning [NSVector]: out of range shift1" << endl; return false; }
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
		if(rank1 < rank2)
			for(int i = rank1; i < rank2; i++)
			{
				aux = v[i];
				v[i] = v[i+1];
				v[i+1] = aux;
			}
		else
			for(int i = rank1; i > rank2; i--)
			{
				aux = v[i];
				v[i] = v[i-1];
				v[i-1] = aux;
			}		
		return true;
	};

	static bool shiftk_apply(vector<T> & v, unsigned int k, unsigned int rank1, unsigned int rank2)
	{
		if (rank1 < 0 || rank1 + k > v.size() || rank2 < 0 || rank2 + k > v.size()) 
		{ cerr << "Warning [NSVector]: out of range shiftk" << endl; return false; }
		if (rank1 < rank2) for (int i = k-1; i+1 > 0; i--) shift1_apply(v,rank1+i,rank2+i); else
		if (rank1 > rank2) for (int i = 0; i < k; i++) shift1_apply(v,rank1+i,rank2+i);
		return true;
	};

	static pair<int,pair<int,int> > shiftk_apply(vector<T> & v, pair<int,pair<int,int> > & m)
	{
		int k = m.first, rank1 = m.second.first, rank2 = m.second.second;
		if (shiftk_apply(v,k,rank1,rank2)) return pair<int,pair<int,int> >(k, pair<int,int>(rank2,rank1));
	};

	static int shift1_size(vector<T> &v) { return v.size()*v.size(); };

	static pair<int,pair<int,int> > shiftk_move(vector<T> &v, int k, int i) 
	{
		return pair<int,pair<int,int> >(k, pair<int,int>((i/v.size()), (i%v.size())));
	};

	static bool shiftk_canBeApplied(vector<T> & v, pair<int,pair<int,int> > & m)
	{
		int k = m.first, rank1 = m.second.first, rank2 = m.second.second;
		return !(rank1 < 0 || rank1 + k > v.size() || rank2 < 0 || rank2 + k > v.size() || rank1 == rank2);
	};

	static vector< pair<int,pair<int,int> > > * shiftk_appliableMoves(vector<T> & v,int k)
	{
		vector< pair<int,pair<int,int> > > * moves = new vector< pair<int,pair<int,int> > >;
		for (int i = 0 ; i < NSVector<int>::shift1_size(v) ; i++)
		{
			pair<int,pair<int,int> > m = NSVector<int>::shiftk_move(v,k,i);
			if (NSVector<int>::shiftk_canBeApplied(v,m)) moves->push_back(m);
		}
		return moves;
	};

	/* Shiftk with Two Vectors - 2010-03-10 */

	static bool shift1_apply(vector<T> & v1, vector<T> & v2, unsigned int rank1, unsigned int rank2)
	{
		if (rank1 < 0 || rank1 >= v1.size() || rank2 < 0 || rank2 > v2.size()) 
		{ cerr << "Warning [NSVector]: out of range shift1 (v1,v2)" << endl; return false; }
		insert(v2,rank2,v1[rank1]);
		remove(v1,rank1);
		return true;
	};

	static bool shiftk_apply(vector<T> & v1, vector<T> & v2, unsigned int k, unsigned int rank1, unsigned int rank2)
	{
		if (rank1 < 0 || rank1 + k > v1.size() || rank2 < 0 || rank2 > v2.size()) 
		{ cerr << "Warning [NSVector]: out of range shiftk (v1,v2)" << endl; return false; }
		for (int i = k-1; i+1 > 0; i--) shift1_apply(v1,v2,rank1+i,rank2);
		return true;
	};

	static pair<int,pair<int,int> > shiftk_apply(vector<T> & v1, vector<T> & v2, pair<int,pair<int,int> > & m)
	{
		int k = m.first, rank1 = m.second.first, rank2 = m.second.second;
		if (shiftk_apply(v1,v2,k,rank1,rank2)) return pair<int,pair<int,int> >(k, pair<int,int>(rank2,rank1));
	};

	static int shift1_size(vector<T> &v1,vector<T> &v2) { return v1.size()*(v2.size()+1); };

	static pair<int,pair<int,int> > shiftk_move(vector<T> &v1,vector<T> &v2, int k, int i) 
	{
		return pair<int,pair<int,int> >(k, pair<int,int>((i/(v2.size()+1)), (i%(v2.size()+1))));
	};

	static bool shiftk_canBeApplied(vector<T> & v1, vector<T> & v2, pair<int,pair<int,int> > & m)
	{
		int k = m.first, rank1 = m.second.first, rank2 = m.second.second;
		return !(rank1 < 0 || rank1 + k > v1.size() || rank2 < 0 || rank2 > v2.size());
	};

	static vector< pair<int,pair<int,int> > > * shiftk_appliableMoves(vector<T> & v1, vector<T> & v2, int k)
	{
		vector< pair<int,pair<int,int> > > * moves = new vector< pair<int,pair<int,int> > >;
		for (int i = 0 ; i < NSVector<int>::shift1_size(v1,v2) ; i++)
		{
			pair<int,pair<int,int> > m = NSVector<int>::shiftk_move(v1,v2,k,i);
			if (NSVector<int>::shiftk_canBeApplied(v1,v2,m)) moves->push_back(m);
		}
		return moves;
	};

	static bool shiftk_canBeApplied(vector< vector<T> > & v, pair<int, pair< pair<int,int>, pair<int,int> > > & m)
	{
		int k = m.first, a = m.second.first.first, rank1 = m.second.first.second, b = m.second.second.first, rank2 = m.second.second.second;
		return !(rank1 < 0 || rank1 + k > v[a].size() || rank2 < 0 || rank2 > v[b].size() || a==b);
	};

	static vector< pair<int, pair< pair<int,int>,pair<int,int> > > > * shiftk_appliableMoves(vector< vector<T> > & v, int k)
	{
		vector< pair<int,pair< pair<int,int>, pair<int,int> > > > * moves = new vector< pair<int,pair< pair<int,int>, pair<int,int> > > >;

		for (int a = 0 ; a < v.size() ; a++)
		{
			for (int b = 0 ; b < v.size() ; b++)
			{
				if (a==b) continue;

				int size = NSVector<int>::shift1_size(v[a],v[b]);

				for (int i = 0 ; i < size ; i++)
				{
					pair<int, pair<int,int> > mLinha = NSVector<int>::shiftk_move(v[a],v[b],k,i);
					pair<int,pair< pair<int,int>,pair<int,int> > > m;
					m.first = mLinha.first;
					m.second.first.first = a;
					m.second.first.second = mLinha.second.first;
					m.second.second.first = b;
					m.second.second.second = mLinha.second.second;
					if (NSVector<int>::shiftk_canBeApplied(v,m)) moves->push_back(m);
				}				
			}
		}

		return moves;
	};

	static pair<int,pair < pair<int,int>,pair<int,int> > >
	shiftk_apply(vector< vector<T> > & v, pair<int,pair < pair<int,int>,pair<int,int> > > & m)
	{
		int k = m.first, a = m.second.first.first, rank1 = m.second.first.second, b = m.second.second.first, rank2 = m.second.second.second;
		if (shiftk_apply(v[a],v[b],k,rank1,rank2)) 
			return pair<int,pair < pair<int,int>,pair<int,int> > >(k, 
				pair< pair<int,int> ,pair<int,int> > ( pair<int,int>(b,rank2) , pair<int,int>(a,rank1) ) );
	};

	/* Swapk */

	//...

	/* Swapk with Two Vectors - 2010-03-16 */

	static bool swap1_apply(vector<T> & v1, vector<T> & v2, unsigned int rank1, unsigned int rank2)
	{
		if (rank1 < 0 || rank1 >= v1.size() || rank2 < 0 || rank2 >= v2.size()) 
		{ cerr << "Warning [NSVector]: out of range swap1 (v1,v2)" << endl; return false; }
		T aux = v1[rank1];
		v1[rank1] = v2[rank2];
		v2[rank2] = aux;
		return true;
	};

	static bool swapk_apply(vector<T> & v1, vector<T> & v2, unsigned int k1, unsigned int k2, unsigned int rank1, unsigned int rank2)
	{
		if (rank1 < 0 || rank1 + k1 > v1.size() || rank2 < 0 || rank2 + k2 > v2.size()) 
		{ cerr << "Warning [NSVector]: out of range swapk (v1,v2)" << endl; return false; }
		if (k1 == k2) for (int i = 0; i < k1; i++) swap1_apply(v1,v2,rank1+i,rank2+i); else
		if (k1 > k2) {
			for (int i = 0; i < k1 - k2; i++) swap1_apply(v1,v2,rank1+i,rank2+i);
			shiftk_apply(v1,v2,k1-k2,rank1+k2,rank2+k2);
		} else
		if (k1 < k2) { 
			for (int i = 0; i < k2 - k1; i++) swap1_apply(v1,v2,rank1+i,rank2+i);
			shiftk_apply(v2,v1,k2-k1,rank2+k1,rank1+k1);
		}
		return true;
	};

	static pair< pair<int,int> , pair<int,int> > shiftk_apply(vector<T> & v1, vector<T> & v2, pair< pair<int,int> , pair<int,int> > & m)
	{
		int k1 = m.first.first, k2 = m.first.second, rank1 = m.second.first, rank2 = m.second.second;
		if (swapk_apply(v1,v2,k1,k2,rank1,rank2)) return pair< pair<int,int> , pair<int,int> >( pair<int,int>(k2,k1) , pair<int,int>(rank2,rank1));
	}; // não testado

	static int swap1_size(vector<T> &v1,vector<T> &v2) { return v1.size()*v2.size(); }; // não testado

	static pair< pair<int,int>,pair<int,int> > swapk_move(vector<T> &v1,vector<T> &v2, int k1, int k2, int i) 
	{
		return pair< pair<int,int>,pair<int,int> >( pair<int,int>(k1,k2), pair<int,int>( i/v2.size() , i%v2.size() ) );
	}; // não testado

	static bool swapk_canBeApplied(vector<T> & v1, vector<T> & v2, pair< pair<int,int>,pair<int,int> > & m)
	{
		int k1 = m.first.first, k2 = m.first.second, rank1 = m.second.first, rank2 = m.second.second;
		return !(rank1 < 0 || rank1 + k1 > v1.size() || rank2 < 0 || rank2 + k2 > v2.size());
	}; // não testado

	static vector< pair< pair<int,int>,pair<int,int> > > * swapk_appliableMoves(vector<T> & v1, vector<T> & v2, int k1, int k2)
	{
		vector< pair< pair<int,int>,pair<int,int> > > * moves = new vector< pair< pair<int,int>,pair<int,int> > >;
		for (int i = 0 ; i < NSVector<int>::swap1_size(v1,v2) ; i++)
		{
			pair< pair<int,int>,pair<int,int> > m = NSVector<int>::swapk_move(v1,v2,k1,k2,i);
			if (NSVector<int>::swapk_canBeApplied(v1,v2,m)) moves->push_back(m);
		}
		return moves;
	}; // não testado

	static bool swapk_canBeApplied(vector< vector<T> > & v, pair< pair<int,int>, pair< pair<int,int>, pair<int,int> > > & m)
	{
		int k1 = m.first.first, k2 = m.first.second, 
			a  = m.second.first.first, rank1 = m.second.first.second, b = m.second.second.first, rank2 = m.second.second.second;
		return !(rank1 < 0 || rank1 + k1 > v[a].size() || rank2 < 0 || rank2 + k2 > v[b].size() || a==b);
	}; // não testado

	static vector< pair< pair<int,int> , pair< pair<int,int>,pair<int,int> > > > * swapk_appliableMoves(vector< vector<T> > & v, int k1, int k2)
	{
		vector< pair< pair<int,int> , pair< pair<int,int>,pair<int,int> > > > * moves = 
			new vector< pair< pair<int,int> , pair< pair<int,int>,pair<int,int> > > >;

		for (int a = 0 ; a < v.size() ; a++)
		{
			//for (int b = 0 ; b < v.size() ; b++)
			for (int b = a+1 ; b < v.size() ; b++)
			{
				//if (a==b) continue;

				int size = NSVector<int>::swap1_size(v[a],v[b]);

				for (int i = 0 ; i < size ; i++)
				{
					pair< pair<int,int> , pair<int,int> > mLinha = NSVector<int>::swapk_move(v[a],v[b],k1,k2,i);
					pair< pair<int,int> , pair< pair<int,int>,pair<int,int> > > m;
					m.first = mLinha.first; // pair
					m.second.first.first = a;
					m.second.first.second = mLinha.second.first;
					m.second.second.first = b;
					m.second.second.second = mLinha.second.second;
					if (NSVector<int>::swapk_canBeApplied(v,m)) moves->push_back(m);
				}				
			}
		}

		return moves;
	}; // não testado

	static pair< pair<int,int> , pair < pair<int,int>,pair<int,int> > >
	swapk_apply(vector< vector<T> > & v, pair< pair<int,int> , pair < pair<int,int>,pair<int,int> > > & m)
	{
		int k1 = m.first.first, k2 = m.first.second, 
			a  = m.second.first.first, rank1 = m.second.first.second, b = m.second.second.first, rank2 = m.second.second.second;
		if (swapk_apply(v[a],v[b],k1,k2,rank1,rank2)) 
			return pair< pair<int,int> ,pair < pair<int,int>,pair<int,int> > >( pair<int,int>(k2,k1) , 
				pair< pair<int,int>,pair<int,int> > ( pair<int,int>(b,rank2) , pair<int,int>(a,rank1) ) );
	}; // não testado

};

#endif /* NSVECTOR_HPP */
