#include <iostream>
#include <stdlib.h>
#include <cassert>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdarg.h>
#include "omp.h"
#include <string>

//Hack to make this work.
#define MAX_THREADS 1024

int64_t remaining_mem_accesses=300000000; //max num of mem accesses (130 Mill trace lines ~= 2.5GB in mem access trace file size)
bool expert_mode=std::getenv("MOSAIC_EXPERT")!=NULL;
//expert mode doesn't limit size of memory trace files

std::ofstream f1[MAX_THREADS]; //for mem.txt
std::ofstream f2[MAX_THREADS]; // for ctrl.txt
std::ofstream f3[MAX_THREADS]; // for acc.txt

__attribute__((noinline))
extern "C"
void tracer_cleanup() {
  for (int i = 0; i < MAX_THREADS; i++) {
    if (f1[i].is_open()) {
      f1[i].close();
    }
    if (f2[i].is_open()) {
      f2[i].close();
    }    
    if (f3[i].is_open()) {
      f3[i].close();
    }
  }
}

__attribute__((noinline))
std::string get_dir_name(std::string run_dir, std::string kernel_type, std::string type) {
  if (omp_get_thread_num() >= MAX_THREADS) {
    std::cout << "ERROR: Unable to log for all threads! Increase MAX_THREADS in tracer.cc" << "\n";
    assert(0);
  }
  std::ostringstream ret;
  ret << run_dir << "/output_" << kernel_type << "_" << omp_get_thread_num() << "/" << type;
  ret.flush();
  //ret << "tmp" << omp_get_thread_num() << ".txt";
  return ret.str();
}

__attribute__((noinline))
extern "C"
void printBranch(char* name, char *kernel_type, char *run_dir, int cond, char* n1, char *n2)
{
  if(!f1[omp_get_thread_num()].is_open()) {
    //f1.open("output/ctrl.txt", std::ofstream::out | std::ofstream::trunc);
    f1[omp_get_thread_num()].open(get_dir_name(run_dir, kernel_type, "ctrl.txt"), std::ofstream::out | std::ofstream::app);
  }
  char *target;
  if(cond == 0)
    target = n1;
  else
    target = n2;
  f1[omp_get_thread_num()] << "B,"<<name << "," << target << "\n";
  //std::cout << "Branch ["<< name << "]: " << cond << " / " << target <<  "\n";	
  //f1[omp_get_thread_num()].close();
}

__attribute__((noinline))
extern "C"
void printuBranch(char* name, char *kernel_type, char *run_dir, char *n1)
{
  if(!f1[omp_get_thread_num()].is_open()) {
    //f1.open("output/ctrl.txt", std::ofstream::out | std::ofstream::trunc);
    f1[omp_get_thread_num()].open(get_dir_name(run_dir, kernel_type, "ctrl.txt"), std::ofstream::out | std::ofstream::app);
  }
  if (!f1[omp_get_thread_num()].is_open())
    assert(false);
  f1[omp_get_thread_num()] << "U,"<< name << "," << n1 << "\n";
  //f1[omp_get_thread_num()].close();
}

__attribute__((noinline))
extern "C"
void printMem(char *name, char *kernel_type, char *run_dir, bool type, long long addr, int size)
{
  
  if(!expert_mode) {    
    if(remaining_mem_accesses<=0) {
      std::cout << "Memory trace exceeded max num of memory accesses. \n Aborting... \n";
      assert(false);
    }
    remaining_mem_accesses--;
  }
  
  if(!f2[omp_get_thread_num()].is_open()) {


    auto fname = get_dir_name(run_dir, kernel_type, "mem.txt");
    //std::cout << omp_get_thread_num() <<  fname << "\n";
    f2[omp_get_thread_num()].exceptions(std::ifstream::failbit | std::ifstream::badbit);
    f2[omp_get_thread_num()].open(fname, std::ofstream::out | std::ofstream::app);


  }

  //printf("%s %d: recording mem trace\n", get_dir_name(run_dir, kernel_type, "mem.txt"), omp_get_thread_num());
  if (type == 0) {
    f2[omp_get_thread_num()] << "L,"<< name << "," << addr << ","<< size <<"\n";

    //std::cout << omp_get_thread_num() << " " << f2[omp_get_thread_num()].good() << "," << std::ifstream::badbit << "," << "L,"<< name << "," << addr << ","<< size <<"\n";
      
  }
  else if (type == 1)
    f2[omp_get_thread_num()] << "S,"<< name << "," << addr << ","<< size <<"\n";

  //f2[omp_get_thread_num()].flush();

}

__attribute__((noinline))
extern "C"
void print_matmul(char *acc_kernel_name, char *kernel_type, char *run_dir, char* node_id, int rowsA, int colsA , int rowsB, int colsB, int batch_size)
{
  if(!f3[omp_get_thread_num()].is_open()) {
    
    f3[omp_get_thread_num()].open(get_dir_name(run_dir, kernel_type, "acc.txt"), std::ofstream::out | std::ofstream::app);
  }
  //std::cout << "printing acc now " << rowsA << ","<< colsA << rowsB << ","<< colsB <<"\n";
 
  f3[omp_get_thread_num()] << acc_kernel_name << "," << node_id << "," << rowsA << ","<< colsA << ","<< rowsB << ","<< colsB << "," << batch_size << "\n";
  //f3[omp_get_thread_num()].close();
}

__attribute__((noinline))
extern "C"
void print_sdp(char *acc_kernel_name, char *kernel_type, char *run_dir, char* node_id, int working_mode, int size)
{
  if(!f3[omp_get_thread_num()].is_open()) {
    
    f3[omp_get_thread_num()].open(get_dir_name(run_dir, kernel_type, "acc.txt"), std::ofstream::out | std::ofstream::app);
  }

 
  f3[omp_get_thread_num()] << acc_kernel_name << "," << node_id << "," << working_mode << ","<< size << "\n";
  //f3[omp_get_thread_num()].close();
}

__attribute__((noinline))
extern "C"
void print_conv2d_layer(char *acc_kernel_name, char *kernel_type, char *run_dir, char* node_id , int batch, int in_channels, int in_height, int in_width, int out_channels, int filter_height, int filter_width, bool zero_pad, int vert_conv_stride, int horiz_conv_stride, bool pooling, int pool_height, int pool_width, int vertical_pool_stride, int horizontal_pool_stride)
{
  if(!f3[omp_get_thread_num()].is_open()) {
    
    f3[omp_get_thread_num()].open(get_dir_name(run_dir, kernel_type, "acc.txt"), std::ofstream::out | std::ofstream::app);
  }
 
  f3[omp_get_thread_num()] << acc_kernel_name << "," << node_id << "," <</*0*/ batch <<"," << /*1*/ in_channels << "," << /*2*/ in_height << "," << /*3*/ in_width << "," << /*4*/ out_channels << "," << /*5*/ filter_height << "," << /*6*/ filter_width << "," << /*7*/ zero_pad << "," << /*8*/ vert_conv_stride << "," << /*9*/ horiz_conv_stride << "," << /*10*/ pooling << "," << /*11*/ pool_height << "," << /*12*/ pool_width << "," << /*13*/ vertical_pool_stride << "," << /*14*/ horizontal_pool_stride << "\n";

  //f3[omp_get_thread_num()].close();
}

__attribute__((noinline))
extern "C"
void print_dense_layer(char *acc_kernel_name, char *kernel_type, char *run_dir, char* node_id, int batch, int in_channels, int out_channels)
{
  if(!f3[omp_get_thread_num()].is_open()) {
    
    f3[omp_get_thread_num()].open(get_dir_name(run_dir, kernel_type, "acc.txt"), std::ofstream::out | std::ofstream::app);
  }
 
  f3[omp_get_thread_num()] << acc_kernel_name << "," << node_id << "," << /*0*/ batch << ","<< /*1*/ in_channels << ","<< /*2*/ out_channels << "\n";

  //f3[omp_get_thread_num()].close();
}

__attribute__((noinline))
extern "C"
void printSw(char *name, char *kernel_type, char *run_dir, int value, char *def, int n, ...)
{
  if(!f1[omp_get_thread_num()].is_open()) {
    //f1.open("output/ctrl.txt", std::ofstream::out | std::ofstream::trunc);
    f1[omp_get_thread_num()].open(get_dir_name(run_dir, kernel_type, "ctrl.txt"), std::ofstream::out | std::ofstream::app);
  }
  va_list vl;
  std::vector<int> vals;
  std::vector<char*> strs;
  char* target;
  va_start(vl,n);
  for(int i=0; i<n; i++) {
    int val = va_arg(vl, int);
    char *bbname = va_arg(vl, char*);
    vals.push_back(val);
    strs.push_back(bbname);
  }
  va_end(vl);
  bool found = false;
  for(int i=0; i<vals.size(); i++) {
    if(vals.at(i) == value) {
      target = strs.at(i);
      found = true;
      break;
    }
  }
  if(!found)
    target = def;
  f1[omp_get_thread_num()] << "S," <<name << "," << target <<"\n";
  //std::cout << "Switch [" << name << "]: " << value << " / " << target << "\n";
  //f1[omp_get_thread_num()].close();
}
