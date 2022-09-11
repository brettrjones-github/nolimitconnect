#pragma once

#include <stdint.h>
#include <vector>

class TimeIntervalEstimator
{
public:
	static const int			MIN_TIME_ESTIMATE_COUNT = 3; // minimun number of system times to estimate what the next time read/write samples time should be
	static const int			MAX_TIME_ESTIMATE_COUNT = 8; // dont grow the number of sample times past this or takes to much cpu time for little gain in accuracy

	TimeIntervalEstimator() = default;
	TimeIntervalEstimator( int64_t intervalUs );

	void						setIntervalUs( int64_t intervalUs )			{ m_IntervalUs = intervalUs; m_SystemTimesMsList.clear(); }
	int64_t						getIntervalUs( void )						{ return m_IntervalUs; }
	bool						getHasMaxTimestamps( void )					{ return m_SystemTimesMsList.size() == MAX_TIME_ESTIMATE_COUNT; }

	int64_t						estimateTime( int64_t systemTimeMs, bool* retTooLongInterval = nullptr );

	void						reset( void )								{ m_SystemTimesMsList.clear(); }

	int64_t						m_IntervalUs{ 0 };
	std::vector<int64_t>		m_SystemTimesMsList;
};
