#include <bur/plctypes.h>
#include <inttypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

static uint64_t cycles, cycles_saved;

uint64_t rdtsc() {
	uint32_t low, high;
	__asm__ __volatile__(
		"rdtsc" 
		: "=a" (low), 
		"=d" (high)
	);
	return ((uint64_t)high << 32) | low;
}

void _INIT ProgramInit(void)
{
	cycles_saved = rdtsc();
}

void _CYCLIC ProgramCyclic(void)
{
	cycles = rdtsc();
	CycleTimeTicks = (UDINT)(cycles - cycles_saved);
	cycles_saved = cycles;
}

void _EXIT ProgramExit(void) { }
