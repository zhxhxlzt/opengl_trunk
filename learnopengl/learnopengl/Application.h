#pragma once
#include "Object.h"
#include "Timer.h"


namespace yk
{
	class Application : public Object
	{
		TYPE(Application);
	private:
		std::vector<std::shared_ptr<Timer>> m_timers;

	public:
		std::shared_ptr<Timer> CreateTimer(int milliseconds)
		{
			return std::make_shared<Timer>();
		}

		void AddTimer(std::shared_ptr<Timer> timer)
		{
			m_timers.push_back(timer);
		}

		shared_ptr<Timer> StartTimer(function<void()> callfunc, unsigned int milliseconds)
		{
			auto timer = new Timer(callfunc, milliseconds);
			shared_ptr<Timer> ptr(timer);
			m_timers.push_back(ptr);
			return ptr;
		}

		void StopTimer(shared_ptr<Timer> timer)
		{
			auto res = find(m_timers.begin(), m_timers.end(), timer);
			if (res != m_timers.end())
			{
				m_timers.erase(res);
			}
		}

		void run()
		{
			while (true)
			{
				for (auto &timer : m_timers)
				{
					timer->Update();
				}
			}
		}
	};
}