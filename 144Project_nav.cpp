/*Description: in this project, you will implement an intelligent traffic light system where cars arrive at a "one-way only" tunnel.
Your program should enforce the requirement that "only at most N cars in one direction can go through the tunnel at a time".
The goal of the project is to learn multi - threading with locks and condition variables.

For implementation, you need to have variables for traffic lights on both Northand South directions.
Lights can be RED or GREEN only.If the light turns RED, no car should attempt to enter the tunnel.
Otherwise, N cars should be allowed to enterand leave the tunnel.You may either have one thread per car, in this case,
N threads need to be woken up when the light turns GREEN.Or you may have one thread per directionand then manage to make N cars leave 
the tunnel in a sequence.

Cars should arrive at a random time with an average interval of 60 seconds.To achieve this, you may do random * 120, 
where the random number is within(0, 1).Or you can do rand() % 121. It should serve the same purpose.

Please take account of the time for cars to leave the tunnel, e.g., 60 seconds, as well as the time for a car to startand follow another car,
e.g., 5 seconds.Such time can be implemented through a sleep() function.
Your program should calculate the total time taken for M cars in both directions together to pass the tunnel.

Your program should display the following events on screen :
Thread creation
When light changes for one directions
Periodic print out of time taken so far
There are two parameters to pass from the command line : M and N*/

#include <iostream>
//#include <bits/stdc++.h>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <queue>
#include <fstream>
//#include "unistd.h"
//#include "unistd.h"
#define MAX 40

using namespace std;

class Car
{
public:
    bool ready;
    int num;
    int arivTime;
    string dir;

    Car(int n, int at, string d)
    {
        num = n;
        arivTime = at;
        dir = d;
        ready = false;
    }//end constructor
};//end class

class CompareCars {
public:
    bool operator()(Car* const& p1, Car* const& p2)
    {
        return ((p1->arivTime) > (p2->arivTime));
    }//end operator
};//end class


bool trafficLight = true; //Green;
mutex m_mutex;
condition_variable cv;
priority_queue<Car*, vector<Car*>, CompareCars> North;
priority_queue<Car*, vector<Car*>, CompareCars> South;

priority_queue<Car*, vector<Car*>, CompareCars> activeLane;
vector<bool> ready(MAX); // holds all of the car threads
int releasecount = 0;



void threadCallback(int, int, string);//to call thread back

void releaseCar(Car* newCar)
{
    unique_lock<mutex> mlock(m_mutex);
    newCar->ready = true;
    cv.notify_one();
    this_thread::sleep_for(std::chrono::milliseconds(1));
}

void mainThreadFunction(int numAllowedCars)
{
    int count = 0;

    while (!North.empty() && !South.empty())
    {
        if (trafficLight) {
            Car* northCar = South.top();
            Car* southCar = North.top();
            releaseCar(northCar);
            releaseCar(southCar);

        }
    }
    trafficLight = !trafficLight;

}

void threadCallback(int vectorPos, int carArrival, string str)
{
    unique_lock<mutex> mlock(m_mutex);
    Car* newCar = new Car(vectorPos, carArrival, str);

    if (str[0] == 'N')
    {
        South.push(newCar);
        ready.at(vectorPos) = false;
        while (!(newCar->ready) || !trafficLight || South.top() != newCar)
        {
            cv.wait(mlock);
        }
        South.pop();
        activeLane.push(newCar);
        this_thread::sleep_for(std::chrono::milliseconds(200));
        cout << "Car left " << "id: " << newCar->arivTime << " " << "Direction " << newCar->dir << endl;
        activeLane.pop();
    }
    if (str[0] == 'S')
    {
        North.push(newCar);
        ready.at(vectorPos) = false;
        while (!(newCar->ready) || !trafficLight || North.top() != newCar)
        {
            cv.wait(mlock);
        }
        North.pop();
        activeLane.push(newCar);
        this_thread::sleep_for(std::chrono::milliseconds(100));
        cout << "Car left " << "id: " << newCar->arivTime << " " << "Direction " << newCar->dir << endl;
        activeLane.pop();

    }
}

/*int main()
{
    int num_cars = 20;
    int numAllowedCars = 10;
    vector<thread> carThreads;// car threads

    int threadCount = 0;
    string direction;
    time_t start = time(0);
    for (int i = 0; i < num_cars; ++i)
    {
        int arrivalTime = time(0);
        if ((rand() % 2) > 1)
        {
            direction = "N";
        }
        else {
            direction = "S";
        }
        carThreads.push_back(thread(threadCallback, threadCount, arrivalTime, direction));
        cout << "Car Thread Created: " << threadCount << endl;
        this_thread::sleep_for(std::chrono::milliseconds(1));
        threadCount++;
    }
    thread releaseCars(mainThreadFunction, numAllowedCars);    //create threads to call the release function

    releaseCars.join();

    double seconds_since_start = difftime(time(0), start);    //end the timer to get final time
    cout << "Total Time: " << seconds_since_start << "s" << endl;


    return 0;

}*/
int main()
{
    vector<thread> carThreads; // car threads
    int numAllowedCars;
    int threadCount = 0;
    string filename;
    string fileline;
    ifstream myfile;

    myfile.open("C:\\Users\\navsa\\OneDrive\\Desktop\\Nav\\bestest.txt");    //Insert file path here

    time_t start = time(0);
    while (getline(myfile, fileline))
    {
        size_t index = fileline.find(" ");
        string arrivalTime = fileline.substr(0, index);
        int tempArrivalTime = stoi(arrivalTime);
        string direction = fileline.substr(index + 1);
        carThreads.push_back(thread(threadCallback, threadCount, tempArrivalTime, direction));
        cout << "Car Thread Created: " << threadCount << endl;
        this_thread::sleep_for(std::chrono::milliseconds(1));
        threadCount++;

    }
    myfile.close();

    thread releaseCars(mainThreadFunction, numAllowedCars);    //create threads to call the release function

    releaseCars.join();

    double seconds_since_start = difftime(time(0), start);    //end the timer to get final time
    cout << "Total Time: " << seconds_since_start << "s" << endl;


    return 0;
}
