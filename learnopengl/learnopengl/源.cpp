#include<stdio.h>
#include<vector>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<assimp/camera.h>
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include"Object.h"
#include"Component.h"
#include "GameObject.h"
#include "Transform.h"
using namespace std;
using namespace yk;



int main()
{
	GameObject gb;
	auto tr = gb.AddComponent<Transform>();

	auto tc = gb.GetComponent<Transform>();

	/*cout << GameObject::type_name << endl;
	cout << gb.GetTypeName() << endl;*/

	cout << boolalpha << (tr == tc) << endl;

	gb.ObjectName

	


	getchar();
	return 0;
}