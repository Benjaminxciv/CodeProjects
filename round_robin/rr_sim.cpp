#include <iostream>
#include <iomanip>
#include <fstream>
#include <queue>
#include <list>

using namespace std;

/*
Class: PCB
Desc: An individual process object
*/
class pcb{

    private:
        int process_id;
        int arrival_time;
        int service_time;
        int tat;
        float ntat;
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
        void set_tat(int);
        int get_tat();
        void set_ntat();
        float get_ntat();

        
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
    this->tat = 0;
}

pcb::~pcb()
{

}


/*
Name:add_cpu_time
Desc: adds one sec to ecexuted cpu time for the current process
Param: N/A
*/
void pcb::add_cpu_time()
{
    this->cpu_time += 1;
    this->time_remaining -= 1;
}

//Get: Get the member variables of the pcb object
int pcb::get_cpu_time()
{
    return this->cpu_time;
}

//Get: Get the member variables of the pcb object
int pcb::get_service_time()
{
    return this->service_time;
}

//Get: Get the member variables of the pcb object
int pcb::get_pid()
{
    return this->process_id;
}


/*
Name: increment_pro_class()
Desc: increments the level of classification up to level 3: Coerelates to
        how many quanta the process took to complete.
Param: N/A
*/
void pcb::increment_pro_class()
{
    if(this->process_class != 3)
        this->process_class += 1;
}

//Set: Set the member variables of the pcb object
void pcb::set_pro_class(string pro_class)
{
    this->pro_class_word = pro_class;
}

//Get: Get the member variables of the pcb object
int pcb::get_pro_class()
{
    return this->process_class;
}

//Get: Get the member variables of the pcb object
string pcb::get_class_word()
{
    return this->pro_class_word;
}

//Get: Get the member variables of the pcb object
int pcb::get_time_remaining()
{
    return this->time_remaining;
}

//Get: Get the member variables of the pcb object
int pcb::get_arrival_time()
{
    return this->arrival_time;
}

//Set: Set the member variables of the pcb object
void pcb::set_tat(int clock)
{
    this->tat = clock - this->tat;
    set_ntat();
}

//Get: Get the member variables of the pcb object
int pcb::get_tat()
{
    return this->tat;
}

//Set: Set the member variables of the pcb object
void pcb::set_ntat()
{
    ntat = float(tat) / float(service_time);
}

//Get: Get the member variables of the pcb object
float pcb::get_ntat()
{
    return this->ntat;
}


/*
Class: simulation
Desc: runs the simulation of the round robin queue and 
        collects statistics to be outputed to a stats file
*/
class simulation
{
    private:
        int clock;
        queue <pcb*> process_queue;
        int process_count;
        pcb* curr_pro;
        list <pcb*> short_pcb_list;
        list <pcb*> med_pcb_list;
        list <pcb*> long_pcb_list;

    public:
        void init_queue(int&, int&);
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
    this->clock         = 0;
}


/*
Name: init_queue()
Desc: Inititalizes the process queue from data read from the input data file 
Param: int& quantum_time1, int quantum_time2
*/
void simulation::init_queue(int& quantum_time1, int& quantum_time2) 
{
    ifstream input("HWK4input.txt", ios::in);
    input >> quantum_time1;
    input >> quantum_time2;
    
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


/*
Name: run_processes() 
Desc: executes the processes in the proccess queue 
        follows round robin procedures
Param: int quantum_time
*/
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
            curr_pro->set_tat(clock);
            determine_class();

            if(finish_count < 15)
            {
                finish_count++;
                print_individual_stats();
            }   
        }
    }
}

/*
Name: determine_class() 
Desc: determines classification of how many quanta the process
    took to finsh. Adds the pcb to a list of the same type of classification
Param: N/a
*/
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

/*
Name: print_header()
Desc: prints a header in the output txt file for individual process stats
Param: int quantum
*/
void simulation::print_header(int quantum)
{
    ofstream out_file;
    out_file.open("ResultOutput.txt", ios::app);
    out_file << "\n\nResults for first 15 processes to finish: quantum = " << quantum << "\n\n";
    out_file << "PID " << " Arrival-Time " << " Service-Time " << " Departure-Time " 
             << " TAT " << "   NTAT " << "   Class "<< endl;
}

/*
Name: print_individual_stats();
Desc: prints statistics for an individual pcb to an output file
Param: N/a
*/
void simulation::print_individual_stats()
{
    ofstream out_file;
    out_file.open("ResultOutput.txt", ios::app);
    
    int arrival_time = curr_pro->get_arrival_time();
    int tat = curr_pro->get_tat();

    out_file << setw(2) << curr_pro->get_pid() << setw(8) << arrival_time << setw(15) << curr_pro->get_service_time() 
                    << setw(15) << clock << setw(12) << tat << setw(7) << setw(8) << fixed 
                    << setprecision(2) << curr_pro->get_ntat() << setw(9) << curr_pro->get_class_word() << endl;
}


/*
Name: print_summary_stats();
Desc: prints summary statistics to output file for each list: short list, med list, long list
        also outputs the overall systems stats
Param: N/a
*/
void simulation::print_summary_stats(int quantum)
{

    int small_tat_sum   = 0;
    int small_ntat_sum  = 0;
    for (auto const& process : short_pcb_list) 
    {
        small_tat_sum += process->get_tat();
        small_ntat_sum += process->get_ntat();

    }
    int tat_avg_short = small_tat_sum / short_pcb_list.size();
    int ntat_avg_short = small_ntat_sum / short_pcb_list.size();

    int med_tat_sum = 0;
    int med_ntat_sum  = 0;
    for (auto const& process : med_pcb_list) 
    {
        med_tat_sum += process->get_tat();
        med_ntat_sum += process->get_ntat();
    }
    int tat_avg_med = med_tat_sum / med_pcb_list.size();
    int ntat_avg_med = med_ntat_sum / med_pcb_list.size();

    int long_tat_sum  = 0;
    int long_ntat_sum  = 0;
    for (auto const& process : long_pcb_list) 
    {
        long_tat_sum += process->get_tat();
        med_ntat_sum += process->get_ntat();
    }
    int tat_avg_long = long_tat_sum / long_pcb_list.size();
    int ntat_avg_long = med_ntat_sum / long_pcb_list.size();

    int system_tat_sum = small_tat_sum + med_tat_sum + long_tat_sum;
    int system_tat_avg = system_tat_sum / process_count;

    int system_ntat_sum = small_ntat_sum + med_ntat_sum + long_ntat_sum;
    int system_ntat_avg = system_ntat_sum / process_count;

    ofstream out_file;
    out_file.open("ResultOutput.txt", ios::app);
    out_file << "\n\n\nSUMMARY STATISTICS FOR CURRENT DATA SET.\n";
    out_file <<"Quantum = " << quantum << endl;
    out_file << "Number of clock ticks: " << clock << endl << endl;
    out_file << setw(15) <<"SHORT" << setw(7) <<"MEDIUM" << setw(5) << "LONG" << setw(8) << "SYSTEM\n"; 
    out_file << "Count:    " <<setw(4) << short_pcb_list.size() << setw(6) << med_pcb_list.size() << setw(7) 
         << long_pcb_list.size() << setw(7) << process_count << endl;
    out_file << "TAT AVG: " << setw(5) << tat_avg_short << setw(6) << tat_avg_med << setw(7) 
         <<tat_avg_long << setw(7) <<system_tat_avg <<endl;
    out_file << "NTAT AVG: " << setw(4) << ntat_avg_short << setw(6) << ntat_avg_med
         << setw(7) << ntat_avg_long << setw(7) << system_ntat_avg << endl << endl;
}

int main(){

    cout << " Round Robin Simulation. " << endl;
    int quantum_time1;
    int quantum_time2;
    
    simulation* sim1 = new simulation();
    sim1->init_queue(quantum_time1, quantum_time2);
    sim1->print_header(quantum_time1);
    sim1->run_processes(quantum_time1);
    sim1->print_summary_stats(quantum_time1);

    simulation* sim2 = new simulation();
    sim2->init_queue(quantum_time1, quantum_time2);
    sim2->print_header(quantum_time2);
    sim2->run_processes(quantum_time2);
    sim2->print_summary_stats(quantum_time2);
    
    cout << "Simulation complete. Checkoutput files for results" << endl;
    return 0;
}
