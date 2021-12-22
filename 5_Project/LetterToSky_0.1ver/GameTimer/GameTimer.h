//***************************************************************************************
// GameTimer.h by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************
#pragma once

class GameTimer
{
public:
	GameTimer();

	float TotalTime()const;  // 초 단위
	float DeltaTime()const;  // 초 단위

	void Reset(); // 메세지 루프 이전에 호출해야 함
	void Start(); // 타이머 시작, 재개 시 호출해야 함
	void Stop();  // 일시정지 호출해야 함
	void Tick();  // 매 프레임마다 호출해야 함

private:
	double mSecondsPerCount;
	double mDeltaTime;

	__int64 mBaseTime;
	__int64 mPausedTime;
	__int64 mStopTime;
	__int64 mPrevTime;
	__int64 mCurrTime;

	bool mStopped;
};
