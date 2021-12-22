//***************************************************************************************
// GameTimer.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************
#include <windows.h>
#include "framework.h"
#include "GameTimer.h"

GameTimer::GameTimer()
	: mSecondsPerCount(0.0), mDeltaTime(-1.0),
	mBaseTime(0), mPausedTime(0), mStopTime(0), mPrevTime(0), mCurrTime(0),
	mStopped(false)
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	mSecondsPerCount = 1.0 / (double)countsPerSec;
}

// Returns the total time elapsed since Reset() was called, NOT counting any
// time when the clock is stopped.
float GameTimer::TotalTime()const
{
	// 현재 타이머가 중단 상태이면 중단된 후로부터 흐른 시간은 계산하지 말아야 한다
	// 또한 이전에 일시 정지된 적이 있다면 그 시간도 전체 시간에 포함시키지 말아야 한다
	// 이를 위해 mStopTime에서 일시정지 시간을 뺀다 
	//
	//                     |<--paused time-->|
	// ----*---------------*-----------------*------------*------------*------> time
	//  mBaseTime       mStopTime        startTime     mStopTime    mCurrTime

	if (mStopped)
	{
		return (float)(((mStopTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
	}

	// 시간차 mCurrTime - mBaseTime에는 일시 정지된 시간이 포함되어 있다
	// 이를 전체 시간에 포함시키면 안 되므로, 그 시간을 mCurrTime에서 제한다
	//
	//  (mCurrTime - mPausedTime) - mBaseTime 
	//
	//                     |<--paused time-->|
	// ----*---------------*-----------------*------------*------> time
	//  mBaseTime       mStopTime        startTime     mCurrTime

	else
	{
		return (float)(((mCurrTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
	}
}

float GameTimer::DeltaTime()const
{
	return (float)mDeltaTime;
}

void GameTimer::Reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	mBaseTime = currTime;
	mPrevTime = currTime;
	mStopTime = 0;
	mStopped = false;
}

void GameTimer::Start()
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	// 중단과 시작(재개) 사이에 흐른 시간을 누적한다
	//
	//                     |<-------d------->|
	// ----*---------------*-----------------*------------> time
	//  mBaseTime       mStopTime        startTime     

	// 중단 상태에서 타이머를 다시 재개하는 경우라면
	if (mStopped)
	{
		// 일시정지 시간을 누적한다
		mPausedTime += (startTime - mStopTime);

		// 타이머를 다시 시작하는 것이므로 mPrevTime(이전시간)의 현재 값은
		// 유효하지 않다(일시정지 도중에 갱신된 값이기 때문)
		// 따라서 현재 시간으로 다시 설정한다
		mPrevTime = startTime;

		// 더 이상은 중단 상태가 아님
		mStopTime = 0;
		mStopped = false;
	}
}

void GameTimer::Stop()
{
	// 이미 정지 상태이면 아무 일도 하지 않는다
	if (!mStopped)
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		// 그렇지 않다면 중단 시간(일시 정지된 시점의 시간)을 저장하고,
		// 타이머가 중단되었음을 뜻하는 부울 플래그를 설정한다
		mStopTime = currTime;
		mStopped = true;
	}
}

void GameTimer::Tick()
{
	if (mStopped)
	{
		mDeltaTime = 0.0;
		return;
	}

	// 이번 프레임의 시간을 얻는다
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mCurrTime = currTime;

	// 이 시간과 이전 프레임의 시간의 차이를 구한다
	mDeltaTime = (mCurrTime - mPrevTime) * mSecondsPerCount;

	// 다음 프레임을 준비한다
	mPrevTime = mCurrTime;

	// 음수가 되지 않게 한다, SDK 문서화의 CDXUTTimer 항목에 따르면 
	// 프로세서가 절전모드로 들어가거나 다른 프로세서와 엉키는 경우
	// mDeltaTime이 음수가 될 수 있다
	if (mDeltaTime < 0.0)
	{
		mDeltaTime = 0.0;
	}
}
