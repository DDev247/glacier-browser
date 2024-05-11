
#pragma once

#include "display_element.h"
#include "button_element.h"
#include "input_element.h"
#include "texture.h"
#include <string>
#include <list>

namespace glacier {

	class NavigationElement : public DisplayElement {
	public:
		void init(Application* app) override;
		void render() override;

		std::string m_address;
		std::list<std::string> m_history;

	private:
		ButtonElement m_backButton;
		
		bool m_drawFwd;
		ButtonElement m_fwdButton;

		ButtonElement m_refreshButton;

		InputElement m_addressBar;

		ButtonElement m_homeButton;
		ButtonElement m_downloadsButton;
		ButtonElement m_menuButton;
	};

}
