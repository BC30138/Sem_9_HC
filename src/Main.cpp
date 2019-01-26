#include <iostream>
#include <algorithm>
#include <cmath>
#include <omp.h>

using namespace std;

#define CACHE_SIZE 32 * 1024 // у меня кэш L1 = 32КБ

int *createBuffer(int buffer_size) {
	int *buffer = new int[buffer_size];
	for (int it = omp_get_thread_num() * buffer_size; 
        it < (omp_get_thread_num() + 1) * buffer_size; it++)
		buffer[it - omp_get_thread_num() * buffer_size] = it;
	return buffer;
}

int *createBuffer_last(int buffer_size, int last_elements_c) {
	int *buffer = new int[buffer_size + last_elements_c];
	for (int it = omp_get_thread_num() * buffer_size; 
        it <  (omp_get_thread_num() + 1) * buffer_size + last_elements_c; it++)
		buffer[it - omp_get_thread_num() * buffer_size] = it;
	return buffer;
}

void Task(int time, int proc_num, size_t buffer_size) {
    cout << "--------------Start task---------------\n";
	int iter_num = 300;
    int proc_b_size = floor(buffer_size / proc_num);

    cout << "Thread number:\t" << proc_num << endl;
    cout << "Buffer size (KB):\t" << (double)buffer_size * sizeof(int) / 1024 << endl;
    omp_set_dynamic(0);
    int count = 0;
	#pragma omp parallel reduction(+:count) num_threads(proc_num)
	{
        int s;
        bool FLAG = false;
        int length;
        int *parameters;
        if (omp_get_thread_num() != (proc_num - 1))
        {
            length = proc_b_size;
            parameters = createBuffer(length);
        }
        else
        {
            length = proc_b_size + (buffer_size % proc_num);
            parameters = createBuffer_last(proc_b_size, (buffer_size % proc_num));
        }
        printf("Thread %d buffer size (KB):\t%f\n", omp_get_thread_num(), (double)(length * sizeof(int) ) / 1024);

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