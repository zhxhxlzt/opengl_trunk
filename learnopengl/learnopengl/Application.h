#pragma once
#include "Object.h"
#include "Timer.h"
#include "GameObject.h"
#include "Window.h"

namespace yk
{
	// 提供事件循环
	class Application : public Object
	{
		TYPE(yk::Application, yk::Object);
	private:
		std::vector<std::shared_ptr<Timer>> m_timers;
		vector<shared_ptr<Timer>> m_expiredTimers;

	public:
		std::shared_ptr<Timer> CreateTimer(int milliseconds)
		{
			return std::make_shared<Timer>();
		}

		void AddTimer(std::shared_ptr<Timer> timer)
		{
			m_timers.push_back(timer);
		}

		shared_ptr<Timer> StartTimer(function<void(Timer&)> callfunc, unsigned int milliseconds=1000/60)
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
			while (!shutdown)
			{
				for (auto &timer : m_timers)
				{
					if (!timer->dead and timer->enabled)
						timer->Update();
				}
			}
		}

		static bool shutdown;
	};

	bool Application::shutdown = false;
}