#include <stdint.h>

/* Minimal stand-in for ST's power-mode entry function.
   Real H7 systems configure SMPS/LDO here — for a first bring-up
   test we do nothing and just return. */
void ExitRun0Mode(void)
{
    /* intentionally empty for now */
}

/* Minimal stand-in for ST's clock/FPU early init.
   Real H7 systems enable FPU access and set up basic clock state here.
   We at least enable the FPU so float/double code doesn't fault. */
void SystemInit(void)
{
    /* Enable FPU (CP10 and CP11 full access) */
    volatile uint32_t *CPACR = (volatile uint32_t*)0xE000ED88;
    *CPACR |= (0xF << 20);
}

/* Stub required by __libc_init_array (C++ static constructor support).
   We have none, so this can be empty. */
void _init(void)
{
    /* intentionally empty */
}