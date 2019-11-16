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
		function<void()> m_callfunc;
		unsigned int m_milliseconds;
	
	public:
		Timer() = default;
		Timer(function<void()> func, unsigned int milliseconds) :
			m_prevPoint(system_clock::now()),
			m_callfunc(func),
			m_milliseconds(milliseconds)
		{
		}
	private:
		friend class Application;
		void Update()
		{
			auto diff = system_clock::now() - m_prevPoint;
			if (m_milliseconds <= duration_cast<milliseconds>(diff).count())
			{
				m_callfunc();
				m_prevPoint = system_clock::now();
			}
		}

	};
}