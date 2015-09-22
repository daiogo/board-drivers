/*
	*
	Author: Diogo Guilherme Garcia de Freitas
	Notes: This class was created to facilitate the interfacing between the user and the cards
		libraries. The validation of user input is NOT fully implemented!
	*
*/

#include <sys/io.h>
#include <iostream>
#include "Menu.h"

Menu::Menu()
{
	Option = 0;
	Option = 0;

	if(pcm3718.GetPermissions())
	{
		cout << "I/O perm error on PCM3718" << endl;
		MainMenu();
	}
	pcm3718.ResetChannels();
	
	if(pc104.GetPermissions())
	{
		cout << "I/O perm error on PC104-DAC06" << endl;
		MainMenu();
	}

	if(enc.GetPermissions())
	{
		cout << "I/O perm error MSI-404" << endl;
		MainMenu();
	}
}

void Menu::MainMenu()
{
	do
	{
		cout << "----------MAIN MENU------------" << endl;
		cout << "Which device do you want to use?" << endl;
		cout << "1 - PCM3718 (A/D converter and IO card)" << endl;
		cout << "2 - PC104-DAC06 (D/A converter)" << endl;
		cout << "3 - MSI-404 (Encoder)" << endl;
		cout << "4 - Quit" << endl;
		cin >> Option;
	}while(Option < 1 || Option > 4);

	switch(Option)
	{
	case 1:
		Menu1();
		break;
	case 2:
		Menu2();
		break;
	case 3:
		Menu3();
		break;
	case 4:
		//systemexit
		break;
	}
}

void Menu::Menu1()
{
	int bit = 0; //User input variables
	int data = 0;
	int channel = 0;
	
	do
	{
		cout << "----------PCM3718------------" << endl;
		cout << "1 - Read digital input (byte)" << endl;
		cout << "2 - Read digital input (bit)" << endl;
		cout << "3 - Write digital output (byte)" << endl;
		cout << "4 - Write digital output (bit)" << endl;
		cout << "5 - Analog input on specific channel (A/D converter)" << endl;
		cout << "6 - Analog input on all channels (A/D converter)" << endl;
		cout << "7 - Back to main menu" << endl;
		cin >> Option;
	}while(Option < 1 || Option > 7);

	switch(Option)
	{
	case 1: //Digital input (byte)
		do
		{
			cout << "Which channel do you wish to use? (0-1)" << endl;
			cin >> channel;
		}while(channel < 0 || channel > 1);
		
		pcm3718.SetChannel(channel);

		cout << "Byte read from channel " << channel << ": " << pcm3718.DigitalInputByte() << endl;

		Menu1();
		break;

	case 2: //Digital input (bit)
		do
		{
			cout << "Which channel do you wish to use? (0-1)" << endl;
			cin >> channel;
		}while(channel < 0 || channel > 1);
		
		pcm3718.SetChannel(channel);

		do
		{
			cout << "Which bit do you wish to read from? (0-7)" << endl;
			cin >> bit;
		}while(bit < 0 || bit > 7);

		cout << "Bit "<< bit << " read from channel " << channel << ": " << pcm3718.DigitalInputBit(bit) << endl;

		Menu1();
		break;

	case 3: //Digital output (byte)
		do
		{
			cout << "Which channel do you wish to use? (0-1)" << endl;
			cin >> channel;
		}while(channel < 0 || channel > 1);
		
		pcm3718.SetChannel(channel);

		do
		{
			cout << "Which byte do you wish to write? (0-255)" << endl;
			cin >> data;
		}while(data < 0 || data > 255);

		pcm3718.DigitalOutputByte(data);

		cout << "Operation sucessful!" << endl;

		Menu1();
		break;

	case 4: //Digital output (bit)
		do
		{
			cout << "Which channel do you wish to use? (0-1)" << endl;
			cin >> channel;
		}while(channel < 0 || channel > 1);
		
		pcm3718.SetChannel(channel);

		do
		{
			cout << "Which value do you want to write in? (0-1)" << endl;
			cin >> data;
		}while(data != 0 && data != 1);

		do
		{
			cout << "In which bit? (0-7)" << endl;
			cin >> bit;
		}while(bit < 0 || bit > 7);

		pcm3718.DigitalOutputBit(data, bit);

		cout << "Operation sucessful!" << endl;

		Menu1();
		break;

	case 5: //Analog input on specific channel (A/D converter)
		do
		{
			cout << "Which channel do you wish to use? (0-1)" << endl;
			cin >> channel;
		}while(channel < 0 || channel > 1);
		
		pcm3718.SetChannel(channel);
		pcm3718.SetupADConversion();

		cout << "Channel " << channel << ": " << pcm3718.ReadAnalogInput() << " volts" << endl;

		Menu1();
		break;

	case 6: //Analog input on all channels (A/D converter)
		cout << pcm3718;

		Menu1();
		break;

	case 7: //Back to main menu
		MainMenu();
		break;
	}
}

void Menu::Menu2()
{
	int channel = 0; //User input variables
	double voltage = 0;

	do
	{
		cout << "----------PC104-DAC06------------" << endl;
		cout << "1 - Analog output on specific channel" << endl;
		cout << "2 - Back to main menu" << endl;
		cin >> Option;
	}while(Option < 1 || Option > 2);
	
	switch(Option)
	{
	case 1: //Analog output on specific channel
		do
		{
			cout << "Which channel do you wish to use? (0-5)" << endl;
			cin >> channel;
		}while(channel < 0 || channel > 5);

		pc104.SetChannel(channel);

		do
		{
			cout << "What is the desired voltage? (-5.00 to 5.00)" << endl;
			cin >> voltage;
		}while(voltage < -5 || voltage > 5);
	
		pc104.WriteAnalogOutput(pc104.ConvertToByte(voltage));

		cout << "Operation sucessful!" << endl;

		Menu2();
		break;

	case 2: //Back to main menu
		MainMenu();
		break;
	}
}

void Menu::Menu3()
{
	int channel = 0; //User input variables
	
	do
	{
		cout << "----------MSI-404------------" << endl;
		cout << "1 - Reset a specific channel" << endl;
		cout << "2 - Read from a specific channel" << endl;
		cout << "3 - Reset all channels" << endl;
		cout << "4 - Calibrate encoder" << endl;
		cout << "5 - Back to main menu" << endl;
		cin >> Option;
	}while(Option < 1 || Option > 5);

	switch(Option)
	{
	case 1: //Reset a specific channel (only ranges from channel 0 to channel 2, as channel 3 is connected to nothing)
		do
		{
			cout << "Which channel do you wish to reset? (0-3)" << endl;
			cin >> channel;
		}while(channel < 0 || channel > 3);

		enc.SetChannel(channel);
		enc.Reset();

		cout << "Operation sucessful!" << endl;

		Menu3();
		break;

	case 2: //Read from a specific channel
		do
		{
			cout << "Which channel do you wish to read from? (0-3)" << endl;
			cin >> channel;
		}while(channel < 0 || channel > 3);

		enc.SetChannel(channel);
		enc.Reset();
		
		for(int i = 0 ; i < 0xFFFFFFFF ; i++)
		{
			enc.ReadCounter();
		}

		cout << "Operation sucessful!" << endl;

		Menu3();
		break;

	case 3: //Reset all channels
		!enc;

		cout << "Operation sucessful!" << endl;

		Menu3();
		break;

	case 4: //Calibrate encoder
		enc.Calibrate();

		cout << "A full rotation value is: " << enc.GetFullRotation() << endl;
		cout << "Operation sucessful!" << endl;

		Menu3();
		break;

	case 5: //Back to main menu
		MainMenu();
		break;
	}
}

