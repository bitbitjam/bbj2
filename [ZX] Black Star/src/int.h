#ifndef _INT_H
#define _INT_H

#include <im2.h>

#ifndef WFRAMES
#define WFRAMES		4
#endif // WFRAMES

#define clock(x)	(*tick)

// timer
const uchar *tick = 0x5c78;
uchar timer = 0;

M_BEGIN_ISR_LIGHT(isr)
{
	// update the clock
	++*tick;
}
M_END_ISR_LIGHT

void
wait()
{
	while(*tick - timer < WFRAMES)
    {
#asm
        halt
#endasm
    }
	timer = *tick;
}

void
setup_int()
{
	uchar *pt = 0xd000;

   	im2_Init(0xd000);
	memset(pt, 0xd1, 258);

	pt = 0xd1d1;
	*pt++ = 0xc3;
	*(uint *)pt = isr;
}

#endif // _INT_H

