#pragma once

#include "MonoBehaviour.h"

namespace yk
{
	class RenderCom : public MonoBehaviour
	{
	public:
		virtual void Update()
		{
			cout << "render com update" << endl;
		}
	};
}

