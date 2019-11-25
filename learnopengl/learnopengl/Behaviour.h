#pragma once
#include "Component.h"

namespace yk
{
	class Behaviour : public Component
	{
		TYPE(yk::Behaviour,Component)

	protected:
		bool m_enabled = true;

	public:
		bool enabled() { return m_enabled; }
		void setEnable(bool state) { m_enabled = state; }
	};
}