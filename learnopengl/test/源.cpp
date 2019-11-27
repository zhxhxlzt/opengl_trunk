#include <memory>
#include <iostream>
#include <functional>
#include <vector>

using namespace std;



#define BRACKET_L() (
#define BRACKET_R() )
#define PP_NARG(...) \
PP_NARG_ ( __VA_ARGS__, PP_RSEQ_N() )
#define PP_NARG_(...) \
PP_ARG_N BRACKET_L() __VA_ARGS__ BRACKET_R()
#define PP_ARG_N( \
_1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
_11,_12,_13,_14,_15,_16, N,...) N
#define PP_RSEQ_N() \
16,15,14,13,12,11,10, \
9,8,7,6,5,4,3,2,1,0

#define TOSTRING(x) #x

template<typename T>
struct G
{
	using type = T;
	static constexpr char name[] = TOSTRING(T);
};

#define B_L() (
#define B_R() )
#define REMOVE_NAMESPACE(T) G<T>::type

class A
{
private:
	int v = 20;

public:
	int * getV()
	{
		return &v;
	}

	void show()
	{
		cout << "v:" << v << endl;
	}
};


int main()
{
	vector<int> c = { 1, 2, 3 };

	auto p = &c[1];

	delete p;

	c[0] = 20;

	for (auto& e : c)
	{
		cout << "ok" << endl;
		cout << e << endl;
	}
	getchar();




	return 0;
}