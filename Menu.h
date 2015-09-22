/*
	*
	Author: Diogo Guilherme Garcia de Freitas
	*
*/

#ifndef MENU_H
#define MENU_H
#include "Encoder.h"
#include "PC104.h"
#include "PCM3718.h"

using namespace std;

class Menu
{
private:
	int Option;
	PCM3718 pcm3718;
	PC104 pc104;
	Encoder enc;

public:
	Menu();
	void MainMenu();
	void Menu1();
	void Menu2();
	void Menu3();
};

#endif
