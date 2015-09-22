/*
	*
	Author: Diogo Guilherme Garcia de Freitas
	*
*/

#include <sys/io.h>
#include <iostream>
#include <math.h>
#include "PCM3718.h"

PCM3718::PCM3718()
{
	Base = 0x300;
	Channel = 0;
	LastLowByte = 0x00;
	LastHighByte = 0x00;
}

void PCM3718::SetBase(int b)
{
	Base = b;
}

void PCM3718::SetChannel(int c)
{
	Channel = c;
}

void PCM3718::SetupADConversion()
{
	outb(0x00, Base + 0x01); //Set input range
	outb((Channel | (Channel << 4)), Base + 0x02); //Set input channel
	outb(0x00, Base + 0x08); //Clear SREG
	usleep(100000);
}

void PCM3718::ResetChannels()
{
	outb(0x00, Base + 0x03); //Initialize low and high bytes as 0x00
	outb(0x00, Base + 0x0B);
}

bool PCM3718::GetPermissions()
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

int PCM3718::DigitalInputByte()
{
	return inb(Base + (Channel*8) + 0x03);
}

int PCM3718::DigitalInputBit(int bit)
{
	int byte = inb(Base + 0x03);
	int mask = (int) pow(2.0, bit);
	int state = 0;

	if((byte & mask) == mask) //Checks if bit selected is currently on or off
	{
		state = 1; //If the bit selected is 1, then the current state is 1 (on). Otherwise, it remains 0 (off)
	}

	return state;
}

void PCM3718::DigitalOutputByte(int data)
{
	outb(data, Base + (Channel*8) + 0x03);

	if(Channel == 0) //Updates the value of the correspondent attribute
	{
		LastLowByte = data;
	}
	else if(Channel == 1)
	{
		LastHighByte = data;
	}
}

void PCM3718::DigitalOutputBit(int data, int bit)
{
	int output = 0;
	int mask = (int) pow(2.0, bit);
	int state = 0;

	if(Channel == 0) //output is a temporary variable that 
	{
		output = LastLowByte;
	}
	else if(Channel == 1)
	{
		output = LastHighByte;
	}

	if((output & mask) == mask) //Checks if bit selected is currently on or off
	{
		state = 1; //If the bit selected is 1, then the current state is 1 (on). Otherwise, it remains 0 (off)
	}

	if(state != data) //If the current state is different from the desired one, change it. Otherwise, it remains unchanged
	{
		output = (output ^ mask); //Will toggle the desired bit
		outb(output, Base + (Channel*8) + 0x03);
	}

	if(Channel == 0) //Updates the value of the correspondent attribute
	{
		LastLowByte = output;
	}
	else if(Channel == 1)
	{
		LastHighByte = output;
	}
}

double PCM3718::ConvertToVolts(int data)
{
	return (((data*10)/4095)-5); //4095 == 0xFFF which is the max value that can be wrote in the 12 bit converter
}

double PCM3718::ReadAnalogInput()
{
	outb(0x00, Base); //Trigger conversion
	while(inb(Base + 0x08) & 0x10); //Wait until the INT bit turns on
	int low = inb(Base); //Read values of low and high byte
	int high = inb(Base + 0x01);
	return ConvertToVolts((low >> 4) | (high << 4));
}

ostream& operator<<(ostream& os, PCM3718& card)
{
	for(int i = 0 ; i < 2 ; i++) //Loops through all channels
	{
		card.SetChannel(i);
		card.SetupADConversion();
		os << "Channel " << i << ": " << card.ReadAnalogInput() << " volts" << endl;
	}
	return os;
}

