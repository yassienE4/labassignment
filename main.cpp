#include <iostream>
#include <queue>
#include <vector>
#include <time.h>
using namespace std;



class allpatients
{
    private:
        struct patient
        {
            int id;
            char gender;
            string time;
            bool urgent;
        };
        void gotourgent();
        void gotonormal();
        int index;
        vector<patient> allpatient;
        queue<patient> urgent;
        queue<patient> normal;
    public: 
        allpatients()
        {
            patient temp;
            for(int i = 0; i< 100; i++)
            {
                srand(time(NULL));
                temp.id = rand()%10000
                temp.gender = 

            }
            
        }
        void gotoqueue(vector<patient>)
        {

        }
        


    
};

int main()
{
    
}