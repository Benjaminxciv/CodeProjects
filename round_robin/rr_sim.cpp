#include <iostream>
#include <iomanip>
#include <fstream>
#include <queue>
#include <list>

using namespace std;

class pcb{

    private:
        int process_id;
        int arrival_time;
        int service_time;
        int time_remaining;
        int departure_time;
        int cpu_time;
        int process_class;
        string pro_class_word;

    public:
        void add_cpu_time();
        int get_cpu_time();
        int get_service_time();
        int get_pid();
        int decrement_service_time();
        void increment_pro_class();
        void set_pro_class(string);
        int get_pro_class();
        string get_class_word();
        int get_time_remaining();
        int get_arrival_time();

        
        pcb(int,int);
        ~pcb();
};

pcb::pcb(int id, int service_time)
{
    this->process_id    = id;
    this->service_time  = service_time;
    this->arrival_time  = 0;
    this->cpu_time      = 0;
    this->time_remaining = service_time;
    this->process_class = 1;
}

pcb::~pcb()
{

}

void pcb::add_cpu_time()
{
    this->cpu_time += 1;
    this->time_remaining -= 1;
}

int pcb::get_cpu_time()
{
    return this->cpu_time;
}

int pcb::get_service_time()
{
    return this->service_time;
}

int pcb::get_pid()
{
    return this->process_id;
}

void pcb::increment_pro_class()
{
    if(this->process_class != 3)
        this->process_class += 1;
}

void pcb::set_pro_class(string pro_class)
{
    this->pro_class_word = pro_class;
}
int pcb::get_pro_class()
{
    return this->process_class;
}

string pcb::get_class_word()
{
    return this->pro_class_word;
}

int pcb::get_time_remaining()
{
    return this->time_remaining;
}

int pcb::get_arrival_time()
{
    return this->arrival_time;
}

using namespace std;

class simulation
{
    private:
        int clock;
        queue <pcb*> process_queue;
        int quantum_time1;
        int quantum_time2;
        int process_count;
        pcb* curr_pro;
        list <pcb*> short_pcb_list;
        list <pcb*> med_pcb_list;
        list <pcb*> long_pcb_list;

    public:
        void init_queue();
        void run_processes(int);
        void print_header(int);
        void print_individual_stats();
        void print_summary_stats(int);
        void determine_class();
        simulation();
};

simulation::simulation()
{
    this->process_count = 0;
    this->quantum_time1 = 0;
    this->quantum_time2 = 0;
    this->clock         = 0;
}

void simulation::init_queue() 
{
    ifstream input("HWK4input.txt", ios::in);
    input >> this->quantum_time1;
    input >> this->quantum_time2;
    
    string line;
    int service_time = 0;

     while(getline(input, line))
     {
        service_time = atoi(line.c_str());
        
        if(service_time != 0)
        {
            this->process_count++;     
            curr_pro = new pcb(process_count,service_time);
            process_queue.push(curr_pro);  
        }    
     }

     line = "";
     input.close();
}


void simulation::run_processes(int quantum_time)
{
    int finish_count = 0;
    int exe_time = 0;

    while(process_queue.size() > 0 )
    {
        curr_pro = process_queue.front();
        process_queue.pop();
        exe_time = 0;

        while(exe_time < quantum_time && curr_pro->get_time_remaining() > 0)
        {
            exe_time++;
            this->clock++;
            curr_pro->add_cpu_time();
        }

        if(curr_pro->get_time_remaining() != 0)
        {
            process_queue.push(curr_pro);
            curr_pro->increment_pro_class();
        }

        else
        {
           determine_class();
            
            if(finish_count < 15)
            {
                finish_count++;
                print_individual_stats();
            }   
        }
    }
}

void simulation::determine_class()
{
    int pro_class = curr_pro->get_pro_class();

    if(pro_class == 1)
    {
        curr_pro->set_pro_class("short");
        short_pcb_list.push_front(curr_pro);
    }
            
    else if(pro_class == 2)
    {
        curr_pro->set_pro_class("medium");
        med_pcb_list.push_front(curr_pro);
    }
         
    else 
    {
        curr_pro->set_pro_class("long");
        long_pcb_list.push_front(curr_pro);
    }
}

void simulation::print_header(int quantum)
{
    ofstream out_file;
    out_file.open("first_fifteen.txt", ios::app);
    out_file << "\n\n\nResults for first 15 processes to finish: quantum = " << quantum << "\n\n";
    out_file << "PID " << " Arrival-Time " << " Service-Time " << " Departure-Time " 
             << " TAT " << "   NTAT " << "   Class "<< endl;
}

void simulation::print_individual_stats()
{
    ofstream out_file;
    out_file.open("first_fifteen.txt", ios::app);
    
    int arrival_time = curr_pro->get_arrival_time();
    int tat = clock - arrival_time;
    float ntat = float(tat) / float(curr_pro->get_service_time());
    
    out_file << setw(2) << curr_pro->get_pid() << setw(8) << arrival_time << setw(15) << curr_pro->get_service_time() 
                    << setw(15) << clock << setw(12) << tat << setw(7) << setw(8) << fixed 
                    << setprecision(2) << ntat << setw(9) << curr_pro->get_class_word() << endl;
}

void simulation::print_summary_stats(int quantum)
{
    // ofstream out_file;
    // out_file.open("first_fifteen.txt", ios::app);
    cout << "\nSUMMARY STATISTICCS FOR CURRENT DATA SET.\n";
    cout <<"Quantum = " << quantum << endl;
    cout << "\nSHORT" << setw(7) <<"MEDIUM" << setw(5) << "LONG" << setw(8) << "SYSTEM\n"; 
    cout << setw(3) << short_pcb_list.size() << setw(6) << med_pcb_list.size() << setw(7) 
         << long_pcb_list.size() << setw(6) << process_count << endl;
    cout << "Clock: " << clock << "\nNumber of processes: " << process_count << endl;
}

int main(){

    //int quantum_time1;
    //int quantum_time2;
    
    simulation* sim = new simulation();
    sim->init_queue();
    sim->print_header(5);
    sim->run_processes(5);
    sim->print_summary_stats(5);

    return 0;
}
