/*
	*
	Author: Diogo Guilherme Garcia de Freitas
	*
*/

#ifndef ENCODER_H
#define ENCODER_H

using namespace std;

union FourBytes
{
	int value;
	char byte[4]; 
};

class Encoder
{
private:
	int Base;
	int IndexBase;
	int Channel;
	FourBytes Counter;
	int Rotations;
	int FullRotation;
	double Angle;

public:
	Encoder();
	void SetBase(int b, int ib);
	void SetChannel(int c);
	int GetCounterValue();
	int GetRotations();
	int GetFullRotation();
	bool GetPermissions();
	bool DetectIndexPulse();
	void Reset();
	void ReadCounter();
	void Calibrate();
	void operator!();
	double ConvertToDegrees();
};

#endif
