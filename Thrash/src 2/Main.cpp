#include <iostream>
#include <fstream>
#include <omp.h>
#include <vector>
using namespace std;
unsigned steps =    35000;
unsigned iter =     1000;

void plot ()
{
    ofstream plot("src/script.gp");
    plot << "set logscale x" << endl;
    plot << "set xlabel 'Data size (Byte)'" << endl;
    plot << "set ylabel 'Time (s)'" << endl;
    plot << "set key spacing 1.5" << endl;
    plot << "set key right" << endl;
    plot << "plot 'data/time.dat' u 1:2 w lines lw 2 lt rgb 'red'" << endl;
    plot << "pause -1" << endl;
    system("gnuplot src/script.gp");
}

int main()
{
    ofstream out("data/time.dat");
    for (size_t step_it = iter; step_it <= steps; step_it += iter)
    {
        double start_time = omp_get_wtime();
        vector<vector<int>> test;
        test.resize(step_it); 
        #pragma omp parallel for num_threads(4)
        for (size_t it = 0; it < step_it; it++)
        {
            vector<int> temp;
            for (size_t jt = 0; jt < step_it; jt++)
            {
                temp.push_back(jt);
            }
            test[it] = temp;
        }
        cout << test[step_it - 1][step_it - 1] << endl;
        out << (double)(sizeof(test) + sizeof(int) * test.capacity() )/ (double)1048576 << "\t" << omp_get_wtime() - start_time << endl;
    }
    plot();
}
