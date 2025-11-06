#pragma once
#include <vector>
#include <mutex>
#include <condition_variable>
#include <random>
#include "Dungeon.h"

class Dispatcher {
public:
	Dispatcher(int nInstances,
		int tank, int healer, int dps,
		int t1, int t2);

	//main scheduler --> form parties and launch dungeon instance
	void run();

	//status
	void printStatus() const;
	void printSummary() const; 

private:
	bool canFormPartyLocked() const;
	int findFreeDungeonIndexLocked();

	const int n;
	const int t1;
	const int t2;

	int nextPartyId = 0001; 

	//remaining players
	int tanks; 
	int healers;
	int dps_r; 

	//instances
	int freeInstances;
	std::vector<std::unique_ptr<Dungeon>> dungeons; 

	//sync
	mutable std::mutex m;
	std::condition_variable cv;

	std::mt19937 rng;
	std::uniform_int_distribution<int> dist; 

};