#pragma once
#include <chrono>
#include <iostream>
#include <thread>

using namespace std;

namespace general
{
	// entirely supericial function for more movie-esque console output
	void beepBoopType(string output, bool newline = false, int charDelay = 4)
	{
		int commaDelay = 200;
		int periodDelay = 300;
		size_t length = output.size();
		for (int i = 0; i < length+1; i++) 
		{
			cout << output.substr(i, 1);
			this_thread::sleep_for(chrono::milliseconds(charDelay));

			if (output.substr(i, 1) == ",")
			{
				this_thread::sleep_for(chrono::milliseconds(commaDelay));
			}
			if (output.substr(i, 1) == ".")
			{
				this_thread::sleep_for(chrono::milliseconds(periodDelay));
			}		
		}
		if (newline)
		{
			cout << "\n";
		}
	}
}