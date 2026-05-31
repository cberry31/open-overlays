#include <iostream>
#include <vector>
#include <boost/beast.hpp>
using namespace std;

#include "irsdk_defines.h"
#include "irsdk_client.h"

irsdkCVar g_SessionFlags("SessionFlags"); // (int) irsdk_Flags, bitfield
vector<string> getColors(int flags);

irsdkCVar g_CarIdxGear("CarIdxGear");


tuple<string, int> getFlags(int flag) {
	// wait up to 16 ms for start of session or new data
	if (irsdkClient::instance().waitForData(16)) {
		int currentFlag = g_SessionFlags.getInt();
		if (flag != currentFlag) {
			flag = currentFlag;
			vector<string> colors = getColors(currentFlag);
			for (string color : colors) {
				std::cout << color << std::endl;
			}
			std::cout << "-----" << std::endl;
			if (colors.size()) {
				return { colors.at(0), flag };
			}
		}
	}
	return { "", flag };
}

vector<string> getColors(int flags) {
	vector<string> namedFlags;
	// global flags
	if ((flags & irsdk_yellow) || (flags & irsdk_yellowWaving) || (flags & irsdk_caution) || (flags & irsdk_cautionWaving)) {
		namedFlags.push_back("yellow");
	}

	if (flags & irsdk_checkered) namedFlags.push_back("checkered");
	if (flags & irsdk_white) namedFlags.push_back("white");
	if (flags & irsdk_green) namedFlags.push_back("green");
	if (flags & irsdk_red) namedFlags.push_back("red");
	if (flags & irsdk_blue) namedFlags.push_back("blue");
	if (flags & irsdk_debris) namedFlags.push_back("debris");
	if (flags & irsdk_crossed) namedFlags.push_back("crossed");

	if ((flags & irsdk_black) || (flags & irsdk_disqualify) || (flags & irsdk_furled)) namedFlags.push_back("black");
	if (flags & irsdk_repair) namedFlags.push_back("meatball");

	return namedFlags;
}

int getGear() {
	// TODO: Create a "game loop" where we only need to connect to the game once and then we send data every "frame"

	// DriverInfo:
	//     DriverCarIdx: 0
	if (irsdkClient::instance().waitForData(16)) {
		const int MAX_STR = 1024;
		char tstr[MAX_STR];
		if (1 == irsdkClient::instance().getSessionStrVal("DriverInfo:DriverCarIdx:", tstr, MAX_STR))
		{
			int driverCarIdx = atoi(tstr);
			return g_CarIdxGear.getInt(driverCarIdx);
		}

	}
	return 0;
}