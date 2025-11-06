#include "Dispatcher.h"
#include <iostream>
#include <chrono>

//constructor
Dispatcher::Dispatcher(int nInstances, int tank, int healer, int dps, int t1, int t2) :
	n(nInstances),
	t1(t1), t2(t2),
	tanks(tank), healers(healer), dps_r(dps),
	freeInstances(nInstances),
	rng(std::mt19937(std::random_device{}())),
	dist(std::uniform_int_distribution<int>(std::min(t1, t2), std::max(t1, t2))) {

	dungeons.reserve(n);
	
	for (int i = 0; i < n; ++i)
		dungeons.emplace_back(std::make_unique<Dungeon>(i));
}

bool Dispatcher::canFormPartyLocked() const {
	return (tanks >= 1 && healers >= 1 && dps_r >= 3);
}

int Dispatcher::findFreeDungeonIndexLocked() {
	for (int i = 0; i < n; ++i) {
		if (!dungeons[i]->isActive()) {
			dungeons[i]->join();
			return i;
		}
	}

	return -1;
}

void Dispatcher::printStatus() const {
	std::lock_guard<std::mutex> lock(m);
	std::cout << "\nInstances:\n";

	for (int i = 0; i < n; ++i) {
		std::cout << " " << dungeons[i]->getStatus() << "\n";
	}

	std::cout << "Queue remaining: T=" << tanks
		<< " H=" << healers
		<< " D=" << dps_r << "\n";
}

void Dispatcher::printSummary() const {
	std::lock_guard<std::mutex> lock(m);
	std::cout << "\nSUMMARY\n";
		for (int i = 0; i < n; ++i) {
			std::cout << " [Dungeon " << dungeons[i]->getId()
				<< "] served=" << dungeons[i]->getServed()
				<< " total_time=" << dungeons[i]->getTotalTime()
				<< "s\n";
		}
}

void Dispatcher::run() {
	printStatus();

	while (true)
	{
		std::unique_lock<std::mutex> lock(m);

		if (!canFormPartyLocked()) {
			lock.unlock();
			break;
		}

		//wait for free instance if there is none available
		cv.wait(lock, [&] {
			return freeInstances > 0;
			});

		//check the roles
			if (!canFormPartyLocked()) {
				lock.unlock();
				break;
		}

		//consume roles
			tanks -= 1;
			healers -= 1;
			dps_r -= 3;
			freeInstances -= 1;

			int partyId = nextPartyId++;

		//pick a dungeon [must be an inactive one]
			int idx = findFreeDungeonIndexLocked();
			if (idx < 0) {
				freeInstances += 1;
				lock.unlock();
				continue;
			}

		//duration of dungeon
			int duration = dist(rng);
			auto& d = dungeons[idx];

			lock.unlock();

			d->start(duration, m, freeInstances, cv, partyId);

			//debug
			printStatus();
	}

	for (auto& d : dungeons) {
		d->join();
	}

	printStatus();
}
