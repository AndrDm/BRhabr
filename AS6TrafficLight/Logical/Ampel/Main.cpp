
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

// amount of memory to be allocated for heap storage must be specified 
// for every ANSI C++ program with the bur_heap_size variable
unsigned long bur_heap_size = 0xFFFF;

#include "machine.hpp"
#include "Logging.hpp"
#include "States.hpp"

//////////////////////////////////////////////////////////////////////////////// 

// shared data storage instance
Context context;
FSM::Instance machine{context};

void _INIT ProgramInit(void)
{
	// Insert code here
	CyclesCount = 0;
	ClearTraceLog();
	DurationRed = 5;
	DurationYellow = 3;
	DurationGreen = 5;
	DurationGreenBlinking = 3;
	LightRed = false;
	LightYellow = false;
	LightGreen = false;
	CountDownRedON = false;
	CountDownGreenON = false;
}


void _CYCLIC ProgramCyclic(void)
{
	// Insert code here 
	machine.update();
	
	LightRedOFF = !LightRed; LightYellowOFF = !LightYellow; LightGreenOFF = !LightGreen;
	CountDownRedON = LightRed;
	CyclesCount++;
	TraceMessage("---cycle---");
}


void _EXIT ProgramExit(void)
{
	// Insert code here 
}
