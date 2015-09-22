/*
	*
	Author: Diogo Guilherme Garcia de Freitas
	*
*/

#ifndef PCM3718_H
#define PCM3718_H

using namespace std;

class PCM3718
{
private:
	int Base;
	int Channel;
	int LastLowByte;
	int LastHighByte;
public:
	PCM3718();
	void SetBase(int b);
	void SetChannel(int c);
	void SetupADConversion();
	void ResetChannels();
	bool GetPermissions();
	int DigitalInputByte();
	int DigitalInputBit(int bit);
	void DigitalOutputByte(int data);
	void DigitalOutputBit(int data, int bit);
	double ConvertToVolts(int data);
	double ReadAnalogInput();
	friend ostream& operator<<(ostream& os, PCM3718& card);
};

#endif
