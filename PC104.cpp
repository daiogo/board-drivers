/*
	*
	Author: Diogo Guilherme Garcia de Freitas
	*
*/

#include <sys/io.h>
#include <iostream>
#include "PC104.h"

PC104::PC104()
{
	Base = 0x320;
	Channel = 0x00;
	LowByte = 0x00;
	HighByte = 0x00;
}

void PC104::SetBase(int b)
{
	Base = b;
}

void PC104::SetChannel(int c)
{
	Channel = c;
}

bool PC104::GetPermissions()
{
	if(ioperm(Base, 16, 1) != 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int PC104::ConvertToByte(double data)
{
	return (int) ((data + 5)*0xFFF)/10; //Convert input in volts to correspondent byte values
}

void PC104::WriteAnalogOutput(int data)
{
	LowByte = (data & 0xFF); //Mask data to obtain low byte
	HighByte = ((data & 0xF00) >> 8); //Mask data and shift value to obtain high byte

	outb(LowByte, Base + Channel); //Write low and high byte on respective addresses
	outb(HighByte, Base + 0x01 + Channel);
}

