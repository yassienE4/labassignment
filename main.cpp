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
            for(int i = 0; i< 100; i++)
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
                    normal.push(temp);
                    //same here
                }
                    


                std::this_thread::sleep_for(std::chrono::minutes(1)); //sleep for one min
                
                
            }
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