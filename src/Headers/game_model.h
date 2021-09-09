#pragma once
#include <string>
#include "Headers/game_frame.h"

class GameModel {
public:
	GameModel();
	void listen();
	void setGameFrame(GameFrame* newFrame);
	bool hasFrame();
	void deleteCurrentFrame() {
		if (hasFrame()) {
			delete currentGameFrame;
			currentGameFrame = nullptr;
		}
	}
	void printModel();
	std::string playerUsername;
	bool musicToggle;
	~GameModel();
private:
	GameFrame* currentGameFrame;
	bool continueEventListenLoop;
};

