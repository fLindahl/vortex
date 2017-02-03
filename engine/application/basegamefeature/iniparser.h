#include <string>
#include <sstream>
#include <fstream>

namespace BaseGameFeature
{
	static float GetSettings()
	{
		float value;

		std::ifstream fin("settings.ini");
		std::string line;
		std::istringstream sin;

		while (std::getline(fin, line))
		{
			sin.str(line.substr(line.find("=") + 1));
			if (line.find("UPDATE_MULTIPLIER") != std::string::npos) {
				sin >> value;
				return value;
			}
			sin.clear();
		}
		return 1.0f;
	}
}