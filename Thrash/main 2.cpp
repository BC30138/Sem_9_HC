#include <time.h>
#include <iostream>
#include <pthread.h>
#include <zconf.h>
#include <unistd.h>


using namespace std;

typedef struct ins {
	int *buffer;//массив
	int length;//длина массива
	int *count;//количество циклов

} ins;

bool FLAG = true;

// размер кэша 32 обычно, его надо умножить на 1024, чтобы перевести в байты, потом умножить на количество ядер, разделить на размерность инта и умножить на коэффициент использования 

int CACHE_SIZE = 6144;// размер кэша

int *createBuffer(int buffer_size) {

	int *buffer = new int[buffer_size];
	for (int i = 0; i < buffer_size; i++)
		buffer[i] = i;
	return buffer;

}

void *threadMain(void *data) {
	ins *parameters = (ins *) data;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < parameters->length; j++)
			parameters->buffer[j] == 0;
	}

	int s = 0;

	while (FLAG) {
		parameters->count[0]++;
		s = parameters->buffer[rand() % parameters->length];
	};
}

void Task1(int time) {

	int count = 0;
	int bufer_size = CACHE_SIZE * 0.75;
	int *buffer = createBuffer(bufer_size);

	ins thread;
	thread.buffer = buffer;
	thread.length = bufer_size;
	thread.count = &count;

	pthread_t handle;
	FLAG = true;
	pthread_create(&handle, NULL, threadMain, &thread);

	sleep(time);

	FLAG = false;
	pthread_join(handle, NULL);

	count /= 1000000;
	cout << "1 thread: " << count << endl;

	delete (buffer);

}

void Task2(int time) {

	int count = 0;
	int bufer_size = 2 * CACHE_SIZE * 0.75;
	int *buffer = createBuffer(bufer_size);

	ins thread;
	thread.buffer = buffer;
	thread.length = bufer_size;
	thread.count = &count;

	FLAG = true;
	pthread_t handle;
	pthread_create(&handle, NULL, threadMain, &thread);

	sleep(time);

	FLAG = false;
	pthread_join(handle, NULL);

	count /= 1000000;
	cout << "1 thread with double array " << count << endl;

}

void Task3(int time) {

	int count1 = 0;

	int count2 = 0;

	int buffer_size = CACHE_SIZE * 0.75;

	int *buffer1 = createBuffer(buffer_size);
	int *buffer2 = createBuffer(buffer_size);

	ins thread1, thread2;
	thread1.buffer = buffer1;
	thread1.length = buffer_size;
	thread1.count = &count1;
	thread2.buffer = buffer2;
	thread2.length = buffer_size;
	thread2.count = &count2;

	pthread_t handle1, handle2;
	FLAG = true;
	pthread_create(&handle2, NULL, threadMain, &thread2);
	pthread_create(&handle1, NULL, threadMain, &thread1);

	sleep(time);

	FLAG = false;


	pthread_join(handle1, NULL);
	pthread_join(handle1, NULL);

	count1 /= 1000000;
	count2 /= 1000000;

	cout << "2 thread: " << count1 + count2 << endl;
	cout << count1;// << " " << count2 << endl;
	cout << " ";
	cout << count2;

}

int main() {

	int time = 30;
	Task1(time);
	Task2(time);
	Task3(time);

	return 0;
}