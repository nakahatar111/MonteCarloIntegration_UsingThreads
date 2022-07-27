#include <pthread.h>
#include <iostream>
#include <chrono>
#include <math.h>
#include <cassert>
#include <vector>
using namespace std;
using namespace std::chrono;
double sum = 0;
pthread_mutex_t mutexsum;
int a = 2;
int b = 5;
int iteration;
vector<long> perThread;

double fn(double x){
  return sin(x)/x;
}

void *integrate(void* seed){
  auto start = high_resolution_clock::now();
  double count = 0;
  double randX;
  for(int i = 0; i < iteration; i++){
    randX = a + (double(rand_r((unsigned int*)(uintptr_t)&seed))/RAND_MAX) * (b-a);
    count+= fn(randX);
  }
  assert(pthread_mutex_lock (&mutexsum)==0);
  sum += count;
  auto stop = high_resolution_clock::now();
  perThread.push_back((long)duration_cast<microseconds>(start.time_since_epoch()).count());
  perThread.push_back((long)duration_cast<microseconds>(stop.time_since_epoch()).count());
  assert(pthread_mutex_unlock (&mutexsum)==0);
  pthread_exit(NULL);
}

int main (int argc, char *argv[]){
    assert(pthread_mutex_init(&mutexsum, NULL)==0);
    int NUM = 100000000;
    int THREAD = 20;
    vector<double> vec;
    for(int i = 1; i <= 20; i ++){
      sum = 0;
      int NUMTHREADS = i;
    
      iteration = NUM / NUMTHREADS;
      int total = iteration * NUMTHREADS;
      auto start = high_resolution_clock::now();
      pthread_t arr_thread[NUMTHREADS];

      for(int i = 0; i < NUMTHREADS; i++){
        int err = pthread_create(&arr_thread[i], NULL, integrate, (void*)(intptr_t)i);
        if(err){
          cout << "Error code "<< err << endl; 
          exit(-1);
        }
      }
      
      for (int t = 0; t < NUMTHREADS; t++){
        int retval = pthread_join(arr_thread[t], NULL); // waits for all threads to finish
        if(retval != 0){
          printf("Error occurred when joining the thread\n");
          exit(-1);
        }
      }
      double estimate = (b-a)*sum/total;
      cout << "Estimate for fn: " << estimate << endl;
      auto stop = high_resolution_clock::now();
      auto duration = duration_cast<microseconds>(stop - start);
      cout << "Time: " << duration.count() << " microseconds" << endl;
      vec.push_back((double)duration.count());
    }
    cout.precision(10);
    cout << endl << '[';
    for(double x : vec)
      cout << x << ',';
    cout << ']' << endl;
    
    int index = 0;
    for(int i = 1; i <= 20; i++){
      cout << "Thread#" << i << ":[";
      for(int j = 1; j <=i; j++){
        cout << '('<< perThread.at(index) %1000000000 << ',';
        cout << perThread.at(index+1) %1000000000 << "),";
        index +=2;
      }
      cout << ']' << endl;
    }

    assert(pthread_mutex_destroy(&mutexsum)==0);
    pthread_exit(NULL); //make sure no threads are still running before terminating
}