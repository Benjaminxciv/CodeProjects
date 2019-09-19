#include <iostream>
using namespace std;
int main(){

int height = 15;
int width = 20;

cout << "     " ;
for(int i = 1; i < width+1; i++)
{
    cout.fill('0');
    cout.width(3);
    cout << i << " ";
}
    cout << endl;

for (int j = 0; j < height+1; j++)
    {   
        cout.fill('0');
        cout.width(3);
        cout << j;

        for(int k = 0; k < width;k++)
        {
            cout.fill(' ');
            cout.width(4);
           get cout << "*";
        }
        cout << endl;
    }


    return 0;
}