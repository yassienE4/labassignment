#include <iostream>
#include <queue>
#include <vector>
#include <time.h>
#include <iomanip>
#include <chrono>
#include <thread>
using namespace std;

struct timeformat
{
    int hour;
    int min;
    timeformat() : hour(0), min(0) {};
    timeformat(int h, int m) : hour(h), min(m) {};
    void display()
    {
        cout << setw(2) << setfill('0') << hour << ":" << setw(2) << setfill('0') << min;
    }
    int toMinutes() const
    {
            return (hour * 60) + min; // Convert hours to minutes and add minutes
    }
};
class allpatients
{
    private:
        enum Type
        {
            Urgent = 1,
            Normal = 0,
        };
        struct patient
        {
            string id; // integer limit doesnt allow for 14 digits
            char gender;
            timeformat time;
            Type type;
            int remainingServeTime = rand() % 10 + 1; //Time it takes to serve patient (cannot be more than 10)
        };
        void gotourgent(patient p);
        void gotonormal(patient p);
        int index;
        vector<patient> allpatient;
        queue<patient> urgent;
        queue<patient> normal;
        string createrandid()
        {
            int temp1 = rand() % 9 + 1;
            string result = to_string(temp1);
            for (int i = 0; i < 13; ++i) 
            {
                int digit = rand() % 10;
                result += to_string(digit);
            }
            return result;
        }
        patient returnpatient(vector<patient> & vec)
        {
            for(int i = 0; i< vec.size(); i++)
            {
                if(vec[i].type)
                {
                    cout << "Urgent" << endl;
                    patient returntemp = vec[i];
                    vec.erase(vec.begin() + i);
                    return returntemp;
                }
            }
            for(int i = 0; i< vec.size(); i++)
            {
                cout << "Normal" << endl;
                patient returntemp = vec[i];
                vec.erase(vec.begin() + i);
                return returntemp;
            }
        }
    public: 
        allpatients()
        {
            patient temp;
            srand(time(NULL));
            index = 0;
            int size;
            
            cout<< "How many people are waiting to be served?" << endl;
            cin>>size;
            for(int i = 0; i< size; i++)
            {
                temp.id = createrandid();
                temp.gender = (rand()%2 == 0) ? 'M' : 'F';
                temp.time.hour = (rand()%24);
                temp.time.min = (rand()%60);
                temp.type = (rand()%2==0) ? Urgent : Normal;
                allpatient.push_back(temp);
                index++;
            }
            
        }
        void dispatchpatients(vector<patient>)
        {
            while(!allpatient.empty())
            {

                patient temp;
                temp = returnpatient(allpatient);
                if(temp.type == 1)
                {
                    urgent.push(temp);
                    //needs a function call otherwise wil fill queues first
                }
                    
                else
                {
                    normal.push(temp);      //same here
                }
                    


//                std::this_thread::sleep_for(std::chrono::minutes(1)); //sleep for one min
                
            }
        }
    
    bool processRemainingServeTime(patient& p, int currentMin) //function used to decide if patient should be popped from queue or is still being served
    {
        // Decrease the remaining serve time
        if (p.remainingServeTime > 0)
        {
            p.remainingServeTime--;
            cout << "Serving patient - ID: " << p.id << ", Type: " << (p.type == 1 ? "Urgent" : "Normal") << ", Remaining Serve Time: " << p.remainingServeTime << " minutes." << endl;
            return false;
        }
        
        // If the patient is done being served
        if (p.remainingServeTime == 0)
        {
            cout << "Finished serving patient - ID: " << p.id << ", Type: " << (p.type == 1 ? "Urgent" : "Normal") << ", Arrival Time: ";
            p.arrivalTime.display();
            cout << ", Waiting Time: " << (currentMin - p.arrivalTime.toMinutes()) << " minutes." << endl;
            return true;
        }
        return false;
    }
    
    void servePatients()
    {
        int currentMinute = 0; // current minute in the 10 minute time step
        int waitingTime;
        int totalServedPatients = 0;
        int totalWaitingTime = 0;
        int nextminute;
        
        while (!urgent.empty() || !normal.empty())
        {
            int N = rand() % 6 + 5;// Randomly generate N between 5 and 10(patients to be served this minute
            int servedCount = 0; // Counter for patients served in this minute
            
            cout << "Time: " << currentMinute << " minutes" << endl;
            cout << "Serving up to " << N << " patients in this time step." << endl;
            
            while ((servedCount < N) && (!urgent.empty() || !normal.empty())) //makes sure to not serve more than N patients at a given minute and that both queues are not empty
            {
                patient p;
                
                // Serving urgent queue first
                if (!urgent.empty())
                {
                    p = urgent.front(); //p is now patient at front of queue
                   if(processRemainingServeTime(p, currentMinute))
                   {
                       urgent.pop(); //only pops if remaining time is 0, otherwise keeps patient at front of Queue in order to decrement remaining time each minute
                   } //after pop it moves on to next patient in queue
                }
                // Serve from the normal queue if urgent queue is empty
                else if (!normal.empty())
                {
                    p = normal.front(); //p is now patient at front of queue
                    if(processRemainingServeTime(p, currentMinute))
                    {
                        normal.pop(); //only pops if remaining time is 0, otherwise keeps patient at front of Queue in order to decrement remaining time each minute
                    } //after pop it moves on to next patient in queue
                }
                else
                {
                    break; // both queues are empty no patients left, breaks second loop and won't enter first loop since they are both empty
                }
                
                // Calculate waiting time PROBABLY WRONG
                int serveTime = p.arrivalTime.toMinutes() + currentMinute; //this just returns current minute which is why its probably wrong
                waitingTime = serveTime - p.arrivalTime.toMinutes();      //this just returns current minute which is why its probably wrong
                
                // Update stats
                totalServedPatients++; //increments total served patients (should probably put this after the pop since it increments every instance of the loop even if p does not change
                totalWaitingTime= totalWaitingTime + waitingTime; //totals waiting time to find average later
                                
                servedCount++; //increments served count so if it is more than N it exits the loop after this iteration
                
                cout << "press 1 to move on to next minute or press anything else to end " << endl; //Timing in the system is advacnced with each enter not the actual computer time
                cin >> nextminute;
                if (nextminute == 1)
                {
                    currentMinute++;
                }
                else
                {
                    cout << "end"<< endl; //can probably find a better behavior for this
                    break;
                }
                
            }
            
        }
            cout << "Simulation Summary: "<< endl;
                cout << "Total Served Patients: " << totalServedPatients << endl;
                if (totalServedPatients > 0)
                {
                    cout << "Average Waiting Time: " << (totalWaitingTime / totalServedPatients) << " minutes" << endl;
                
                }
        // Print remaining patients in queues, only if loop breaks early 
           cout << "Remaining Urgent Patients: " << urgent.size() << endl;
           cout << "Remaining Normal Patients: " << normal.size() << endl;
    }
    
    
        void print() // debugging
        {
            for(int i =0; i<10;i++)
            {
                cout << allpatient[i].gender << endl;
                cout << allpatient[i].id << endl;
                cout << allpatient[i].type << endl;
                allpatient[i].time.display();
                cout <<endl;
            }
        }
        void printqueue() //testing
        {
            dispatchpatients(allpatient);
            patient temp = urgent.front();
            cout << temp.gender;
            cout << urgent.front().id;
        }
        
        


    
};

int main()
{
    allpatients test;


    //test.print();
    test.printqueue();
    
}
