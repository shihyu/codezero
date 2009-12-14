/*
 * Definitions for exception support on ARM
 *
 * Copyright (C) 2007 Bahadir Balban
 */
#ifndef __ARCH_EXCEPTIONS_H__
#define __ARCH_EXCEPTIONS_H__

#include INC_ARCH(asm.h)


static inline void enable_irqs()
{
	__asm__ __volatile__(
		"mrs	r0, cpsr_fc\n"
		"bic	r0, r0, #0x80\n" /* ARM_IRQ_BIT */
		"msr	cpsr_fc, r0\n"
	);
}


static inline void disable_irqs()
{
	__asm__ __volatile__(
		"mrs	r0, cpsr_fc\n"
		"orr	r0, r0, #0x80\n" /* ARM_IRQ_BIT */
		"msr	cpsr_fc, r0\n"
	);
}

int irqs_enabled();

/* Disable the irqs unconditionally, but also keep the previous state such that
 * if it was already disabled before the call, the restore call would retain
 * this state. */
void irq_local_disable_save(unsigned long *state);
#if 0
{
	unsigned long temp;
	__asm__ __volatile__ (
		"mrs	%0, cpsr_fc\n"
		"orr	%2, %0, #0x80\n"
		"msr	cpsr_fc, %2\n"
		: "=r" (*state)
		: "r" (*state),"r" (temp)
	);
}
#endif

/* Simply change it back to original state supplied in @flags. This might enable
 * or retain disabled state of the irqs for example. Useful for nested calls. */
void irq_local_restore(unsigned long state);

static inline void irq_local_enable()
{
	enable_irqs();
}

static inline void irq_local_disable()
{
	disable_irqs();
}

/* This is filled on entry to irq handler, only if a process was interrupted.*/
extern unsigned int preempted_psr;

/*
 * FIXME: TASK_IN_KERNEL works for non-current tasks, in_kernel() works for current task?
 * in_kernel() is for irq, since normally in process context you know if you are in kernel or not :-)
 */

/* Implementing these as functions cause circular include dependency for tcb.h */
#define TASK_IN_KERNEL(tcb)	(((tcb)->context.spsr & ARM_MODE_MASK) == ARM_MODE_SVC)
#define TASK_IN_USER(tcb)	(!TASK_IN_KERNEL(tcb))

static inline int in_kernel()
{
	return (((preempted_psr & ARM_MODE_MASK) == ARM_MODE_SVC)) ? 1 : 0;
}

static inline int in_user()
{
	return !in_kernel();
}

int pager_pagein_request(unsigned long vaddr, unsigned long size,
			 unsigned int flags);

#endif
