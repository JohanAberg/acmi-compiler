
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <tchar.h>
#include "AcmiTape.h"
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <time.h>
#include <algorithm>
#include <atomic>

#include <string>
#include <vector>

#include <atomic>
#include <thread>
#include <future>

#include "threading.h"
#pragma warning(disable:4996)


template <class F>
void par_for(int begin, int end, F fn) {
	std::atomic<int> idx;
	idx = begin;

	int num_cpus = std::thread::hardware_concurrency();
	std::vector<std::future<void>> futures(num_cpus);

	for (int cpu = 0; cpu != num_cpus; ++cpu) {
		futures[cpu] = std::async(
			std::launch::async,
			[cpu, &idx, end, &fn]() {
			for (;;) {
				int i = idx++;
				if (i >= end) break;
				fn(i, cpu);
			}
		}
		);
	}
	for (int cpu = 0; cpu != num_cpus; ++cpu) {
		futures[cpu].get();
	}
}


int main()
{
	std::mutex mtx;

	std::cout << std::thread::hardware_concurrency() << std::endl;
	int i = 0;

	par_for(
		0, 20,
		[&](int idx, int cpu) {

			{
				auto lock = std::unique_lock<std::mutex>(mtx); // equivalent to mtx.lock();
				//... //protected stuff
				printf("task %d running on cpu %d\n", idx, cpu);
				printf("i : %d\n", i++);
			}  // <---- however you leave this brace, equivalent to mtx.unlock();
		
		//printf("task %d running on cpu %d\n", idx, cpu);
		//Sleep(3);
	//	std::cout << "i:" << i << std::endl;
	//	i++;

	}
	);


	system("pause");
	exit(0);









	FILE *fp;
	int y;
	char fname[100];
	char fltname[MAX_PATH];
	HANDLE findHand;
	WIN32_FIND_DATA fData;
	BOOL foundAFile = TRUE;

	// look for *.flt files to import
	findHand = FindFirstFile("D:\\tmp\\acmi*.flt", &fData);
	
	std::cout << "Hello World" << std::endl;
	
	// find anything?
	if (findHand == INVALID_HANDLE_VALUE)
		return -42;

	while (foundAFile)
	{
		strcpy(fltname, "D:\\tmp\\");
		strcat(fltname, fData.cFileName);

		std::cout << "Hello World" << std::endl;
		
		// find a suitable name to import to
		for (y = 1; y < 10000; y++)
		{
			sprintf(fname, "D:\\tmp\\TAPE%04d.vhs", y);

			fp = fopen(fname, "r");
			if (!fp)
			{
				clock_t t;
				t = clock();

				ACMITape::Import(fltname, fname);

				t = clock() - t;
				printf("It took me %d clicks (%f seconds).\n", t, ((float)t) / CLOCKS_PER_SEC);
				break;
			}
			else
			{
				fclose(fp);
			}
		}
		//exit(0); 
		system("pause");
		
		// get next file
		foundAFile = FindNextFile(findHand, &fData);
	}

	FindClose(findHand);
	return (0);
}

