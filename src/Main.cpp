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
	int sizes[proc_num];

    int *buffer = createBuffer(buffer_size);
    int **buffer_proc = new int*[proc_num];
    for (int it = 0; it < proc_num - 1; it++) {
        sizes[it] = proc_b_size;
        buffer_proc[it] = new int[proc_b_size];
        copy(buffer + (it * proc_b_size), 
            buffer + (it + 1) * proc_b_size - 1,
            buffer_proc[it]);
    }

    sizes[proc_num - 1] = proc_b_size + (buffer_size % proc_num);
    buffer_proc[proc_num - 1] = new int[sizes[proc_num - 1]];
    copy(buffer + (proc_num - 1) * proc_b_size, 
        buffer + (proc_num - 1) * proc_b_size + sizes[proc_num - 1] - 1,
        buffer_proc[proc_num - 1]);

    cout << "Thread number:\t" << proc_num << endl;
    cout << "Buffer size (KB):\t" << (double)buffer_size * sizeof(int) / 1024 << endl;
    for(int it = 0; it < proc_num; it++)
    {
        cout << "Thread " << it << " buffer size (KB):\t" << (double)sizes[it] * sizeof(int) / 1024  << endl;
    }

    omp_set_dynamic(0);
    int count = 0;
	#pragma omp parallel reduction(+:count) num_threads(proc_num)
	{
        double start_time = omp_get_wtime();
        int s;
        bool FLAG = false;
        int *parameters = buffer_proc[omp_get_thread_num()];
        int length = sizes[omp_get_thread_num()];

        for (int it = 0; it < buffer_size; it++) {
				for (int jt = 0; jt < length; jt++)
					parameters[jt] == jt;
			} 
        
        #pragma omp single
        {
             cout << "Wait " << time << " s." << endl;
        }

		while(!FLAG)
		{    
            count++;
			s = parameters[rand() % length] + parameters[0] + parameters[length];
			
            if ((omp_get_wtime() - start_time) > time) 
            {
                FLAG = true;
                printf("Thread %d ref:\t%d x 10^6\n", omp_get_thread_num(), count / 1000000);
            }
		}
	}
    cout << "Sum ref:\t" << count / 1000000 << " x 10^6\n" << endl;
}

int main() {
	int time = 20;
    int compare_proc_num = 2;
    size_t buffer_size = (CACHE_SIZE / sizeof(int)) * 0.8 * compare_proc_num;
	Task(time, 1, buffer_size);
    Task(time, compare_proc_num, buffer_size);

	return 0;
}