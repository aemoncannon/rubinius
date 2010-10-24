#ifndef UTILITIES_HPP
#define UTILITIES_HPP



#define DUMP_BITCODE false
#define GDB_BREAK __asm__("int3")


#define DEBUG true


#ifdef DEBUG
#define DEBUGLN(x) std::cout << x << endl
#define IF_DEBUG(x) x
#else
#define DEBUGLN(x)
#define IF_DEBUG(x)
#endif


//#define TRACE_PROFILE true

int const NULL_TIMER = 0;
int const ON_TRACE_TIMER = 1;
int const INTERP_TIMER = 2;
int const IN_EXIT_TIMER = 3;
int const UNCOMMON_INTERP_TIMER = 4;
int const TRACE_SETUP_TIMER = 5;
int const TRACE_COMPILER_TIMER = 6;
int const NUM_TIMERS = 7;

const string TRACE_TIMER_LABELS[7] = {
	"NULL",
	"Trace",
	"Interp", 
	"Exit-Stub",
	"Uncommon Interp",
	"Trace Bookkeeping",
	"Compilation"};

#ifdef TRACE_PROFILE
#define TRACK_TIME(x) state->start_trace_timer(x)
#define TRACK_TIME_ON_TRACE(x) //track_time(x)
#define IF_TRACE_PROFILE(x) x
#else
#define TRACK_TIME(x)
#define IF_TRACE_PROFILE(x)
#define TRACK_TIME_ON_TRACE(x)
#endif



#endif
