#include <iostream>
#include <algorithm>
#include <cmath>
#include <omp.h>

using namespace std;

#define CACHE_SIZE 32 * 1024 // у меня кэш L1 = 32КБ

int *createBuffer(int buffer_size) {
	int *buffer = new int[buffer_size];
	for (int i = 0; i < buffer_size; i++)
		buffer[i] = i;
	return buffer;
}

void Task(int time, int proc_num, size_t buffer_size) {
    cout << "--------------Start task---------------\n";
	int iter_num = 300;
    int proc_b_size = floor(buffer_size / proc_num);
    int *buffer = createBuffer(buffer_size);
    bool create_done[proc_num]; 
    for (int it = 0; it < proc_num; it++)
    {
        create_done[it] = false;
    }
    bool create_done_main = false;

    cout << "Thread number:\t" << proc_num << endl;
    cout << "Buffer size (KB):\t" << (double)buffer_size * sizeof(int) / 1024 << endl;
    omp_set_dynamic(0);
    int count = 0;
	#pragma omp parallel reduction(+:count) num_threads(proc_num) shared(create_done_main, create_done, buffer)
	{
        int s;
        bool FLAG = false;
        int length;
        int *parameters;
         #pragma omp critical 
        {
        if (omp_get_thread_num() != (proc_num - 1))
        {
            length = proc_b_size;
            parameters = new int[length];
            copy(buffer + (omp_get_thread_num() * proc_b_size), 
            buffer + (omp_get_thread_num() + 1) * proc_b_size - 1,
            parameters);
        }
        else
        {
            length = proc_b_size + (buffer_size % proc_num);
            parameters = new int[length];
            copy(buffer + (proc_num - 1) * proc_b_size, 
            buffer + (proc_num - 1) * proc_b_size + length - 1,
            parameters);
        }
        printf("Thread %d buffer size (KB):\t%f\n", omp_get_thread_num(), (double)(length * sizeof(int) ) / 1024);
        create_done[omp_get_thread_num()] = true;
        }
        
        

        for (int it = 0; it < proc_num; it++)
        {
            if (create_done == false) break;
            if (it == (proc_num - 1)) create_done_main = true;
        }

        if (create_done_main)
        {
            #pragma omp single 
            {
            delete[] buffer; 
            }
        }

        for (int it = 0; it < buffer_size * 10; it++) {
				for (int jt = 0; jt < length; jt++)
					parameters[jt] == jt;
			} 
        
        #pragma omp single
        {
            printf("Wait time (s.):\t%d\n", time);
        }
        
        double start_time = omp_get_wtime();
		while(!FLAG)
		{    
            count++;
			s = parameters[rand() % length] + parameters[rand() % length] 
                + parameters[rand() % length];
			
            if ((omp_get_wtime() - start_time) > time) 
            {
                #pragma omp critical
                {
                FLAG = true;
                count = count / 1000000;
                printf("Thread %d ref:\t%d x 10^6\n", omp_get_thread_num(), count);
                }
            }
		}
	}
     printf("Sum ref:\t%d x 10^6\n", count);
}

void Task_single(int time, size_t buffer_size)
{
    cout << "--------------Start task---------------\n";
	int iter_num = 300;
    int *buffer = createBuffer(buffer_size);

    cout << "Single thread\n";
    cout << "Buffer size (KB):\t" << (double)buffer_size * sizeof(int) / 1024 << endl;

    int count = 0;
    int s;
    bool FLAG = false;
 
    for (int it = 0; it < buffer_size * 10; it++) {
            for (int jt = 0; jt < buffer_size; jt++)
                buffer[jt] == jt;
        } 

    cout << "Wait " << time << " s." << endl;
    
    double start_time = omp_get_wtime();
    while(!FLAG)
    {    
        count++;
        s = buffer[rand() % buffer_size] + buffer[rand() % buffer_size] 
        + buffer[rand() % buffer_size];
        
        if ((omp_get_wtime() - start_time) > time) 
        {
            FLAG = true;
            count = count / 1000000;
            printf("Thread %d ref:\t%d x 10^6\n", omp_get_thread_num(), count);
        }
    }

    delete[] buffer;
}

int main() {
	int time = 20;
    int compare_proc_num = 4;
    size_t buffer_size = (CACHE_SIZE / sizeof(int)) * 0.95 * compare_proc_num;
	Task_single(time, buffer_size);
    Task(time, compare_proc_num, buffer_size);

	return 0;
}