#pragma once
#include<vector>
#include<functional>

/*
	File Name: frame_control.h
	Date     : 25/05/2021
	Author   : Darshan Pillay (Student#219012395)
	Creation : COMP315 - Advanced Programming { Major Group Project }

	Details  : This header file contains the definiton of a class
			   named FrameControlHandler which will provide a mechanism for
			   simple UI event handling for a GameFrame (see game_frame.h)

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
constexpr auto CONTINUE = 0;
constexpr auto TERMINATE = INT_MAX;

typedef std::function<int()> callback;
typedef unsigned int Callback_Return_Code;
typedef unsigned int controlID;

class FrameControlHandler {
public:
	FrameControlHandler();
	void addCallBackFunction(callback callBackFunction);
	Callback_Return_Code getCallBackReturnCode() { return returnCode;}
	bool callControlCallback(controlID controlNumber);
	~FrameControlHandler();
private:
	std::vector<std::function<Callback_Return_Code()>>* controlCallBackFunctions;
	Callback_Return_Code returnCode;
};