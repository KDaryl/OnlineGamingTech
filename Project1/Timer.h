#pragma once
class Timer
{
public:
	Timer();
	~Timer();

	void start();
	void stop();
	void pause();
	void unpause();

	//Getters
	int getTicks();
	bool& isStarted() { return m_started; }
	bool& isPause() { return m_paused; }
private:
	//The clock time when the timer starts
	int m_startTicks;
	//The ticks stored when the timer is paused
	int m_pausedTicks;

	//The timer status
	bool m_paused;
	bool m_started;
};

