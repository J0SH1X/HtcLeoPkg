#ifndef ARMLIB_DISABLE_INTERRUPTS_H_
#define ARMLIB_DISABLE_INTERRUPTS_H_

#define FUNCTION(x)                     \
  .text;                                \
  .align 4;                             \
  .global x;                            \
  .type x, %function;                   \
  x:

#ifndef __ASSEMBLY__
#include <stdint.h>
//#include <asm.h>

#define CF do { asm volatile("" ::: "memory"); } while(0)

int critical_section_count;

static inline void arch_disable_ints(void) {
  uint32_t cpsr;
  __asm__ volatile("mrs %0, cpsr\n\t"
                   "orr %0, %0, #(1<<7)\n\t"
                   "msr cpsr_c, %0"
                   : "=r" (cpsr)
                   :
                   : "memory");
}

static inline void arch_enable_ints(void) {
  uint32_t cpsr;
  __asm__ volatile("mrs %0, cpsr\n\t"
                   "bic %0, %0, #(1<<7)\n\t"
                   "msr cpsr_c, %0"
                   : "=r" (cpsr)
                   :
                   : "memory");
}

static inline void enter_critical_section(void){
    CF;
    
    if (critical_section_count == 0){
        arch_disable_ints();
    }
    critical_section_count++;
    CF;
}

static inline void exit_critical_section(void){
    CF;
    critical_section_count--;
    if (critical_section_count == 0){
        arch_enable_ints();
    }
    CF;
}

static inline int in_critical_section(void){
    CF;
    return critical_section_count > 0;
}



#endif /* __ASSEMBLY__ */

#endif /* ARMLIB_DISABLE_INTERRUPTS_H_ */

