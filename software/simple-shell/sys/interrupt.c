/*
 * interrupt.c
 *
 * Created on: Jul 20, 2012
 *     Author: Ekawahyu Susilo
 *
 * Copyright (c) 2012, Chongqing Aisenke Electronic Technology Co., Ltd.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * 
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of the copyright holder.
 * 
 */

#include <unistd.h>
#include "board.h"
#include "interrupt.h"

/* Platform may have the lowest IRQn as negative number and here we assign
 * it as array index. Therefore, we need to offset the lowest negative number
 * to conform with the array index 0. The original IRQn is kept within the
 * irq_handlers[] structure.
 */

static irq_hook_t *irq_handlers[NR_IRQ_VECTORS] = { NULL };

void disable_irq(int irq, unsigned int flags)
{
	if (irq > NR_IRQ_VECTORS + LOWEST_IRQn) return;

	if (irq_is_hooked(irq))
		if (irq_handlers[irq - LOWEST_IRQn]->disable_irq) {
			irq_handlers[irq - LOWEST_IRQn]->disable_irq(irq, flags);
			irq_handlers[irq - LOWEST_IRQn]->flags &= ~flags;
		}
}

void enable_irq(int irq, unsigned int flags)
{
	if (irq > NR_IRQ_VECTORS + LOWEST_IRQn) return;

	if (irq_is_hooked(irq))
		if (irq_handlers[irq - LOWEST_IRQn]->enable_irq) {
			irq_handlers[irq - LOWEST_IRQn]->enable_irq(irq, flags);
			irq_handlers[irq - LOWEST_IRQn]->flags |= flags;
		}
}

void hook_irq_handler(irq_hook_t *hook, int irq, irq_handler_t handler)
{
	if (irq > NR_IRQ_VECTORS + LOWEST_IRQn) return;

	irq_handlers[irq - LOWEST_IRQn] = hook;
	if (handler) irq_handlers[irq - LOWEST_IRQn]->handler = handler;
	irq_handlers[irq - LOWEST_IRQn]->id = irq;
}

int irq_is_hooked(int irq)
{
	if (irq_handlers[irq - LOWEST_IRQn])
		return 1;
	else
		return 0;
}

int irq_is_enabled(int irq, unsigned int flags)
{
	if (irq_handlers[irq - LOWEST_IRQn]) {
		if (irq_handlers[irq - LOWEST_IRQn]->flags & flags)
			return 1;
		else
			return 0;
	}
	else
		return 0;
}

void remove_irq_handler(int irq)
{
	if (irq > NR_IRQ_VECTORS + LOWEST_IRQn) return;

	irq_handlers[irq - LOWEST_IRQn]->handler = NULL;
	irq_handlers[irq - LOWEST_IRQn]->id = 0;
	irq_handlers[irq - LOWEST_IRQn] = NULL;
}

void request_to_handle(int irq, unsigned int flags)
{
	if (irq > NR_IRQ_VECTORS + LOWEST_IRQn) return;

	if (irq_handlers[irq - LOWEST_IRQn])
		irq_handlers[irq - LOWEST_IRQn]->handler(flags);
	else
		irq_handlers[irq - LOWEST_IRQn]->disable_irq(irq, flags);
}
