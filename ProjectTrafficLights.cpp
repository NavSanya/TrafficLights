/*Description: in this project, you will implement an intelligent
traffic light system where cars arrive at a "one-way only" tunnel.
Your program should enforce the requirement that "only at most N
cars in one direction can go through the tunnel at a time".
The goal of the project is to learn multi-threading with locks and
condition variables.

For implementation, you need to have variables for traffic lights
on both North and South directions. Lights can be RED or GREEN only.
If the light turns RED, no car should attempt to enter the tunnel.
Otherwise, N cars should be allowed to enter and leave the tunnel.
You may either have one thread per car, in this case, N threads need
to be woken up when the light turns GREEN. Or you may have one thread
per direction and then manage to make N cars leave the tunnel in a
sequence.

Cars should arrive at a random time with an average interval
of 60 seconds. To achieve this, you may do random*120, where
the random number is within (0,1). Or you can do rand()%121.
It should serve the same purpose.

Please take account of the time for cars to leave the tunnel,
e.g., 60 seconds, as well as the time for a car to start and
follow another car, e.g., 5 seconds. Such time can be implemented
through a sleep() function. Your program should calculate the total
time taken for M cars in both directions together to pass the tunnel.

Your program should display the following events on screen:

-Thread creation
-When light changes for one directions
-Periodic print out of time taken so far
There are two parameters to pass from the command line: M and N*/


#include <iostream>
//#include <bits/stdc++.h>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <chrono>
//#include "unistd.h"
#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>



using namespace std;

int total_cars; //N
int cars_to_pass_per_cycle;//M
mutex m_mutex;
condition_variable intersection;
vector<bool> ready;
vector<thread> stack;
boost::mutex mutex1, mutex2

void initialization(int tc, int ctppc)//initializing the global variables
{
	total_cars = tc;
	cars_to_pass_per_cycle = ctppc;
	ready.resize(total_cars);
}//initialization()

void main_thread_function_direction(bool traffic_light)
{
	while (traffic_light)//true =green and false = red
	{
		for (int i = cars_to_pass_per_cycle - 1; i >= 0; --i)
		{
			cout << "releasing thread" << i << endl;
			ready.at(i) = true;
			intersection.notify_one();
			this_thread::sleep_for(std::chrono::milliseconds(total_cars * 5));//the time for a car to start and follow another car is 5 seconds

		}//for loop
		traffic_light = 0;//RED Light
	}//while loop
}//main_thread_function_direction()

void thread_callback_direction(int x)
{
	unique_lock<mutex> mlock(m_mutex);
	cout << "Passed Number = " << x << endl;
	cout << "thread " << x << " waiting..." << endl;
	ready.at(x) = false;
	while (!ready.at(x))
		intersection.wait(mlock);
	cout << "thread " << x << " completed!" << endl;
}//thread_callback_direction()


int main(int argc, int argv[])
{
	int N = 10;//argv[0];
	int M = 5;//argv[1];
	initialization(N, M);

	bool trafficLight = true;//GREEN
	vector<thread> north;
	vector<thread> south;
	int n;
	for (n = N; n > 0; n = n - (2 * M))
	{
		this_thread::sleep_for(std::chrono::milliseconds(1));

		for (int i = 0; i < M; ++i)//cars going toward the north
		{
			this_thread::sleep_for(std::chrono::milliseconds(rand() % 121));
			north.push_back(thread(thread_callback_direction, i));
			south.push_back(thread(thread_callback_direction, i));
		}//for loop

		this_thread::sleep_for(std::chrono::milliseconds(1));

		

		/*for (int i = 1; i < M; ++i)//cars going toward the south
		{
			this_thread::sleep_for(std::chrono::milliseconds(rand() % 121));
			south.push_back(thread(thread_callback_direction, i));
		}//foor loop
		*/

		this_thread::sleep_for(std::chrono::milliseconds(1));

		cout << "\nSTART Cars going to North Direction" << endl;
		thread Thread1(main_thread_function_direction, trafficLight);

		Thread1.join();
		for (int i = M-1; i >= 0; --i)
		{
			this_thread::sleep_for(std::chrono::milliseconds(5));
			(north.at(i)).join();
			north.pop_back();
		}//for loop

		cout << "STOP Cars going to North Direction\n" << endl;

		this_thread::sleep_for(std::chrono::milliseconds(1));

		cout << "\nSTART Cars going to South Direction" << endl;
		thread Thread2(main_thread_function_direction, trafficLight);

		Thread2.join();
		for (int i = 0; i < M; ++i)
		{
			this_thread::sleep_for(std::chrono::milliseconds(5));
			(south.at(i)).join();
			south.pop_back();
		}//for loop

		cout << "STOP Cars going to South Direction\n" << endl;

	}//for n

	return 0;
}//main()