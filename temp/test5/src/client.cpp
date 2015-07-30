/*************************************************************************
 > File Name: client.cpp
 > Author: zhushh
 > Mail: 
 > Created Time: Tue 21 Jul 2015 11:48:16 AM CST
 ************************************************************************/

#include <iostream>
using namespace std;

//#include "AGsocket.h"
#include "../headers/AgendaUI.h"

int main() {
	AgendaUI aui;
	aui.OperationLoop();
    return 0;
}
