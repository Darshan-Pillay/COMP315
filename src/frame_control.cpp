#include "Headers/frame_control.h"
#include <vector>
#include<functional>

/*
	File Name: frame_control.cpp
	Date     : 25/05/2021
	Author   : Darshan Pillay (Student#219012395)
	Creation : COMP315 - Advanced Programming { Major Group Project }

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

FrameControlHandler::FrameControlHandler() {
	this->controlCallBackFunctions = new std::vector<std::function<Callback_Return_Code()>>();
	this->returnCode = CONTINUE;
}

FrameControlHandler::~FrameControlHandler() {
	delete this->controlCallBackFunctions;
}

void FrameControlHandler::addCallBackFunction(callback callBackFunction) {
	if (this->controlCallBackFunctions)
	{
		(*(this->controlCallBackFunctions)).push_back(callBackFunction);
	}
	
}

bool FrameControlHandler::callControlCallback(controlID controlNumber) {
	if ((*this->controlCallBackFunctions).size() - 1 < controlNumber || (*this->controlCallBackFunctions).empty()) {
		return false;
	}
	
	returnCode = (*this->controlCallBackFunctions)[controlNumber]();
	return true;
}