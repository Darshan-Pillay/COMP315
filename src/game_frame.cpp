#include "Headers/game_frame.h"
#include <string>
#include <iostream>

GameFrame::GameFrame(std::string gameFrameString) {
	
	frameString = gameFrameString;
	frameControl = new FrameControlHandler();
	
}

void GameFrame::addFrameControl(callback callBackFunction) {
	frameControl->addCallBackFunction(callBackFunction);
}

GameFrame::~GameFrame() {
	delete frameControl;
}

