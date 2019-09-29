#include <iostream>
#include <string>
#include <iomanip>

void print_grid(int,int);

int main(){

    print_grid(110,100);
    
    return 0;
}

void print_grid(int x_size, int y_size)
{   
    std::string column_space = "    ";    
    int counter = 0;
    std::cout << column_space;
    for(int x = 0; x < x_size; x++)
        {
            std::cout << counter;
            if((x+1) % 100 == 0)
            {
                counter++;
            }
        }
    std::cout << std::endl << column_space;

    counter = 0;
    for(int x = 0; x < x_size; x++)
    {
        if((x+1) % 10 == 0)
        {  
                counter ++;  
            if(counter > 9)
            {
                counter = 0;
            }
        }
        std::cout << counter;
    }

    std::cout << std::endl << column_space;
    counter = 0;
    for(int x = 1; x < x_size+1; x++)
    {
        counter++;
        if (x % 10 == 0)
        {
            counter = 0;
        } 
        std::cout << counter;
    }
    std::cout << std::endl;

    for(int y = 0; y < y_size; y++)
    {
            std::cout.width(3);
            std::cout.fill('0');
            std::cout<<y<<std::endl;
    }
 }
