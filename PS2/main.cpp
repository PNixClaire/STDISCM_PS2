#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <chrono>
#include <ctime>
#include "Dispatcher.h"

//read the config file
std::unordered_map<std::string, int> readConfig(const std::string& filename) {

	std::unordered_map<std::string, int> config;
	std::ifstream file(filename);

	if (!file) throw std::runtime_error("Could not open config file.");

	std::string line;
	while (std::getline(file, line)) {
		auto pos = line.find('=');
		
		if (pos != std::string::npos) {
			std::string key = line.substr(0, pos);
			int value = std::stoi(line.substr(pos + 1));
			config[key] = value;
		}
	}

	return config; 
}

//time stamp
std::string getTime() {

	using namespace std::chrono;
	auto tp = system_clock::now();

	std::time_t tt = system_clock::to_time_t(tp);
	std::tm tm{};

#if defined(_WIN32)
	localtime_s(&tm, &tt);
#endif 

	char buf[32];
	std::strftime(buf, sizeof(buf), "%H:%M:%S", &tm);
	return buf; 

}

int main() {
	try {
		auto config = readConfig("config.txt");

		int n = config["n"];
		int t = config["t"]; //tank
		int h = config["h"]; //healer
		int d = config["d"]; //dps
		int t1 = config["t1"];
		int t2 = config["t2"];

		std::cout << "===== LFG DUNGEON SYSTEM =====\n";
		std::cout << "Instances: " << n << "\n";
		std::cout << "Players queued:\n";
		std::cout << "\tTank: " << t << "\n"
			<< "\tHealer: " << h << "\n"
			<< "\tDPS: " << d << "\n";
		std::cout << "==============================\n\n";

		Dispatcher dispatcher(n, t, h, d, t1, t2);

		std::cout << "[START] " << getTime() << "\n";
		auto start = std::chrono::high_resolution_clock::now();

		dispatcher.run();

		auto end = std::chrono::high_resolution_clock::now();
		std::cout << "[END] " << getTime() << "\n";

		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		std::cout << "Elapsed time: " << ms << "ms\n"; 

		dispatcher.printSummary();
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << "\n"; 
		return 1;
	}

	return 0; 
}