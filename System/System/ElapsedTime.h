/**
 * @file System/ElapsedTime.h
 * @ingroup System
 * @brief Definition of ElapsedTime class
 */

#ifndef __ELAPSED_TIME_H__
#define __ELAPSED_TIME_H__

#include <System/ConfigSystem.h>
#include <System/Portage.h>
#include <ratio>
#include <chrono>

namespace Omiscid {

/**
 * @class ElapsedTimeBase ElapsedTime.cpp System/ElapsedTime.h
 * @brief Template base class for c++11 elapsed time computation.
 *
 * @author Dominique Vaufreydaz, Grenoble Alpes University, Inria
 */
template<typename ChronoType, typename StdType, typename DurationType>
class ElapsedTimeBase
{
protected:
	typedef typename ChronoType::time_point TimePoint; 

public:
	/** @brief Constructor */
	ElapsedTimeBase()
	{
		Reset();
	}

	/** @brief Copy constructor */
	ElapsedTimeBase(const ElapsedTimeBase& Other)
	{
		operator=(Other);
	}

	/** @brief Virtual destructor */
	virtual ~ElapsedTimeBase() {}

	/** @brief Operator= */
	const ElapsedTimeBase& operator=(const ElapsedTimeBase& Other)
	{
		StartTime = Other.StartTime;

		return Other;
	}	

	/** @brief reset the start time */
	void Reset()
	{
		StartTime = ChronoType::now();
	}

	/** @brief Get the elapsed time in DurationType since construction or last reset.
	 * @param DoReset says if we need to reset the object with a new start time
	 * @return the Elapsed Time in ms since the
	 */
	StdType Get(bool DoReset = false)
	{
		TimePoint CurrentTime = ChronoType::now();

		StdType res = (StdType)(std::chrono::duration_cast<DurationType>(CurrentTime-StartTime).count());

		if ( DoReset == true )
		{
			// Reset the Time and return elapsed time
			StartTime = CurrentTime;
		}

		return res;
	}

	/** @brief Get the elapsed time in second since construction or last reset.
	* @param DoReset says if we need to reset the object with a new start time
	* @return the Elapsed Time in second (and microsecond) since the
	*/
	double GetInSeconds(bool DoReset = false)
	{
		TimePoint CurrentTime = ChronoType::now();

		// Better way to write convertion to second, i.e. ratio<1,1>
		std::chrono::duration<double, std::ratio<1,1>> ElapsedTimeInSecond = CurrentTime-StartTime;
		double res = ElapsedTimeInSecond.count();

		if ( DoReset == true )
		{
			// Reset the Time and return elapsed time
			StartTime = CurrentTime;
		}

		return res;
	}

	/** @brief Get the current time in DurationType and StdType
	 */
	static StdType GetCurrentTime()
	{
		return std::chrono::duration<StdType,DurationType>(ChronoType::now());
	}

protected:
	TimePoint StartTime;	/*!< A value to store start time in TimePoint  */
};

/**
 * @class PerfElapsedTime ElapsedTime.cpp System/ElapsedTime.h
 * @brief Enable to compute the elapse time between two calls in milliseconds.
 *
 * @author Dominique Vaufreydaz, Grenoble Alpes University, Inria
 */
class ElapsedTime : public ElapsedTimeBase<std::chrono::steady_clock,unsigned int,std::chrono::milliseconds>
{
public:
	/** @brief Constructor */
	ElapsedTime() {}

	/** @brief Virtual destructor */
	virtual ~ElapsedTime() {}
};

/**
 * @class ElapsedTime ElapsedTime.cpp System/ElapsedTime.h
 * @brief Enable to compute the elapse time between two calls in microseconds.
 *
 * @author Dominique Vaufreydaz, Grenoble Alpes University, Inria
 */
class PerfElapsedTime : public ElapsedTimeBase<std::chrono::high_resolution_clock,unsigned long long,std::chrono::microseconds>
{
public:
	/** @brief Constructor */
	PerfElapsedTime() {}

	/** @brief Virtual destructor */
	virtual ~PerfElapsedTime() {}
};

} // namespace Omiscid

#endif // __ELAPSED_TIME_H__

