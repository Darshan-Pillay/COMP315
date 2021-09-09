#pragma onc
#include <string>
#include "Headers/game_frame.h"
#include "Headers/game_model.h"
#include <queue>
#include <map>
#include <Windows.h>

namespace core {
	typedef unsigned int AnimationSpeed;
	typedef unsigned int FrameID;
	typedef unsigned int Length;
	typedef unsigned int CharPosition;
	typedef unsigned int GameMode;
	typedef unsigned int GameDifficulty;
	typedef unsigned int Points;

	constexpr GameDifficulty Easy = 1;
	constexpr GameDifficulty Medium = 2;
	constexpr GameDifficulty Hard = 3;

	constexpr GameMode FrogsAndFlies = 1;
	constexpr GameMode MathTrivia = 2;

	constexpr FrameID ANDMainMenu = 1;
	constexpr FrameID PlayerProfileScreen = 2;
	constexpr FrameID GameSelectionMenu = 3;
	constexpr FrameID FrogsAndFliesLeaderboardScreen = 4;
	constexpr FrameID MathTriviaLeaderboardScreen = 5;
	constexpr FrameID FrogsAndFliesMenu = 6;
	constexpr FrameID MathTriviaMenu = 7;
	constexpr FrameID TutorialScreen = 8;

	
	class LoadingBar {
		public:
			LoadingBar(Length length);
			int playLoadingAnimation(AnimationSpeed speed, FrameID frameID);
		private:
			int loadingBarWidth;
			
	};

	void updatePlayerPoints(Points points, GameModel* gameModel, GameMode gameMode);

	Callback_Return_Code runQuiz(GameDifficulty difficulty, GameMode gameMode, GameModel* gameModel);

	GameFrame* createPlayerProfileScreen(GameModel* gameModel);

	GameFrame* createTutorialScreen(GameModel* gameModel);

	GameFrame* createMainMenuScreen(GameModel* gameModel);

	GameFrame* createGameSelectionScreen(GameModel* gameModel);

	GameFrame* createMathTriviaLeaderboardScreen(GameModel* gameModel);

	GameFrame* createFrogsAndFliesLeaderboardScreen(GameModel* gameModel);

	GameFrame* createFrogsAndFliesGameMenu(GameModel* gameModel);
	
	GameFrame* createMathTriviaGameMenu(GameModel* gameModel);

	Callback_Return_Code runSignUpProcedure(GameModel* gameModel);

	Callback_Return_Code runLogInProcedure(GameModel* gameModel);

	void runGameIntroAnimation();

	void displayMathTriviaLeaderboard();
	
	void displayFrogsandFliesLeaderboard();

	/*
	* Prints out each character of the arguement string
	* at charPerMillisecond milliseconds per second. 
	* This makes it seems as if the text being printed
	* is being typed by a person.
	*/
	void autoTypeString(const std::string& text, AnimationSpeed charPerMillisecond);

	//Clears all text from the console
	void clearConsole();

	std::string* hashPassword(std::string* password);

	bool verifyPassword(std::string* username, std::string* password);

	void addNewPlayer(std::string* username, std::string* hashedPassword);

	/*
	* Returns true if the argument is a nullpointer
	* or if the string which username points to
	* is a string occuring as a value in the Username
	* column of the player database.
	*/
	bool doesUsernameExist(std::string* username);

	void dispatchFrame(FrameID frameID, GameModel* gameModel);

	void clearCurrentConsoleLine(bool goToStartOfLine, Length numberOfSpacesToProceed);

	bool checkIfANSIEscapeSequenceSupported();

	bool replaceCharInString(std::string& stringHandle,char replacementChar, CharPosition position);

	void showConsoleCursor(bool displayFlag);

	void runAND();

	std::priority_queue<int, std::vector<int>, std::greater<int>>* getSortedRandomNumberQueue(int numberOfElements,int startOfRange,int endOfRange);

	struct Question {
		std::string question;
		std::string possibleAnswerOne;
		std::string possibleAnswerTwo;
		std::string possibleAnswerThree;
		std::string possibleAnswerFour;
		std::string possibleAnswerFive;


	};

	inline std::ostream& operator<< (std::ostream& out, const Question& question) {
		out << "Question: " << question.question << std::endl;
		out << "\n" << "1) " << question.possibleAnswerOne << std::endl;
		out << "\n" << "2) " << question.possibleAnswerTwo << std::endl;

		if (question.possibleAnswerThree.compare("NULL") != 0 && question.possibleAnswerThree.length() > 0)
		{
			out << "\n" << "3) " << question.possibleAnswerThree << std::endl;
		}

		if (question.possibleAnswerFour.compare("NULL") != 0 && question.possibleAnswerFour.length() > 0)
		{
			out << "\n" << "4) " << question.possibleAnswerFour << std::endl;
		}

		if (question.possibleAnswerFive.compare("NULL") != 0 && question.possibleAnswerFive.length() > 0)
		{
			out << "\n" << "5) " << question.possibleAnswerFive << std::endl;
		}
		return out;
	}

	std::vector<std::pair<Question, int>> getQuestionSet(int numberOfQuestions, GameMode gameMode);

	std::string generateQuizBannerTitle(GameMode gameMode, GameDifficulty difficulty);

	template<typename Key, typename Value>
	inline std::vector<std::pair<Key, Value>> jumbleMap(std::map<Key, Value> inputMap)
	{
		std::srand(time(NULL));
		std::vector<std::pair<Key, Value>> orderedPairVector;


		for (typename std::map<Key, Value>::iterator pairIterator = inputMap.begin(); pairIterator != inputMap.end(); pairIterator++) {
			orderedPairVector.push_back(std::pair<Key, Value>(pairIterator->first, pairIterator->second));
		}

		std::random_shuffle(orderedPairVector.begin(), orderedPairVector.end());

		return orderedPairVector;
	}

	void runThankYouExitSequence();

	inline void playBackGroundMusic() {
		PlaySound(TEXT("BGM.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
	}

	inline void turnMusicOff() {
		PlaySound(NULL, NULL, 0);
	}

	void toggleMusic(GameModel& gameModel);

};
