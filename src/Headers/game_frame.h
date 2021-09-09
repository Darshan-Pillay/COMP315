#pragma once
#include <string>
#include<iostream>
#include <functional>
#include "Headers/frame_control.h"

class GameFrame {
public:
	
	GameFrame(std::string gameFrameString);
	bool callCallback(controlID controlNumber) { return frameControl->callControlCallback(controlNumber); }
	void addFrameControl(callback callBackFunction);
	Callback_Return_Code getCurrentFrameStatusCode() {
		if (frameControl) {
			return frameControl->getCallBackReturnCode();
		}
		return TERMINATE;
	}
	std::string getFrameSting() { return frameString; }
	~GameFrame();
private:
	std::string frameString;
	FrameControlHandler* frameControl; 
	
};

/*
Template functtion which Checks if current input in the standard (cin) input stream
is of type E and puts that value into the reference variable e of Type E. Otherwise
we display an error message, clear the input stream and ignore all invalid input 
entered previously
*/

template<class E> static void getValidInput(E& e) {
	while (!(std::cin >> e)) {
		std::cerr << "Error: Incorrect input type used. Expected " << typeid(e).name() << std::endl;
		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');
	}
}