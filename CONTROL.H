#ifndef CONTROL_H
#define CONTROL_H

#include <stdbool.h>

unsigned long time = 0;

bool Timer(unsigned int m)
{
	if(time == 0)
	{
		time = 1;
		return false;
	}

	if(time >= m)
	{
		time = 0;
		return true;
	}
	else
	{
		++time;
		return false;
	}
}

bool Start()
{
	return time == 0;
}

#endif CONTROL_H