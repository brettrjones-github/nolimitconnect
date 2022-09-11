
#include "TimeIntervalEstimator.h"
#include "VxDebug.h"

//============================================================================
TimeIntervalEstimator::TimeIntervalEstimator( int64_t intervalUs )
	: m_IntervalUs( intervalUs )
{
}

//============================================================================
int64_t	TimeIntervalEstimator::estimateTime( int64_t systemTimeMs, bool* retTooLongInterval )
{
	if( retTooLongInterval )
	{
		*retTooLongInterval = false;
	}

	if( !m_IntervalUs || !systemTimeMs )
	{
		return systemTimeMs;
	}

	if( m_SystemTimesMsList.empty() )
	{
		m_SystemTimesMsList.push_back( systemTimeMs );
		return systemTimeMs;
	}	

	if( systemTimeMs - m_SystemTimesMsList.back() > (m_IntervalUs * 4) / 1000 )
	{
		// interval greater than twice the known interval between time samples so
		// device was probably paused or turned off or changed for some time 
		if( retTooLongInterval )
		{
			LogMsg( LOG_WARNING, "TimeIntervalEstimator reset because time diff %d > max interval %d", (int)(systemTimeMs - m_SystemTimesMsList.back()), (m_IntervalUs * 4) / 1000 );
			*retTooLongInterval = true;
		}

		m_SystemTimesMsList.clear();
	}

	if( m_SystemTimesMsList.size() < MIN_TIME_ESTIMATE_COUNT )
	{
		// not enough times to make an estimate	
		m_SystemTimesMsList.push_back( systemTimeMs );
		return systemTimeMs;
	}

	int64_t sumOfTime{ 0 };
	int64_t intervalAddUs = 0;
	int64_t intervalTotalUs = 0;
	for( std::vector<int64_t>::reverse_iterator riter = m_SystemTimesMsList.rbegin(); riter != m_SystemTimesMsList.rend(); riter++ )
	{
		sumOfTime += *riter;
		intervalAddUs += m_IntervalUs;
		intervalTotalUs += intervalAddUs;
	}

	int64_t estTime = (sumOfTime + (intervalTotalUs / 1000)) / m_SystemTimesMsList.size();

	if( m_SystemTimesMsList.size() >= MAX_TIME_ESTIMATE_COUNT )
	{
		m_SystemTimesMsList.erase( m_SystemTimesMsList.begin() );
	}

	m_SystemTimesMsList.push_back( systemTimeMs );
	
	return estTime;
}