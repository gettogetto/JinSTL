#ifndef JIN_PAIR_H_
#define JIN_PAIR_H_
namespace jinstl{
	template<class T1,class T2>
		class pair{
			public:
				pair():first(T1()),second(T2()){}
				pair(const T1& t1,const T2& t2):first(t1),second(t2){}
				pair(const pair<T1,T2>& rhs):first(rhs.first),second(rhs.second){}

				T1 first;
				T2 second;
		};
	template<class T1,class T2>
		inline bool operator == (const pair<T1,T2>& p1,const pair<T1,T2>& p2){
			return p1.first==p2.first&&p1.second==p2.second;
		}
	template<class T1,class T2>
		inline bool operator < (const pair<T1,T2>& p1,const pair<T1,T2>& p2){
			if(p1.first < p2.first) return true;
			else if(p1.first==p2.first) return p1.second<p2.second;
			else return false;
		}
	template<class T1,class T2>
		inline pair<T1,T2> make_pair(const T1& t1,const T2& t2){
			return pair<T1,T2>(t1,t2);
		}

}//namespace jinstl
#endif//JIN_PAIR_H_
