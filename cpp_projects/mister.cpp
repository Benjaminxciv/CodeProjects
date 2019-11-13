#include <iostream>
#include <fstream>
#include <queue>

class pcb{

    private:
        int process_id;
        int arrival_time;
        int service_time;
        int cpu_time;
        int process_class;

    
    public:
        void add_cpu_time();
        int get_cpu_time();
        int get_service_time();
        int get_pid();
        int decrement_service_time();
        void increment_pro_class();
        int get_pro_class();
        
        pcb(int,int);
        ~pcb();
};

pcb::pcb(int id, int service_time)
{
    this->process_id    = id;
    this->service_time  = service_time;
    this->arrival_time  = 0;
    this->cpu_time      = 0;
    this->process_class = 1;
}

void pcb::add_cpu_time()
{
    this->cpu_time += 1;
    this->service_time -= 1;
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

int pcb::get_pro_class()
{
    return this->process_class;
}

using namespace std;

//Function Prototypes
void init_queue(queue<pcb*>&, int&, int&);
void run_processes(queue<pcb*>&, int quantum_time, int& clock);

int main(){

    int quantum_time1;
    int quantum_time2;
    
    int clock = 0;
    queue <pcb*> process_queue;
    init_queue(process_queue,quantum_time1,quantum_time2);
    cout << "The q1 time is:" << quantum_time1 << endl;
    cout << "The q2 time is:" << quantum_time2 << endl;
    
    run_processes(process_queue,quantum_time1,clock);
    return 0;
}

void init_queue(queue<pcb*>& pro_queue,int& quantum_time1,int& quantum_time2) 
{
    ifstream input("HWK4input.txt");
    input >> quantum_time1;
    input >> quantum_time2;

    string line;
    int process_count = 0;
 
    while(getline(input,line))
    {
        int service_time;
        service_time = atoi(line.c_str());

        if(service_time != 0)
        {
            process_count++;
            pcb* pro = new pcb(process_count,service_time);
            pro_queue.push(pro);
        }    
    }
}

void run_processes(queue<pcb*>& process_queue, int quantum_time, int& clock)
{
    int exe_time = 0;
    while(process_queue.size() > 0 )
    {
        cout << "QUEUE SIZE: " << process_queue.size() << endl;
        pcb* pro2 = process_queue.front();
        process_queue.pop();
        cout << "PID:" << pro2->get_pid() << " CLASS: " <<  pro2->get_pro_class() << " TIME REMAINING:" << pro2->get_service_time() << endl;
        
        exe_time = 0;
        while(exe_time < quantum_time && pro2->get_service_time() > 0)
        {
            exe_time++;
            clock++;
            pro2->add_cpu_time();
        }
        
        
       // cout << "PID: " << pro2->get_pid() << endl;
        cout << "Time Remaining: " << pro2->get_service_time() << endl;
        cout << "CLOCK: " << clock << endl;

        if(pro2->get_service_time() != 0)
        {
            process_queue.push(pro2);
            pro2->increment_pro_class();
        }
    }

}