#include <Platform/ops.h>
#include <Library/OpsLib.h>

/* critical sections */
int critical_section_count;

//static inline 
void enter_critical_section(void)
{
	critical_section_count++;
	if (critical_section_count == 1)
		arch_disable_ints();
}

//static inline 
void exit_critical_section(void)
{
	critical_section_count--;
	if (critical_section_count == 0)
		arch_enable_ints();
}