#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <functional>

namespace test {

	class Test {

	public:
		Test(){}
		virtual ~Test(){}

		virtual void onUpdate(float detltaTime){}
		virtual void onRender(){}
		virtual void onImguiRender(){}
	};

	class TestMenu : public Test {

	public:
		TestMenu(Test*& currentTestPointer);
		

		void onImguiRender() override;

		template<typename T>
		void registerTest(const std::string& name) {

			std::cout << "registering test: " << name << std::endl;

		//	m_test.push_back(std::make_pair(name, []() ->T* {return new T(); }));

	//		auto myLambda = []() {return T(); };
	//		m_test.push_back(std::make_pair(name, myLambda));

		   std::function<Test * ()> myLambda = []() { return new T(); };

			m_test.push_back(std::make_pair(name, myLambda));
		}

	private:
		Test*& m_currentTest;
	
		std::vector<std::pair<std::string, std::function<Test*()>>> m_test;

	};
}