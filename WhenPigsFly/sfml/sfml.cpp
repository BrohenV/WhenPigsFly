// 
//  Assignment:     PROG1266 Assignment xxx
//  Instructor:     David Burchill
//  Year / Term:    Spring 2023
//  File name:      sfml.cpp
// 
//  Student name:   Brohen Verhoeven
//  Student email:  bverhoeven01@mynbcc.ca
// 
//     I certify that this work is my work only, any work copied from Stack Overflow, textbooks, 
//     or elsewhere is properly cited. 
// 
//	sfml.cpp : This file contains the 'main' function. Program execution begins and ends there.
//




#include <iostream>
#include <SFML/Graphics.hpp>
#include <iomanip>
#include "GameEngine.h"

int main()
{
	std::cout << system("cd") << "\n";
	GameEngine game("../assets.txt");
	game.run();
}