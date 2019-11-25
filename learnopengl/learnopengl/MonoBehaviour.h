#pragma once
#include "Behaviour.h"
namespace yk
{
	class MonoBehaviour : public Behaviour
	{
		TYPE(yk::MonoBehaviour, Behaviour);
	public:
		virtual void Awake() {}

		virtual void Start() {}

		virtual void Update() {}

		virtual void LateUpdate() {}

		virtual void OnDestroy() {}
	};
}