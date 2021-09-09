#pragma once
#include <stack>
#include <queue>

template<typename T>
struct Shuffler {
	Shuffler<T>(std::vector<T> initialVector);
	void shuffle();
	~Shuffler<T>() {};
	std::vector<T> vectorContainer;
};