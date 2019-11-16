#pragma once
#include "Object.h"


namespace yk
{
	using namespace std::chrono;
	class Timer : Object
	{
		TYPE(yk::Timer, yk::Object);

	private:
		time_point<system_clock> m_prevPoint;
		function<void(Timer &timer)> m_callfunc;
		
	public:
		Timer() = default;
		Timer(function<void(Timer &timer)> func, unsigned int milliseconds) :
			m_prevPoint(system_clock::now()),
			m_callfunc(func),
			m_milliseconds(milliseconds)
		{
		}

		unsigned int m_milliseconds;	// ��������������޸��´ε��õ�ʱ����
		bool m_oneshot = false;			// ֻ����һ�Σ�Ȼ��disable
		bool enabled = true;			
		bool dead = false;				// ���Ϊtrue����˶�ʱ���ᱻ�Ƴ�

	private:
		friend class Application;
		void Update()
		{
			auto diff = system_clock::now() - m_prevPoint;
			if (m_milliseconds <= duration_cast<milliseconds>(diff).count())
			{
				if (m_oneshot) enabled = false;
				m_callfunc(*this);
				m_prevPoint = system_clock::now();
			}
		}

	};
}