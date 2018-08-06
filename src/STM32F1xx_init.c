#include "STM32F1xx.h"
#include "common.h"

/**
 * Initialize the HSI clock source and reset the PLL configuration. This
 * function is called by the startup_common.s file, just before calling the
 * main() function.
 *
 * You can adapt this function to fit your needs, however, do not change its
 * name! It is called in the startup code.
 */
void SystemInit (void) {
  // Set HSION bit
  RCC->CR |= 0x00000001U;

  // Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits
#if !defined(STM32F105xC) && \
    !defined(STM32F107xC)
  RCC->CFGR &= 0xF8FF0000U;
#else
  RCC->CFGR &= 0xF0FF0000U;
#endif

  // Reset HSEON, CSSON and PLLON bits
  RCC->CR &= 0xFEF6FFFFU;

  // Reset HSEBYP bit
  RCC->CR &= 0xFFFBFFFFU;

  // Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits
  RCC->CFGR &= 0xFF80FFFFU;

#if defined(STM32F105xC) || \
    defined(STM32F107xC)
  // Reset PLL2ON and PLL3ON bits
  RCC->CR &= 0xEBFFFFFFU;

  // Disable all interrupts and clear pending bits
  RCC->CIR = 0x00FF0000U;

  // Reset CFGR2 register
  RCC->CFGR2 = 0x00000000U;

#elif defined(STM32F100xB) || \
      defined(STM32F100xE)
  // Disable all interrupts and clear pending bits
  RCC->CIR = 0x009F0000U;

  // Reset CFGR2 register
  RCC->CFGR2 = 0x00000000U;

#else
  // Disable all interrupts and clear pending bits
  RCC->CIR = 0x009F0000U;
#endif
}

// These symbols are provided by the linker
extern void (*__preinit_array_start[])();
extern void (*__preinit_array_end[])();
extern void (*__init_array_start[])();
extern void (*__init_array_end[])();

/**
 * Calls the functions in the preinit and init arrays. The start and end
 * addresses of these arrays are provided by the linker.
 *
 * You can adapt this function to fit your needs, however, do not change its
 * name! It is called in the startup code.
 */
void __main () {
  for (void (**f)() = __preinit_array_start; f < __preinit_array_end; ++f) {
    (*f)();
  }

  for (void (**f)() = __init_array_start; f < __init_array_end; ++f) {
    (*f)();
  }
}
