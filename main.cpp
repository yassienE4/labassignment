#include <iostream>
#include <queue>
#include <vector>
#include <time.h>
#include <iomanip>
#include <stdexcept>
using namespace std;



struct timeformat
{
    int hour;
    int min;
    timeformat() : hour(0), min(0) {};
    timeformat(int h, int m) : hour(h), min(m) {};
    void display() const
    {
        cout << setw(2) << setfill('0') << hour << ":" << setw(2) << setfill('0') << min;
    }
    int toMinutes()
    {
        return( (hour *60) + min);
    }
    timeformat toformat(int x)
    {
        int hours = x / 60;
        int mins = x % 60;
        timeformat temp(hours,mins);
        return temp;
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
        };
    timeformat currenttime;
        int currentTimeStep = 0;
        int index;
        vector<patient> allpatient;
        queue<patient> urgent;
        queue<patient> normal;
        string createrandid()
        {
            int temp1 = rand() % 9 + 1;
            string result = to_string(temp1);
            try {
                for (int i = 0; i < 13; ++i)
                {
                    int digit = rand() % 10;
                    result += to_string(digit);
                }
                if (result.length() != 14) // Shouldn't happen with the current logic
                    {
                        throw out_of_range("National ID is out of range: " + result);
                    }
            } catch (const exception& e) {
                cout << e.what() <<endl;
            }
            
            return result;
        }
        patient returnpatient(vector<patient> & vec)
        {
            for(int i = 0; i< vec.size(); i++)
            {
                if(vec[i].type)
                {
                    //cout << "Urgent" << endl; //debugging
                    patient returntemp = vec[i];
                    vec.erase(vec.begin() + i);
                    return returntemp;
                }
            }
            for(int j = 0; j< vec.size(); j++)
            {
                //cout << "Normal" << endl;
                patient returntemp = vec[j];
                vec.erase(vec.begin() + j);
                return returntemp;
            }
            patient x; //
            return x;
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
        timeformat add(timeformat x, int y)
        {
            return(x.toformat(x.toMinutes() + y));
        }
        
    void dispatchpatients()
    {
        vector<patient>::iterator it = allpatient.begin();

        while (it != allpatient.end()) // Traverse through all patients
        {
            if (it->time.toMinutes() <= currenttime.toMinutes()) // Check if patient can be dispatched
            {
                if (it->type == Urgent)
                {
                    urgent.push(*it); // Add to urgent queue
                }
                else
                {
                    normal.push(*it); // Add to normal queue
                }
                it = allpatient.erase(it); // Remove dispatched patient from allpatient
            }
            else
            {
                ++it; // Move to the next patient
            }
        }
    }
    
    void servePatients()
    { //current time variable is declared in the begining as 00:00
        int waitingTime;
        int totalServedPatients = 0;
        int totalWaitingTime = 0;
        int nextTimestep;

        while (!urgent.empty() || !normal.empty() || !allpatient.empty()) // Check all queues and pending patients
        {
            dispatchpatients(); // Add eligible patients to queues before serving

            int N = rand() % 6 + 5; // Randomly generate N between 5 and 10
            int servedCount = 0;

            cout << "\nCurrent Time: ";
            currenttime.display();
            cout << " | Serving up to " << N << " patients in this time step.\n";

            while (servedCount < N && (!urgent.empty() || !normal.empty()))
            {
                patient p;

                // Serve urgent queue first
                if (!urgent.empty())
                {
                    p = urgent.front();
                    urgent.pop();
                }
                // Serve normal queue if urgent queue is empty
                else if (!normal.empty())
                {
                    p = normal.front();
                    normal.pop();
                }
                else
                {
                    break; // Both queues are empty
                }

                // Calculate waiting time
                waitingTime = (currenttime.toMinutes() + currentTimeStep) - p.time.toMinutes();
                totalServedPatients++;
                totalWaitingTime += waitingTime;

                servedCount++;
                cout << "Patient ID: " << p.id << " | Waiting Time: " << waitingTime << " minutes.\n";
            }

            cout << "Simulation Summary:\n";
            cout << "Total Served Patients: " << totalServedPatients << "\n";
            if (totalServedPatients > 0)
            {
                cout << "Average Waiting Time: " << (totalWaitingTime / totalServedPatients) << " minutes\n";
                cout << "Remaining Urgent Patients: " << urgent.size() << "\n";
                while (!urgent.empty())
                {
                    cout << urgent.front().id << " ";
                    urgent.pop();
                }
                cout << "Remaining Normal Patients: " << normal.size() << "\n";
                while (!normal.empty())
                {
                    cout << normal.front().id << " ";
                    normal.pop();
                }
            }

            cout << "Press 1 to move on to the next 10-minute increment or press anything else to end: ";
            cin >> nextTimestep;

            if (nextTimestep == 1)
            {
                currenttime = add(currenttime, 10); // Increment current time by 10 minutes
            }
            else
            {
                cout << "Program ended.\n";
                break;
            }
        }

        if (!allpatient.empty())
        {
            cout << "Remaining patients not yet eligible for dispatch:\n";
            for (const auto& p : allpatient)
            {
                cout << "Patient ID: " << p.id << " | Arrival Time: ";
                p.time.display();
                cout << "\n";
            }
        }
    }
           
};
    
int main()
{
    allpatients test; // generates patient
    test.servePatients();
    
}
