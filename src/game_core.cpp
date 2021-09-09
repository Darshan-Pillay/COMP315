
#include <string>
#include <thread>
#include <chrono>
#include "Headers/game_strings.h"
#include "Headers/sqlitewrapper.h"
#include "Headers/game_core.h"
#include "bcrypt/BCrypt.hpp"
#include "Headers/sqlitewrapper.h"
#include <vector>
#include "Headers/game_model.h"
#include <wchar.h>
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <random>
#include <queue>
#include <unordered_set>
#include <string>
#include "shuffler.cpp"

core::LoadingBar::LoadingBar(Length length) {
	this->loadingBarWidth = length;

}

int core::LoadingBar::playLoadingAnimation(AnimationSpeed speed, FrameID frameID) {
	if (checkIfANSIEscapeSequenceSupported())
	{	
		showConsoleCursor(false);
		std::string loadingBar("[");

		for (size_t i = 0; i < this->loadingBarWidth; i++)
		{
			loadingBar = loadingBar + " ";
		}
		loadingBar = loadingBar + "]";
		
		std::cout << "\033[s";
		std::cout << loadingBar;
		for (size_t i = 0; i < this->loadingBarWidth; i++)
		{	
			std::this_thread::sleep_for(std::chrono::milliseconds(speed));
			std::cout << "\033[u";
			replaceCharInString(loadingBar, '>', i + 1);
			for (size_t k = 0; k < loadingBar.size()+2; k++)
			{
				std::cout << " ";
			}
			std::cout << "\033[u";
			std::cout << loadingBar;
		}

	
	}
	return frameID;
}

void core::clearConsole() {
//clears a windows console
#ifdef _WIN32
	system("cls");
#endif // WIN_32

//clears a linux console
#ifdef Linux
	system("clear");
#endif
}

void core::autoTypeString(const std::string& text, AnimationSpeed charPerMillisecond) {
	for (int charPosition = 0; charPosition < text.length(); charPosition++) {
		std::cout << text[charPosition];
		std::this_thread::sleep_for(std::chrono::milliseconds(charPerMillisecond));
	}
}

Callback_Return_Code core::runLogInProcedure(GameModel* gameModel) { 
	clearConsole();
	std::string loginBanner = 
		"+=========================================================+\n"
		"\|                       Login                             \|\n"
		"\|=========================================================\|\n";

	bool validPlayerInformation = false;
	std::string username("guest");
	std::string userPassword("guest");
	while (!validPlayerInformation)
	{
		std::cout << loginBanner << "\n";
		std::cout << "Username (no spaces):";
		getValidInput<std::string>(username);
		std::cout << std::endl;
		std::cout << "Password (no spaces):";
		getValidInput(userPassword);
		std::string* usernamePtr = &username;
		std::string* userPasswordPtr = &userPassword;

		if (doesUsernameExist(usernamePtr) && verifyPassword(usernamePtr, userPasswordPtr)) {
			validPlayerInformation = true;
		}
		else {
			std::cout << "\n";
			std::cout << "Invalid login details. Please try login again." << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(3));
			clearConsole();
		}
		
	}
	if (gameModel)
	{
		gameModel->playerUsername = username;
	}
	return GameSelectionMenu; }

Callback_Return_Code core::runSignUpProcedure(GameModel* gameModel) { 
	clearConsole();
	std::string signinBanner = 
		"+=========================================================+\n"
		"\|                       Sign-Up                           \|\n"
		"\|=========================================================\|\n";

	bool validPlayerInformation = false;
	std::string username("guest");
	std::string userPassword("guest");
	std::string passwordValidationString("guest");
	std::cout << signinBanner;
	std::cout << "\n";
	std::cout << "Username (no spaces):";
	while (!validPlayerInformation) {
		getValidInput<std::string>(username);
		std::string* temp = new std::string(username);
		if (doesUsernameExist(temp)) {
			std::cout << std::endl;
			std::cout << "The username " + username << " is already taken.";
			std::cout << " Please choose another username.";
			delete temp;
			std::this_thread::sleep_for(std::chrono::seconds(3));
			clearConsole();
			std::cout << signinBanner;
			std::cout << "\n";
			std::cout << "Username (no spaces):";
		}
		else
		{
			validPlayerInformation = true;
			delete temp;
		}

	}

	validPlayerInformation = false;
	std::cout << "\n";
	std::cout << "Password (no spaces):";
	while(!validPlayerInformation)
	{
		getValidInput(userPassword);
		std::cout << "\n";
		std::cout << "Re-enter password (no spaces):";
		getValidInput(passwordValidationString);
		if (userPassword.compare(passwordValidationString) == 0) {
			validPlayerInformation = true;
		}
		else {
			std::cout << "\n";
			std::cout << "Passwords don't match. Please re-enter your desired password.";
			std::this_thread::sleep_for(std::chrono::seconds(3));
			clearConsole();
			std::cout << signinBanner;
			std::cout << "\n";
			std::cout << "Username:" << username << std::endl;
			std::cout << "\n";
			std::cout << "Password (no spaces):";
		}
	}

	std::string* usernamePtr = &username;
	std::string* passwordPtr = &userPassword;
	core::addNewPlayer(usernamePtr, passwordPtr);
	
	if (gameModel)
	{
		gameModel->playerUsername = username;
	}
	return GameSelectionMenu; }

Callback_Return_Code core::runQuiz(core::GameDifficulty difficulty, core::GameMode gameMode, GameModel* gameModel) {
	if (gameModel)
	{	
		Callback_Return_Code frameCode;
		bool playAgain = true;
		int timeLimit;
		int pointMultiplier;
		int numberOfCorrectQuestions;
		std::string quizBannerTitle;
		std::vector<std::pair<core::Question, int>> questionSet;
		while (playAgain)
		{
			numberOfCorrectQuestions = 0;
			switch (difficulty)
			{
			case Easy: questionSet = getQuestionSet(10, gameMode);
				difficulty = Easy;
				timeLimit = 60;
				quizBannerTitle = generateQuizBannerTitle(gameMode, Easy);
				pointMultiplier = 1;
				break;
			case Medium: questionSet = getQuestionSet(20, gameMode);
				difficulty = Medium;
				timeLimit = 35;
				quizBannerTitle = generateQuizBannerTitle(gameMode, Medium);
				pointMultiplier = 3;
				break;
			case Hard: questionSet = getQuestionSet(30, gameMode);
				difficulty = Hard;
				timeLimit = 15;
				quizBannerTitle = generateQuizBannerTitle(gameMode, Hard);
				pointMultiplier = 7;
				break;
			default:
				difficulty = Easy;
				questionSet = getQuestionSet(10, gameMode);
				quizBannerTitle = generateQuizBannerTitle(gameMode, Easy);
				timeLimit = 60;
				pointMultiplier = 1;
				break;
			}
			Shuffler<std::pair<core::Question, int>> shuffledQuestionSet(questionSet);
			shuffledQuestionSet.shuffle();
			int currentQuestionNumber = 1;
			std::chrono::steady_clock::time_point timerEnd = std::chrono::steady_clock::now() + std::chrono::seconds(timeLimit);
			do 
			{
				core::Question& currentQuestion = shuffledQuestionSet.vectorContainer[((int)currentQuestionNumber)-1].first;
				clearConsole();
				std::cout << quizBannerTitle << "\n";
				std::cout << "Question " << currentQuestionNumber << " of " << shuffledQuestionSet.vectorContainer.size() << std::endl;
				std::cout << "Score: " << numberOfCorrectQuestions * pointMultiplier << "/" << shuffledQuestionSet.vectorContainer.size() * pointMultiplier << std::endl;
				std::cout << currentQuestion << "\n";
				std::cout << "Enter your choice... ";
				int userChoice;
				bool validChoice = true;
				getValidInput(userChoice);

				if (userChoice < 1 || userChoice > 5)
				{
					validChoice = false;
				}

				if (currentQuestion.possibleAnswerThree.length() == 0 && userChoice >= 3)
				{
					validChoice = false;
				}

				std::string correctAnswer;
				switch (shuffledQuestionSet.vectorContainer[((int)currentQuestionNumber) - 1].second)
				{
				case 1:correctAnswer = currentQuestion.possibleAnswerOne;
					break;
				case 2:correctAnswer = currentQuestion.possibleAnswerTwo;
					break;
				case 3:correctAnswer = currentQuestion.possibleAnswerThree;
					break;
				case 4:correctAnswer = currentQuestion.possibleAnswerFour;
					break;
				case 5:correctAnswer = currentQuestion.possibleAnswerFive;
					break;

				}
				if (validChoice)
				{
					if (userChoice == shuffledQuestionSet.vectorContainer[(int)(currentQuestionNumber-1)].second)
					{
						std::cout << "\n";
						std::cout << "You are correct! The correct answer is " << correctAnswer;
						numberOfCorrectQuestions = numberOfCorrectQuestions + 1;
					}
					else
					{
						std::cout << "\n";
						std::cout << "You are incorrect! The correct answer is " << correctAnswer;
					}
				}
				else {
					std::cout << "\n";
					std::cout << "Incorrect Option!" << " You have lost points." << " The correct answer is " << correctAnswer << std::endl;
				}
				std::this_thread::sleep_for(std::chrono::seconds(2));
				timerEnd = timerEnd + std::chrono::seconds(2);
				currentQuestionNumber = currentQuestionNumber + 1;
			} while (std::chrono::steady_clock::now() < timerEnd && currentQuestionNumber != shuffledQuestionSet.vectorContainer.size() + 1);
			clearConsole();
			if (currentQuestionNumber > shuffledQuestionSet.vectorContainer.size())
			{
				std::cout << "Well done you've managed to answer all questions!" << "\n\n";
				
			}
			else
			{
				std::cout << "Your time is up!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << "\n\n";
			}

			std::cout << "Please wait while we prepare your quiz summary." << "\n\n";
			std::cout << "Generating Quiz Session Summary... ";
			LoadingBar test(20);
			test.playLoadingAnimation(150, 0);
			core::clearCurrentConsoleLine(true, 0);
			std::cout << "Press any key to continue...";
			bool keyPressed = false;
			while (!keyPressed) {
				if (_kbhit()) {
					keyPressed = true;
					core::clearConsole();
				}
			}

			updatePlayerPoints(numberOfCorrectQuestions* pointMultiplier, gameModel, gameMode);
			std::cout << quizBannerTitle << std::endl << "\n";
			std::cout << "Quiz Summary" << std::endl;
			std::cout << "===========================================================" << std::endl;
			std::cout << "Total Questions Answered Correctly: " << numberOfCorrectQuestions << "/" << shuffledQuestionSet.vectorContainer.size() << std::endl;
			std::cout << "Total accumulated points: " << numberOfCorrectQuestions * pointMultiplier << std::endl;
			std::cout << "===========================================================" << std::endl;

			std::cout << "\n";
			std::cout << "[0] - Play Again (Next Level)" << std::endl;
			std::cout << "[1] - See Leaderboards" << std::endl;
			std::cout << "[2] - Return to Game Selection Menu" << std::endl;
			std::cout << "\n";
			std::cout << "Enter your choice...";
			
			int selectedOption;
			bool validChoice = false;
			while (!validChoice)
			{
				getValidInput(selectedOption);

				if (!(selectedOption < 0 || selectedOption > 2))
				{
					if (selectedOption == 0) {
						if (difficulty < Hard)
						{
							difficulty = difficulty + 1;
						}
					}
					else if (selectedOption == 1)
					{
						frameCode = FrogsAndFliesLeaderboardScreen;
						playAgain = false;
					}
					else
					{
						frameCode = GameSelectionMenu;
						playAgain = false;
					}
					validChoice = true;
				}
			}
		}
		return frameCode;
	}
	else
	{
		return CONTINUE;
	}
}

void core::runGameIntroAnimation() {
	std::cout << splashScreenLogo;
	playBackGroundMusic();
	core::LoadingBar test(42);
	test.playLoadingAnimation(150, 0);
	core::clearCurrentConsoleLine(true, 0);
	std::cout << "\n" << "                 Press any key to continue...";
	bool keyPressed = false;
	while (!keyPressed) {
		if (_kbhit()) {
			keyPressed = true;
			core::clearCurrentConsoleLine(true, 0);
			std::cout << "\n";
		}
	}
	core::autoTypeString(welcomeMessage, 60);
}

bool core::doesUsernameExist(std::string* username) {
	if (username) {
		sqlitewrapper::SqliteDatabaseEngine databasehandler("players.db");
		std::string usernameSQLQuery = "SELECT Username FROM player WHERE Username='" + *username + "';";
		databasehandler.setExecutionStatement(usernameSQLQuery.c_str());
		databasehandler.executeStatement();
		
		if (databasehandler.getNumberOfReturnedRows())
		{
			return true;
		}

		return false;
	}
	
	return true;
}

void core::addNewPlayer(std::string* username, std::string* password) {
	if (username && password) {
		if (!core::doesUsernameExist(username)) {
			sqlitewrapper::SqliteDatabaseEngine* databaseHandle = new sqlitewrapper::SqliteDatabaseEngine("players.db");
			std::string* insertQuery = new std::string("INSERT INTO player (Username, ENCPassword) VALUES ('");
			std::string* hashedPassword = core::hashPassword(password);
			(*insertQuery) = (*insertQuery) + (*username) + "', '" + (*hashedPassword) + "');";
			databaseHandle->setExecutionStatement(insertQuery->c_str());
			databaseHandle->executeStatement();
			delete insertQuery;
			delete hashedPassword;
			
			databaseHandle->changeDatabaseConnection("leaderboard.db");
			std::string* leaderBoardQuery = new std::string("INSERT INTO playerleaderboard (Username, FF_Points, MAT_Points) VALUES('");
			(*leaderBoardQuery) = (*leaderBoardQuery) + (*username)+"', '0', '0');";
			databaseHandle->setExecutionStatement(leaderBoardQuery->c_str());
			databaseHandle->executeStatement();
			delete databaseHandle;
			delete leaderBoardQuery;
			
		}
	}
}

std::string* core::hashPassword(std::string* password) {
	if (password) {
		std::string* hash = new std::string(BCrypt::generateHash(*password));
		return hash;
	}

	return nullptr;
}

bool core::verifyPassword(std::string* username,std::string* password) {
	if (username && password) {
		if (core::doesUsernameExist(username)) {
			sqlitewrapper::SqliteDatabaseEngine* databaseHandle = new sqlitewrapper::SqliteDatabaseEngine("players.db");
			std::string* passwordQuery = new std::string("SELECT ENCPassword FROM player WHERE Username='");
			(*passwordQuery) = (*passwordQuery) + (*username) + "';";
			databaseHandle->setExecutionStatement(passwordQuery->c_str());
			databaseHandle->executeStatement();
			std::vector<std::string>* result = new std::vector<std::string>;
			databaseHandle->obtainStatementResultRow(result);
			std::string hash = (*result)[0];
			delete databaseHandle;
			delete passwordQuery;
			delete result;

			return BCrypt::validatePassword(*password, hash);
			
		}
		else
		{
			return false;
		}
	}

	return false;
}

GameFrame* core::createPlayerProfileScreen(GameModel* gameModel) {
	
	if (gameModel)
	{
		GameFrame* profileScreen = new GameFrame(playerProfileScreen);
		profileScreen->addFrameControl([=]() -> Callback_Return_Code
			{
				return runLogInProcedure(gameModel);
			});
		profileScreen->addFrameControl([=]() -> Callback_Return_Code
			{
				return runSignUpProcedure(gameModel);
			});
		profileScreen->addFrameControl([]() -> Callback_Return_Code
			{
				return GameSelectionMenu;
			});
		profileScreen->addFrameControl([=]()->Callback_Return_Code
			{
				toggleMusic(*gameModel);
				return CONTINUE;
			});
		profileScreen->addFrameControl([]() -> Callback_Return_Code
			{
				return ANDMainMenu;
			});
		return profileScreen;
	}
		
	return nullptr;
}

GameFrame* core::createMainMenuScreen(GameModel* gameModel) {
	
	GameFrame* mainMenu = new GameFrame(mainMenuScreen);
	mainMenu->addFrameControl([]()->Callback_Return_Code
			{
				return PlayerProfileScreen;
			});
	mainMenu->addFrameControl([]()->Callback_Return_Code
			{
				clearConsole();
				std::cout << aboutANDScreen;
				std::cout << "\n";
				std::cout << "         Press any key to return to main menu...";
				bool keyPressed = false;
				while (!keyPressed) {
					if (_kbhit()) {
						keyPressed = true;
						core::clearCurrentConsoleLine(true, 0);
						std::cout << "\n";
					}
				}
				clearConsole();
				return CONTINUE;
			});
	mainMenu->addFrameControl([]()->Callback_Return_Code
			{
				clearConsole();
				std::cout << aboutDevsScreen;
				std::cout << "\n";
				std::cout << "         Press any key to return to main menu...";
				bool keyPressed = false;
				while (!keyPressed) {
					if (_kbhit()) {
						keyPressed = true;
						core::clearCurrentConsoleLine(true, 0);
						std::cout << "\n";
					}
				}
				clearConsole();
				return CONTINUE;
			});
	mainMenu->addFrameControl([=]()->Callback_Return_Code
		{	
			if (gameModel)
			{
				toggleMusic(*gameModel);
			}
			return CONTINUE;
		});
	mainMenu->addFrameControl([]()->Callback_Return_Code 
		{
			return TERMINATE;
		});
	return mainMenu;

}

GameFrame* core::createGameSelectionScreen(GameModel* gameModel) {


	if (gameModel)
	{
		GameFrame* selectionScreen = new GameFrame(gameSelectionScreen);
		selectionScreen->addFrameControl([]()->Callback_Return_Code
			{
				return FrogsAndFliesMenu;
			});
		selectionScreen->addFrameControl([]()->Callback_Return_Code
			{
				return MathTriviaMenu;
			});
		selectionScreen->addFrameControl([]()->Callback_Return_Code
			{
				return FrogsAndFliesLeaderboardScreen;
			});
		selectionScreen->addFrameControl([=]()->Callback_Return_Code
			{
				toggleMusic(*gameModel);
				return CONTINUE;
			});
		selectionScreen->addFrameControl([=]()->Callback_Return_Code
			{
				
				return TutorialScreen;
			});
		selectionScreen->addFrameControl([]()->Callback_Return_Code
			{
				return PlayerProfileScreen;
			});
		return selectionScreen;
	}
	
	return nullptr;
}

GameFrame* core::createFrogsAndFliesLeaderboardScreen(GameModel* gameModel) {
	GameFrame* frogsAndFliesLeaderBoard = new GameFrame(frogsAndFliesLeaderboardBanner);
	frogsAndFliesLeaderBoard->addFrameControl([]()->Callback_Return_Code {
			return MathTriviaLeaderboardScreen;;
		});
	frogsAndFliesLeaderBoard->addFrameControl([]()->Callback_Return_Code {
		return GameSelectionMenu;
		});
	return frogsAndFliesLeaderBoard;
}

GameFrame* core::createMathTriviaLeaderboardScreen(GameModel* gameModel) {
	GameFrame* mathTriviaLeaderBoard = new GameFrame(mathTriviaLeaderboardBanner);
	mathTriviaLeaderBoard->addFrameControl([]()->Callback_Return_Code {
			return FrogsAndFliesLeaderboardScreen;;
		});
	mathTriviaLeaderBoard->addFrameControl([]()->Callback_Return_Code {
		return GameSelectionMenu;
		});
	return mathTriviaLeaderBoard;
}

GameFrame* core::createFrogsAndFliesGameMenu(GameModel* gameModel) {
	GameFrame* frogsandFliesMenu = new GameFrame(frogsAndFliesMenu);
	if (gameModel)
	{
		frogsandFliesMenu->addFrameControl([=]()->Callback_Return_Code {
			return runQuiz(Easy, FrogsAndFlies, gameModel);
			});
		frogsandFliesMenu->addFrameControl([=]()->Callback_Return_Code {
			return runQuiz(Medium, FrogsAndFlies, gameModel);
			});
		frogsandFliesMenu->addFrameControl([=]()->Callback_Return_Code {
			return runQuiz(Hard, FrogsAndFlies, gameModel);
			});
		frogsandFliesMenu->addFrameControl([=]()->Callback_Return_Code {
			toggleMusic(*gameModel);
			return CONTINUE;
			});
		frogsandFliesMenu->addFrameControl([]()->Callback_Return_Code {
			return GameSelectionMenu;
			});
	}
	return frogsandFliesMenu;
}

GameFrame* core::createMathTriviaGameMenu(GameModel* gameModel) {
	GameFrame* mathtriviaMenu = new GameFrame(mathTriviaMenu);
	if (gameModel)
	{
		mathtriviaMenu->addFrameControl([=]()->Callback_Return_Code {
			return runQuiz(Easy, MathTrivia, gameModel);
			});
		mathtriviaMenu->addFrameControl([=]()->Callback_Return_Code {
			return runQuiz(Medium, MathTrivia, gameModel);
			});
		mathtriviaMenu->addFrameControl([=]()->Callback_Return_Code {
			return runQuiz(Hard, MathTrivia, gameModel);
			});
		mathtriviaMenu->addFrameControl([=]()->Callback_Return_Code {
			toggleMusic(*gameModel);
			return CONTINUE;
			});
		mathtriviaMenu->addFrameControl([]()->Callback_Return_Code {
			return GameSelectionMenu;
			});
	}
	
	return mathtriviaMenu;
}

GameFrame* core::createTutorialScreen(GameModel* gameModel) {
	
	GameFrame* tutorial = new GameFrame(tutorialScreen);
	if (gameModel)
	{
		tutorial->addFrameControl([=]()->Callback_Return_Code
			{
				if (gameModel->musicToggle)
				{
					core::toggleMusic(*gameModel);
				}
				system("start https://www.youtube.com/watch?v=J0D2lVVctv8");
				return CONTINUE;
			});
		tutorial->addFrameControl([=]()->Callback_Return_Code
			{
				if (gameModel->musicToggle)
				{
					core::toggleMusic(*gameModel);
				}
				system("start https://www.youtube.com/watch?v=dQw4w9WgXcQ");
				return CONTINUE;
			});
		tutorial->addFrameControl([]()->Callback_Return_Code
			{
				return GameSelectionMenu;
			});
	}
	return tutorial;
}


void core::dispatchFrame(FrameID frameID, GameModel* gameModel) {
	if (gameModel && gameModel->hasFrame())
	{	
		
		switch (frameID)
		{
			case ANDMainMenu:
				gameModel->deleteCurrentFrame();
				gameModel->setGameFrame(core::createMainMenuScreen(gameModel));
				clearConsole();
				gameModel->printModel();
				std::cout << "Enter your choice...";
				break;
			case PlayerProfileScreen:
				gameModel->deleteCurrentFrame();
				gameModel->setGameFrame(core::createPlayerProfileScreen(gameModel));
				clearConsole();
				gameModel->printModel();
				gameModel->playerUsername = "guest";
				std::cout << "Enter your choice...";
				break;
			case GameSelectionMenu:
				gameModel->deleteCurrentFrame();
				gameModel->setGameFrame(core::createGameSelectionScreen(gameModel));
				clearConsole();
				gameModel->printModel();
				std::cout << "Logged in as: " << gameModel->playerUsername << "\n";
				std::cout << "Enter your choice...";
				break;
			case FrogsAndFliesLeaderboardScreen:
				gameModel->deleteCurrentFrame();
				gameModel->setGameFrame(core::createFrogsAndFliesLeaderboardScreen(gameModel));
				clearConsole();
				gameModel->printModel();
				core::displayFrogsandFliesLeaderboard();
				break;
			case MathTriviaLeaderboardScreen:
				gameModel->deleteCurrentFrame();
				gameModel->setGameFrame(core::createMathTriviaLeaderboardScreen(gameModel));
				clearConsole();
				gameModel->printModel();
				core::displayMathTriviaLeaderboard();
				break;
			case FrogsAndFliesMenu:
				gameModel->deleteCurrentFrame();
				gameModel->setGameFrame(createFrogsAndFliesGameMenu(gameModel));
				clearConsole();
				gameModel->printModel();
				std::cout << "Logged in as: " << gameModel->playerUsername << "\n";
				std::cout << "Enter your choice...";
				break;
			case MathTriviaMenu:
				gameModel->deleteCurrentFrame();
				gameModel->setGameFrame(createMathTriviaGameMenu(gameModel));
				clearConsole();
				gameModel->printModel();
				std::cout << "Logged in as: " << gameModel->playerUsername << "\n";
				std::cout << "Enter your choice...";
				break;
			case TutorialScreen:
				gameModel->deleteCurrentFrame();
				gameModel->setGameFrame(createTutorialScreen(gameModel));
				clearConsole();
				gameModel->printModel();
				break;
			case TERMINATE:
				gameModel->deleteCurrentFrame();
				break;
			default:
				gameModel->printModel();
		}

	}
}

void core::displayFrogsandFliesLeaderboard() {
	sqlitewrapper::SqliteDatabaseEngine* databaseHandle = new sqlitewrapper::SqliteDatabaseEngine("leaderboard.db");
	databaseHandle->setExecutionStatement("SELECT Username, FF_Points FROM playerleaderboard ORDER BY FF_Points DESC, Username ASC LIMIT 5;");
	databaseHandle->executeStatement();
	int numberOfPlayerScores = databaseHandle->getNumberOfReturnedRows();
	std::vector<std::string>* playerScore = new std::vector<std::string>();
	std::cout << "\n";
	std::cout << "Top Five Frogs And Flies Players" << std::endl;
	std::cout << "==========================================================\n";
	for (size_t i = 0; i < numberOfPlayerScores; i++)
	{
		playerScore->clear();
		databaseHandle->executeStatement();
		databaseHandle->obtainStatementResultRow(playerScore);
		std::cout << (*playerScore)[0] << " " << (*playerScore)[1] << std::endl;
		
	}
	std::cout << "\n";
	std::cout << "[0] - Math Trivia Leaderboard" << std::endl;
	std::cout << "\n";
	std::cout << "[1] - Return to Game Selection Menu" << std::endl;
	std::cout << "\n";
	std::cout << "Enter your choice...";
	delete databaseHandle;
	delete playerScore;
	
}

void core::displayMathTriviaLeaderboard() {
	sqlitewrapper::SqliteDatabaseEngine* databaseHandle = new sqlitewrapper::SqliteDatabaseEngine("leaderboard.db");
	databaseHandle->setExecutionStatement("SELECT Username, MAT_Points FROM playerleaderboard ORDER BY MAT_Points DESC, Username ASC  LIMIT 5;");
	databaseHandle->executeStatement();
	int numberOfPlayerScores = databaseHandle->getNumberOfReturnedRows();
	std::vector<std::string>* playerScore = new std::vector<std::string>();
	std::cout << "\n";
	std::cout << "Top Five Math Trivia Players" << std::endl;
	std::cout << "==========================================================\n";
	for (size_t i = 0; i < numberOfPlayerScores; i++)
	{
		playerScore->clear();
		databaseHandle->executeStatement();
		databaseHandle->obtainStatementResultRow(playerScore);
		std::cout << (*playerScore)[0] << " " << (*playerScore)[1] << std::endl;

	}
	std::cout << "\n";
	std::cout << "[0] - Frogs And Flies Leaderboard" << std::endl;
	std::cout << "\n";
	std::cout << "[1] - Return to Game Selection Menu" << std::endl;
	std::cout << "\n";
	std::cout << "Enter your choice...";
	delete databaseHandle;
	delete playerScore;
}

bool core::checkIfANSIEscapeSequenceSupported()
{
	// Set output mode to handle virtual terminal sequences
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	if (hIn == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	DWORD dwOriginalOutMode = 0;
	DWORD dwOriginalInMode = 0;
	if (!GetConsoleMode(hOut, &dwOriginalOutMode))
	{
		return false;
	}
	if (!GetConsoleMode(hIn, &dwOriginalInMode))
	{
		return false;
	}

	DWORD dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
	DWORD dwRequestedInModes = ENABLE_VIRTUAL_TERMINAL_INPUT;

	DWORD dwOutMode = dwOriginalOutMode | dwRequestedOutModes;
	if (!SetConsoleMode(hOut, dwOutMode))
	{
		// we failed to set both modes, try to step down mode gracefully.
		dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		dwOutMode = dwOriginalOutMode | dwRequestedOutModes;
		if (!SetConsoleMode(hOut, dwOutMode))
		{
			// Failed to set any VT mode, can't do anything here.
			return false;
		}
	}

	DWORD dwInMode = dwOriginalInMode | ENABLE_VIRTUAL_TERMINAL_INPUT;
	if (!SetConsoleMode(hIn, dwInMode))
	{
		// Failed to set VT input mode, can't do anything here.
		return false;
	}

	return true;
}

//This only works if your console supports ANSI escape sequences
void core::clearCurrentConsoleLine(bool goToStartOfLine, Length numberOfSpacesToProceed) {
	if (checkIfANSIEscapeSequenceSupported())
	{
		if (goToStartOfLine) {
			std::cout << "\33[2K\r";
		}
		else {
			std::cout << "\33[2K";
		}

		for (size_t i = 0; i < numberOfSpacesToProceed; i++)
		{
			std::cout << " ";
		}
	}
	
}

bool core::replaceCharInString(std::string& stringHandle , char replacementChar, CharPosition position) {
	if (position < 0 || position > stringHandle.size() - 1)
	{
		return false;
	}

	std::string charReplacement{ replacementChar };
	stringHandle.replace(position, 1, charReplacement);
}

void core::showConsoleCursor(bool displayFlag) {
	HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor;

	GetConsoleCursorInfo(outputHandle, &cursor);
	cursor.bVisible = displayFlag;
	SetConsoleCursorInfo(outputHandle, &cursor);
}

void core::runAND(){

#ifdef _WIN32
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, 650, 580, 1);
#endif // WIN_32
	system("color 16");
	std::string currentLoggedInPlayerUsername("");
	GameModel* andModel = new GameModel();
	andModel->setGameFrame(createMainMenuScreen(andModel));
	runGameIntroAnimation();
	std::this_thread::sleep_for(std::chrono::seconds(2));
	clearConsole();
	andModel->printModel();
	std::cout << "Enter your choice...";
	andModel->listen();
	clearConsole();
	delete andModel;
	core::runThankYouExitSequence();

}

void core::updatePlayerPoints(core::Points points, GameModel* gameModel, GameMode gameMode) {
	if (gameModel && gameModel->playerUsername.compare("guest") != 0)
	{
		sqlitewrapper::SqliteDatabaseEngine databaseHandle("leaderboard.db");
		core::Points newPointTotal;
		std::string columnToUpdate;
		std::string* username = &(gameModel->playerUsername);
		if (doesUsernameExist(username))
		{
			std::string query = "SELECT ";
			if (gameMode == FrogsAndFlies)
			{
				columnToUpdate = "FF_Points";
				query = query + "FF_Points FROM playerleaderboard WHERE Username='" + *username + "';";
			}
			else
			{
				columnToUpdate = "Mat_Points";
				query = query + "MAT_Points FROM playerleaderboard WHERE Username='" + *username + "';";
			}
			databaseHandle.setExecutionStatement(query.c_str());
			databaseHandle.executeStatement();
			std::vector<std::string>* resultVector = new std::vector<std::string>();
			databaseHandle.obtainStatementResultRow(resultVector);
			newPointTotal = std::stoi((*resultVector)[0]) + points;
			std::string updateQuery = "UPDATE playerleaderboard  SET " + columnToUpdate + "= " + std::to_string(newPointTotal) + " WHERE Username='" + *username + "';";
			databaseHandle.setExecutionStatement(updateQuery.c_str());
			databaseHandle.executeStatement();
			delete resultVector;
		}
	}
}

std::priority_queue<int, std::vector<int>, std::greater<int>>* core::getSortedRandomNumberQueue(int numberOfNumbers, int startOfRange, int endOfRange) {
	if (numberOfNumbers > 0 && endOfRange >= startOfRange && numberOfNumbers <= (endOfRange - startOfRange + 1))
	{
		std::random_device randomGen; //seed
		std::mt19937 rng(randomGen()); //psuedo random number generator
		std::uniform_int_distribution<int> uni(startOfRange, endOfRange); //uniformly distributed
		
		std::unordered_set<int>* alreadyGeneratedNumbers = new std::unordered_set<int>();
		std::priority_queue<int, std::vector<int>, std::greater<int>>* sortedRandomIntQueue = new std::priority_queue<int, std::vector<int>, std::greater<int>>();
		while ((sortedRandomIntQueue->size()) != numberOfNumbers)
		{
			int randomNumber = uni(rng);
			if (!alreadyGeneratedNumbers->count(randomNumber))
			{
				(*alreadyGeneratedNumbers).insert(randomNumber);
				sortedRandomIntQueue->push(randomNumber);
			}
			
		}
		delete alreadyGeneratedNumbers;
		return sortedRandomIntQueue;
	}
	return nullptr;
}

std::vector<std::pair<core::Question, int>> core::getQuestionSet(int numberOfQuestions, core::GameMode gameMode) {
	if (numberOfQuestions > 0)
	{
		std::vector<std::pair<core::Question, int>> questionAnswerVec;
		std::string gameModeQuestionDatabaseName = "";
		if (gameMode == core::FrogsAndFlies)
		{
			gameModeQuestionDatabaseName = "frogsandflies.db";
		}
		else
		{
			gameModeQuestionDatabaseName = "mathtrivia.db";
		}

		sqlitewrapper::SqliteDatabaseEngine databaseHandle(gameModeQuestionDatabaseName.c_str());
		std::string questionQuery = "SELECT DISTINCT * FROM questions;";
		databaseHandle.setExecutionStatement(questionQuery.c_str());
		databaseHandle.executeStatement();
		int numberOfQuestionsReturned = databaseHandle.getNumberOfReturnedRows();
		std::priority_queue<int, std::vector<int>, std::greater<int>>* rowsToObtain = core::getSortedRandomNumberQueue(numberOfQuestions,1,numberOfQuestionsReturned);
		if (rowsToObtain)
		{	
			int count = 1;
			std::vector<std::string>* questionRow = new std::vector<std::string>();
			while (!rowsToObtain->empty())
			{
				databaseHandle.executeStatement();
				if (count == rowsToObtain->top())
				{
					questionRow->clear();
					databaseHandle.obtainStatementResultRow(questionRow);
					core::Question newQuestion;
					newQuestion.question = (*questionRow)[0];
					newQuestion.possibleAnswerOne = (*questionRow)[2];
					newQuestion.possibleAnswerTwo = (*questionRow)[3];
					if ((*questionRow)[1].compare("0") == 0)
					{
						newQuestion.possibleAnswerThree = (*questionRow)[4];
						newQuestion.possibleAnswerFour = (*questionRow)[5];
						newQuestion.possibleAnswerFive = (*questionRow)[6];
					}

					int correctOption = std::stoi((*questionRow)[7]);
					std::pair<core::Question, int> questionAnswerPair(newQuestion, correctOption);
					questionAnswerVec.push_back(questionAnswerPair);
					rowsToObtain->pop();

				}
				count = count + 1;
			}
			delete rowsToObtain;
			delete questionRow;
		}
		return questionAnswerVec;

	}
	else
	{
		std::vector<std::pair<core::Question, int>> emptyVec;
		return emptyVec;
	}
}

std::string core:: generateQuizBannerTitle(core::GameMode gameMode, core::GameDifficulty difficulty) {
	std::string banner;
	if (gameMode == FrogsAndFlies)
	{
		switch (difficulty)
		{
		case Easy:
			banner = 
				"+==========================================================+\n"
				"\|                  Frogs And Flies Easy                    \|\n"
				"\|==========================================================\|\n";
			break;
		case Medium:
			banner = 
				"+==========================================================+\n"
				"\|                  Frogs And Flies Medium                  \|\n"
				"\|==========================================================\|\n";
			break;
		case Hard:
			banner = 
				"+==========================================================+\n"
				"\|                  Frogs And Flies Hard                    \|\n"
				"\|==========================================================\|\n";
			break;
		default:
			break;
		}
	}
	else
	{
		switch (difficulty)
		{
		case Easy:
			banner =
				"+==========================================================+\n"
				"\|                  Math Trivia Easy                        \|\n"
				"\|==========================================================\|\n";
			break;
		case Medium:
			banner =
				"+==========================================================+\n"
				"\|                  Math Trivia Medium                      \|\n"
				"\|==========================================================\|\n";
			break;
		case Hard:
			banner =
				"+==========================================================+\n"
				"\|                  Math Trivia Hard                        \|\n"
				"\|==========================================================\|\n";
			break;
		default:
			break;
		}
	}

	return banner;
}

void core::runThankYouExitSequence() {
	showConsoleCursor(false);
	std::cout << "******************************************************************" << std::endl;
	Sleep(100);
	std::cout << "*        _   _                 _                                 *" << std::endl;
	Sleep(100);
	std::cout << "*       | | | |               | |                                *" << std::endl;
	Sleep(100);
	std::cout << "*       | |_| |___  __ _ _ ___| | _   _   _  ___  _   _          *" << std::endl;
	Sleep(100);
	std::cout << "*       | __| '_  || _` | '_  | || | | | | || _ || | | |         *" << std::endl;
	Sleep(100);
	std::cout << "*       | |_| | | ||(_| | | | |   <  | |_| ||(_)|| |_| |         *" << std::endl;
	Sleep(100);
	std::cout << "*       |___|_| |_||__,_|_| |_|_||_| |___, ||___||___,_|         *" << std::endl;
	Sleep(100);
	std::cout << "*                                      __| |                     *" << std::endl;
	Sleep(100);
	std::cout << "*                                     |____|                     *" << std::endl;
	Sleep(100);
	std::cout << "*                                                                *" << std::endl;
	Sleep(100);
	std::cout << "*      _ ___                     _             _                 *  " << std::endl;
	Sleep(100);
	std::cout << "*     |  _  | ___  _ ___   _ __ | | __ _ _   _(_)_ __   __ _     *" << std::endl;
	Sleep(100);
	std::cout << "*    _| |_|_|| _ ||  _  | | '_ || || _` | | | | | '_  || _` |    *" << std::endl;
	Sleep(100);
	std::cout << "*   |_   _|  |(_)|| | |_| | |_)|| ||(_| | |_| | | | | ||(_| |    * " << std::endl;
	Sleep(100);
	std::cout << "*     |_|    |___||_|     | .__||_||__,_|___, |_|_| |_||__, |    *" << std::endl;
	Sleep(100);
	std::cout << "*                         |_|            |____|         |___|    *" << std::endl;
	Sleep(100);
	std::cout << "*                                                                *" << std::endl;
	Sleep(100);
	std::cout << "******************************************************************" << std::endl;
	Sleep(100);
	std::cout << "* Team Memebers: Darshan Pillay                                  *" << std::endl;
	Sleep(100);
	std::cout << "*                Nevan Padayachee                                *" << std::endl;
	Sleep(100);
	std::cout << "*                Aqeel Madhi                                     *" << std::endl;
	Sleep(100);
	std::cout << "*                                                                *" << std::endl;
	Sleep(100);
	std::cout << "* Developed by AND Games(A-Aqeel, N-Nevan, D-Darshan)            *" << std::endl;
	Sleep(100);
	std::cout << "* Implemented using C++ programming                              *" << std::endl;
	Sleep(100);
	std::cout << "*                                                                *" << std::endl;
	Sleep(100);
	std::cout << "*                  -COMP315 Final Project-                       *" << std::endl;
	Sleep(100);
	std::cout << "*                                                                *" << std::endl;
	Sleep(100);
	std::cout << "******************************************************************" << std::endl;

	Sleep(5000);
}

void core::toggleMusic(GameModel& gameModel) {
	
	if (gameModel.musicToggle) {
		gameModel.musicToggle = false;
		core::turnMusicOff();
	}
	else
	{
		gameModel.musicToggle = true;
		core::playBackGroundMusic();
	}
	
}

