#include <bur/plctypes.h>
#include <inttypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

#include "sha256.h"


uint64_t rdtsc() {
	uint32_t low, high;
	__asm__ __volatile__(
	"rdtsc" 
	: "=a" (low), 
	"=d" (high)
	);
		return ((uint64_t)high << 32) | low;
}
 
static long long begin, begin_saved, end;

void _INIT ProgramInit(void)
{
	int i; //C99
	for(i = 0; i< sizeof(buffer); i++) buffer[i] = 'a';
}

void _CYCLIC ProgramCyclic(void)
{
	SHA256_CTX ctx;

	begin = rdtsc();
	cycle_time = (LREAL)(begin - begin_saved);
	begin_saved = begin;
	
	sha256_init(&ctx);
	sha256_update(&ctx, buffer, sizeof(buffer));
	sha256_final(&ctx, digest);

	end = rdtsc();
	cpu_time_used = ((LREAL)(end - begin));

	MBperSec = cycle_time / cpu_time_used;
}

void _EXIT ProgramExit(void) {}
