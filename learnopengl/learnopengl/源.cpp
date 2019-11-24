#include "Application.h"
#include "RenderMgr.h"
#include "SceneMgr.h"
#include "GameObject.h"
#include "Scene.h"

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

int main()
{
	//testProfile();
	Application app;
	RenderMgr::InitWindow();
	SceneMgr::testScene();
	app.StartTimer(&RenderMgr::RenderUpdate);
	app.run();
	//getchar();
	return 0;
}