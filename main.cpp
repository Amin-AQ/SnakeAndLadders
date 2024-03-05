#include<iostream>
using namespace std;
#include"Engine.h"
int main()
{
	srand(time(0));
	SnakeAndLadders sal;
	sal.GamePlay();
	return 0;
}