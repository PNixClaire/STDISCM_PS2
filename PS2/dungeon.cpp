#include "Dungeon.h"
#include <chrono>
#include <thread>
#include <iostream>

//constructor
Dungeon::Dungeon(int id) :
	id(id), active(false), served(0), totalTime(0) {}

//start
void Dungeon::start(int durationSec, std::mutex& m, int& freeInstances, std::condition_variable& cv, int partyId) {

	active.store(true);

	std::cout << "\n\nParty #" << partyId
		<< " entered Dungeon " << id << "\n\n";

	//worker threads for dungeons
	worker = std::thread([=, &m, &freeInstances, &cv]() {
		//simulate dungeon activity
		std::this_thread::sleep_for(std::chrono::seconds(durationSec));

		//add parties served
		served.fetch_add(1);
		//add total time 
		totalTime.fetch_add(durationSec);

		{
			std::lock_guard<std::mutex> lock(m);
			active.store(false);
			freeInstances += 1;

			std::cout << "\n\nParty #" << partyId
				<< " finished Dungeon " << id << " (duration: " << durationSec << "s)\n\n";
		}

		cv.notify_all();
	});
}

void Dungeon::join() {
	if (worker.joinable()) worker.join();
}

bool Dungeon::isActive() const { return active.load(); }

int Dungeon::getServed() const { return served.load(); }
int Dungeon::getTotalTime() const { return totalTime.load(); }
int Dungeon::getId() const { return id; }


//debug
std::string Dungeon::getStatus() const {
	return "[Dungeon " + std::to_string(id) + "] " + (isActive() ? "active" : "empty");
}