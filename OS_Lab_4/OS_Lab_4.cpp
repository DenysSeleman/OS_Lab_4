#include <iostream>
#include <mutex>
#include <thread>
#include <time.h>
using namespace std;
const int numOfPhils = 5;

struct Fork
{
    Fork() {}
    mutex mtx;
};
Fork forks[numOfPhils];

bool statusOfPhils[numOfPhils];
string printStatuses()
{
    string ans = "Statuses of philosophers: ";
    for (int i = 0; i < numOfPhils; i++)
    {
        if (statusOfPhils[i])
            ans += "eat ";
        else
            ans += "think ";
    }
    ans += "\n";
    return ans;
}

int randomNum(int minNum, int maxNum)
{
    return minNum + rand() % (maxNum - minNum + 1);
}

mutex mtxCout;

void eat(Fork& lFork, Fork& rFork, int philNum)
{
    unique_lock<mutex> lLock(lFork.mtx);
    unique_lock<mutex> rLock(rFork.mtx);

    statusOfPhils[philNum - 1] = true;
    mtxCout.lock();
    cout << "Philosopher " << philNum << " has started eating. " << printStatuses();
    mtxCout.unlock();

    this_thread::sleep_for(chrono::milliseconds(randomNum(800, 1300)));

    statusOfPhils[philNum - 1] = false;
    mtxCout.lock();
    cout << "Philosopher " << philNum << " has finished eating.\n";
    mtxCout.unlock();
}

void philProc(int philNum, int eatCount)
{
    while (eatCount > 0)
    {
        mtxCout.lock();
        cout << "Philosopher " << philNum << " is thinking now. " << printStatuses();
        mtxCout.unlock();

        this_thread::sleep_for(chrono::milliseconds(randomNum(1500, 2500)));

        if (philNum == numOfPhils)
            eat(forks[philNum - 1], forks[0], philNum);
        else
            eat(forks[philNum - 1], forks[philNum], philNum);

        eatCount--;
    }
}

int main()
{
    srand(time(NULL));
    int eatCount = 5;
    thread phils[numOfPhils];

    cout << "Dinner has started!\n";
    for (int i = 0; i < numOfPhils; i++)
    {
        phils[i] = thread(philProc, i + 1, eatCount);
        this_thread::sleep_for(chrono::milliseconds(1));
    }

    for (auto& phil : phils)
        phil.join();
    cout << "Dinner is over!\n";

    return 0;
}