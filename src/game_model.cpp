#include "Headers/game_model.h"
#include <iostream>
#include "Headers/game_core.h"

GameModel::GameModel() {
	playerUsername = "guest";
	continueEventListenLoop = false;
	musicToggle = true;
	currentGameFrame = nullptr;
}

void GameModel::listen() {
	int uiOption;
	continueEventListenLoop = true;
	while (continueEventListenLoop) {
		if (currentGameFrame) {
			bool validOption = false;
			while (!validOption) {
				getValidInput(uiOption);
				validOption = currentGameFrame->callCallback(uiOption);
				if (currentGameFrame->getCurrentFrameStatusCode() == TERMINATE)
				{
					continueEventListenLoop = false;
				}
				else
				{
					core::dispatchFrame(currentGameFrame->getCurrentFrameStatusCode(), this);
				}
			}
		}
		else
		{
			continueEventListenLoop = false;
		}
	}
}

void GameModel::setGameFrame(GameFrame* newFrame) {
	if (newFrame) {
		currentGameFrame = newFrame;
	}
}

void GameModel::printModel() {
	if (currentGameFrame) {
		core::clearConsole();
		std::cout << currentGameFrame->getFrameSting();
	}
}

bool GameModel::hasFrame() {
	if (this->currentGameFrame) {
		return true;
	}
	return false;
}

GameModel::~GameModel() {
	if (hasFrame())
	{
		delete currentGameFrame;
	}

}


