#include <iostream>
#include <string>
using namespace std;


int main(){

    int height = 750;
    int width = 1000;

    std::string column_space =  "     ";
    std::cout << column_space;

    for(int i = 1; i < (width/25)+1; i++)
    {
        cout.fill('0');
        cout.width(3);
        cout << i*25 << " ";
    }
    cout << endl;

    for (int j = 0; j < (height/25)+1; j++)
    {   
        cout.fill('0');
        cout.width(3);
        cout << j*25;

        for(int k = 0; k < (width/25);k++)
        {
            cout.fill(' ');
            cout.width(4);
            cout << "*";
        }
            cout << endl;
    }
    return 0;
}