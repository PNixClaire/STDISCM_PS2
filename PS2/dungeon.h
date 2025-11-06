#pragma once

#include <thread>
#include <atomic>
#include <string>
#include <condition_variable>
#include <mutex>

class Dungeon {

public:
	explicit Dungeon(int id); //what instance it is

	Dungeon(const Dungeon&) = delete;
	Dungeon& operator=(const Dungeon&) = delete; 

	Dungeon(Dungeon&&) noexcept = default;
	Dungeon& operator=(Dungeon&&) noexcept = default;

	void start(int durationSEc,
		std::mutex& m,
		int& freeInstances,
		std::condition_variable& cv,
		int partyId);

	void join();

	bool isActive() const;

	int getServed() const;
	int getTotalTime() const;
	int getId() const;
	std::string getStatus() const; 

private:
	int id;

	std::atomic<bool> active;
	std::atomic<int> served;
	std::atomic<int> totalTime;
	std::thread worker; 
	
};