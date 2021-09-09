#include "shuffler.h"
#include <random>
#include <chrono>
#include <algorithm>



template<typename T> 
Shuffler<T>::Shuffler(std::vector<T> initialVector) {
	vectorContainer = initialVector;
}

template<typename T>
void Shuffler<T>::shuffle() {
	if (!vectorContainer.empty())
	{	
		unsigned number = std::chrono::system_clock::now().time_since_epoch().count();
		std::shuffle(vectorContainer.begin(), vectorContainer.end(),std::default_random_engine(number));
	}
}



