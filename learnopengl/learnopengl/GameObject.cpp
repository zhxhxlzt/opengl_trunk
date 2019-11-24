#include "GameObject.h"
#include "SceneMgr.h"
#include "Camera.h"
using namespace yk;

#include "ModelFilter.h"
#include "ModelRenderer.h"

//template<>
//std::shared_ptr<Camera> GameObject::AddComponent()
//{
//	auto cam_comp = _AddComponent<Camera>(falseType);
//	if (not SceneMgr::currentScene()->mainCamera())
//	{
//		SceneMgr::currentScene()->SetMainCamera(cam_comp);
//	}
//	return cam_comp;
//}

//shared_ptr<GameObject> GameObject::Create()
//{
//	GameObject* gb_ptr = new GameObject;
//	shared_ptr<GameObject> gb(gb_ptr);
//	gb->m_this = gb;
//
//	auto tr = make_shared<Transform>();
//	tr->attachToGameObject(gb);
//
//	gb->m_comps.push_back(tr);
//	gb->m_transform = tr;
//	return gb;
//}