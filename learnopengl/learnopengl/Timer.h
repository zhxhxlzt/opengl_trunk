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

		unsigned int m_milliseconds;	// 允许调用者自行修改下次调用的时间间隔
		bool m_oneshot = false;			// 只调用一次，然后disable
		bool enabled = true;			
		bool dead = false;				// 如果为true，则此定时器会被移除

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