#include "test.h"
#include "imgui/imgui.h"

namespace test {

	TestMenu::TestMenu(Test*& currentTestPointer): m_currentTest(currentTestPointer) {}

	
	void TestMenu::onImguiRender() {

		for (auto& test : m_test) {
			if (ImGui::Button(test.first.c_str())) {
				m_currentTest = test.second();
			}
		}
		
	}
}