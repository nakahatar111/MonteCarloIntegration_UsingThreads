#include <pthread.h>
#include <iostream>
#include <chrono>
#include <math.h>
#include <cassert>
using namespace std;
using namespace std::chrono;
double sum = 0;
pthread_mutex_t mutexsum;
int a;
int b;
int iteration;

double fn(double x){
  return sin(x)/x;
}

void *integrate(void* seed){
  double count = 0;
  double randX;
  for(int i = 0; i < iteration; i++){
    randX = a + (double(rand_r((unsigned int*)(uintptr_t)&seed))/RAND_MAX) * (b-a);
    count+= fn(randX);
  }
  assert(pthread_mutex_lock (&mutexsum)==0);
  sum += count;
  assert(pthread_mutex_unlock (&mutexsum)==0);
  pthread_exit(NULL);
}

int main (int argc, char *argv[]){
  if(argc == 5 && isdigit(*argv[1]) && isdigit(*argv[2]) && isdigit(*argv[3]) && isdigit(*argv[4])){
    assert(pthread_mutex_init(&mutexsum, NULL)==0);
    a = stoi(argv[1]);
    b = stoi(argv[2]);
    int NUM = stoi(argv[3]);
    int NUMTHREADS = stoi(argv[4]);

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
    assert(pthread_mutex_destroy(&mutexsum)==0);
    pthread_exit(NULL); //make sure no threads are still running before terminating
  }
  else{
    cout << "Argument is missing or is not an int" << endl;
  }
}