#pragma once
#include "Component.h"

namespace yk
{
	class Behaviour : public Component
	{
		TYPE(yk::Behaviour, yk::Component)

	protected:
		bool m_enabled = true;

	public:

		virtual void Awake() {}

		virtual void Start() {}

		virtual void Update() {}

		virtual void OnDestroy() {}

		bool enabled() { return m_enabled; }
		void setEnable(bool state) { m_enabled = state; }
	};
}