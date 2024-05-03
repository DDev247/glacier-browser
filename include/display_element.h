
#pragma once
#include "../include/application.h"

namespace glacier {

	class DisplayElement {
	public:
		DisplayElement(Application app);

		virtual void render();

	private:
		Application m_app;

	};

}
