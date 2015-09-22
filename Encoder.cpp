/*
	*
	Author: Diogo Guilherme Garcia de Freitas
	*
*/

#include <sys/io.h>
#include <iostream>
#include "Encoder.h"

using namespace std;

Encoder::Encoder()
{
	Base = 0x200;
	IndexBase = 0x800;
	Channel = 0;
	Counter.value = 0;
	Angle = 0;
	Rotations = 0;
	FullRotation = 4096; //Approximate value on the counter on a full rotation
}

void Encoder::SetBase(int b, int ib)
{
	Base = b;
	IndexBase = ib;
}

void Encoder::SetChannel(int c)
{
	Channel = c;
}

int Encoder::GetCounterValue()
{
	return Counter.value;
}

int Encoder::GetRotations()
{
	//Handles rotation counting
	//Compares Counter.value to keep track of COMPLETE roations done
	//A negative value will mean a counter clockwise rotation
	Rotations = Counter.value / FullRotation;
	return Rotations;
}

int Encoder::GetFullRotation()
{
	return FullRotation;
}

bool Encoder::GetPermissions()
{
	//Ask for IO permissions
	if((ioperm(IndexBase, 8, 1) != 0) || (ioperm(Base, 16, 1) != 0))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Encoder::DetectIndexPulse()
{
	//Acquire data from IndexBase and mask MSB to detect the index pulse
	bool index = (bool) (inb(IndexBase) & 0x080);
	if(index)
	{	
		return true;
	}
	else
	{
		return false;
	}
}

void Encoder::Reset()
{
	outb(0x00, Base + Channel); //Write 0x00 to reset counter on Channel
}

void Encoder::ReadCounter()
{
	//Acquire bytes form encoder counter
	for(int i = 0 ; i < 4 ; i++)
		Counter.byte[i] = inb(Base + (Channel*4) + i);

	usleep(100);
}

void Encoder::Calibrate()
{
	while(this->DetectIndexPulse() != true){} //Wait for the first pulse
	
	this->Reset(); //Reset to start counting from zero

	usleep(10000); //Sleep so the program won't be fast enough to detect the same pulse twice
	
	while(this->DetectIndexPulse() != true) //Read the counter while waitin for the next pulse
	{
		this->ReadCounter();
	}

	FullRotation = Counter.value; //Value on counter becomes the value of a full rotation

	this->Reset(); //Reset again
}

void Encoder::operator!() //Overload operator ! to reset on all channels
{
	//Originally should loop till channel 3, but it doesn't exist so loop till channel 2
	for(int ch = 0 ; ch < 4 ; ch++) //Loops thorugh all possible channels
	{
		outb(0x00, Base + ch); //Write zeros to the counter address
	}
}

double Encoder::ConvertToDegrees()
{
	Angle = (double) ((Counter.value*360)/FullRotation); //Use proportionality to reset encoder counter
	return Angle;
}

