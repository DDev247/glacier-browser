
#pragma once

namespace glacier {

	class Application;

	class DisplayElement {
	public:
		DisplayElement() {};

		virtual void init(Application* app) { m_app = app; };
		virtual void render();

	protected:
		Application* m_app;

	};

}
