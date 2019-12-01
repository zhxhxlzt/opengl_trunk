#include "Application.h"
#include "RenderMgr.h"
#include "SceneMgr.h"
#include "GameObject.h"
#include "Scene.h"
#include <queue>
#include <deque>
#include <stack>


using namespace std;
using namespace yk;

template<typename du=nanoseconds>
void showdiff(time_point<system_clock> t1, time_point<system_clock> t2, unsigned int den=10000)
{
	cout << "create time:" << duration_cast<du>(t2 - t1).count() / den << "纳秒" << endl;
}

void testProfile()
{
	auto t1 = chrono::system_clock::now();
	auto gb = GameObject::Create();
	for (int i = 0; i < 10000; i++)
	{
		gb->AddComponent<ModelFilter>();
	}
	gb->AddComponent<MeshFilter>();
	auto t2 = system_clock::now();

	showdiff(t1, t2);

	auto t3 = system_clock::now();
	gb->GetComponent<MeshFilter>();
	auto t4 = system_clock::now();
	showdiff(t3, t4);
}

class A
{
public:
	int val;
	A() {
		val = 20;
	};
	A(A && a)
	{
		val = a.val;
	}
	A(const A& a)
	{
		val = a.val;
	}
	A(int &&c) : val(c) {}
};

void testmap()
{
	map<int, string> mpc;
	mpc[12] = "good";
	cout << mpc[12] << endl;
	mpc.erase(12);

	mpc.emplace(make_pair(12, "gg"));
	mpc.emplace(make_pair(15, "ggq"));
	mpc.emplace(make_pair(12, "gg12"));
	mpc.emplace(make_pair(12, "agg12"));


	mpc.emplace_hint(mpc.end(), make_pair(10, "gd"));
	mpc.emplace(make_pair(0, "gq"));

	vector<A> va;

	vector<vec3> vecs;

	for (int i = 0; i < 10; i++)
	{
		vecs.emplace_back(i, i*i, i*i / 2);
	}

	for (auto& e : vecs)
	{
		cout << e << endl;
	}


	va.emplace_back(12);
	cout << va[0].val << endl;

	for (auto &e : mpc)
	{
		cout << e.first << "--" << e.second << endl;
	}
	
}

void testconstruct()
{
	A a0(10);
	auto a1 = new A(move(a0));

	cout << a1->val << endl;

}

int main()
{
	//testProfile();
	Application app;
	RenderMgr::InitWindow();
	auto scene = SceneMgr::testScene();
	SceneMgr::load(scene);
	//app.StartTimer(&RenderMgr::RenderUpdate);
	app.StartTimer(&SceneMgr::UpdateScene);
	app.run();
	//testconstruct();
	//testmap();
	//getchar();
	return 0;
}