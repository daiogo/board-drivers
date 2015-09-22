/*
	*
	Author: Diogo Guilherme Garcia de Freitas
	*
*/

#ifndef PC104_H
#define PC104_H

using namespace std;

class PC104
{
private:
	int Base;
	int Channel;
	int LowByte;
	int HighByte;
public:
	PC104();
	void SetBase(int b);
	void SetChannel(int c);
	bool GetPermissions();
	int ConvertToByte(double data);
	void WriteAnalogOutput(int data);
};

#endif
