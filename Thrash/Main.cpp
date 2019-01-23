#include <iostream>
#include <cmath>
#include <omp.h>

using namespace std;

#define CACHE_SIZE 32 * 1024 // у меня кэш L1 = 32КБ

typedef struct ins {
	int *buffer; //массив
	int length; //длина массива
	// int *count; //количество циклов
} ins;

// bool FLAG = true;

// размер кэша 32 обычно, его надо умножить на 1024, чтобы перевести в байты
// разделить на размерность инта и умножить на коэффициент использования 

// int CACHE_SIZE = 6144;// размер кэша

int *createBuffer(int buffer_size) {
	int *buffer = new int[buffer_size];
	for (int i = 0; i < buffer_size; i++)
		buffer[i] = i;
	return buffer;
}


void Task1(int proc_num) {
	// int *count = new int[proc_num];
	int iter_num = 300;
	// for (int it = 0; it < proc_num; it++)
	// 	count[it] = 0;

	int buffer_size = (CACHE_SIZE / sizeof(int)) * 0.95 * 4;
	int *buffer = createBuffer(buffer_size);

	ins parameters;
	parameters.buffer = buffer;
	parameters.length = buffer_size;
	// parameters.count = count;

	omp_set_num_threads(proc_num);
	
	
	#pragma omp parallel 
	{
		for (int it = 0; it < 10; it++) {
				for (int kt = 0; kt < parameters.length; kt++)
					parameters.buffer[kt] == 0;
			}
	}

	double start_time = omp_get_wtime();
	// int it, s;
	int dyn_par = iter_num * 1000000 / proc_num;
	// int count = 0;
	// #pragma omp parallel for schedule(dynamic, dyn_par) private(it, s) shared(parameters) reduction(+:count)
	// for (it = 0; it < (iter_num * 1000000); ++it)
	// {
	// 	count++;
	// 	s = parameters.buffer[rand() % parameters.length] + parameters.buffer[0] + parameters.buffer[parameters.length - 1];;
	// 	// cout << omp_get_thread_num();
	// }

	int total_sum = 0;  /* do all variable initialization prior to omp pragma */
	int i, s;

		#pragma omp parallel for \
			schedule(dynamic, dyn_par) \
            private(i) \
            reduction(+:total_sum)

	   	for (i = 0; i < iter_num * 1000000; i++)
   		{
       		total_sum += i;  /* you used nLocalSum here */
			s = parameters.buffer[rand() % parameters.length] + parameters.buffer[0] + parameters.buffer[parameters.length - 1];;
   		}


	cout << "Thread number:\t" << proc_num << endl;
	cout << "Buffer size:\t" << buffer_size << endl;
	cout << "Iterations number:\t" << iter_num << " x 10^6" << endl; 
	cout << "Time:\t" << omp_get_wtime() - start_time << endl;
}

void Task2(int proc_num)
{
	int iter_num = 1;
	int matrix_size = sqrt((CACHE_SIZE / sizeof(int)) * 0.9) * 100;
	int** matrix = new int*[matrix_size];
	for(int it = 0; it < matrix_size; ++it)
		matrix[it] = new int[matrix_size];

	for(int x = 0; x < matrix_size; x++){
        for(int y = 0; y < matrix_size; y++){
            matrix[y][x] = y;
        }
    }

	#pragma omp parallel for num_threads(proc_num)
	for (int jt = 0; jt < iter_num * 100000; jt++)
		matrix[rand() % matrix_size][rand() % matrix_size] = matrix[rand() % matrix_size][rand() % matrix_size];

	double start_time = omp_get_wtime();

    #pragma omp parallel for num_threads(proc_num)
    for(int it = 0; it < matrix_size; it++){
        for(int jt = 0; jt < matrix_size; jt++){
			for (int kt = 0; kt < iter_num; kt++)
            matrix[it][jt] = matrix[rand() % matrix_size][rand() % matrix_size] + matrix[0][0] - matrix[matrix_size - 1][matrix_size - 1];
			// cout << omp_get_thread_num(); 
        }
    }

	cout << "Thread number:\t" << proc_num << endl;
	cout << "Buffer size:\t" << matrix_size * matrix_size * sizeof(int) << endl;
	// cout << "Iterations number:\t" << iter_num / 1000000 << " x 10^6" << endl; 
	cout << "Time (s):\t" << (omp_get_wtime() - start_time) << endl;

	for(int it = 0; it < matrix_size; ++it) {
        delete[] matrix[it];   
    }
    delete[] matrix;
}

int main() {
	// Task1(2);
	Task2(4);
	// Task3(time);

	return 0;
}