#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>
#include <map>
#include <assert.h>
#include "../tile/Bpred.h"

using namespace std;

#define NUM_INST_TYPES 36
typedef enum { 
  I_ADDSUB, I_MULT, I_DIV, I_REM, FP_ADDSUB, FP_MULT, FP_DIV, FP_REM, LOGICAL, 
  CAST, GEP, LD, ST, TERMINATOR, PHI, SEND, RECV, STADDR, STVAL, LD_PROD, 
  INVALID, BS_DONE, CORE_INTERRUPT, CALL_BS, BS_WAKE, BS_VECTOR_INC, 
  BARRIER, ACCELERATOR, ATOMIC_ADD, ATOMIC_FADD, ATOMIC_MIN, ATOMIC_CAS, 
  TRM_ATOMIC_FADD, TRM_ATOMIC_MIN, TRM_ATOMIC_CAS, LLAMA
} TInstr;
#define word_size_bytes  4  // TODO: allow different sizes. Now, word_size is a constant

class Config {
public:

  void read(string name);
  vector<string> split(const string &s, char delim);
  void getCfg(int id, int val);

  // Config parameters
  int  verbLevel; // verbosity level
  bool cf_mode;   // 0: one at a time / 1: all together+ prefect prediction
  bool mem_speculate;
  bool mem_forward;

  // branch predictor
  int branch_predictor; 
  int misprediction_penalty=0; // number of cycles to insert before creation of next context to model misprediction
  int bht_size=1024; 
  int gshare_global_hist_bits=10; 

  // Resources
  int lsq_size;
  int cache_load_ports;
  int cache_store_ports;
  int mem_read_ports=65536;
  int mem_write_ports=65536;
  int outstanding_load_requests;
  int outstanding_store_requests;
  int max_active_contexts_BB;
  
  // FUs
  int instr_latency[NUM_INST_TYPES];
  int num_units[NUM_INST_TYPES];
  
  // Energy variables, all in Joules
  map <int, double [NUM_INST_TYPES]> energy_per_instr;
  map <int, double> energy_per_L3_access;
  map <int, double> energy_per_DRAM_access;
  double base_freq_for_EPI = 2000;  // in Mhz
  
  // cache
  bool ideal_cache;
  int cache_latency = 1;  // cycles
  int cache_size;         // in KB
  int cache_assoc = 8; 
  int cache_linesize = 64; // bytes
  int window_size = 128; //instruction window size
  int issueWidth = 8; //total # issues per cycle
  int prefetch_distance=0; // number of cachelines ahead to prefetch
  int mshr_size = 32;
  bool is_l1 = false;
  bool is_llc = false;
  int bank_size = 2*1024*1024;

  // l2 cache
  bool use_l2=false;
  bool l2_ideal_cache;
  int l2_cache_latency = 6;
  int l2_cache_size = 0;
  int l2_cache_assoc = 8;
  int l2_cache_linesize = 64;
  int l2_cache_load_ports;
  int l2_cache_store_ports;
  int l2_prefetch_distance = 0;
  int l2_num_prefetched_lines = 1;

  // llama cache
  bool llama_ideal_cache;
  int llama_cache_size = 0;
  int llama_cache_assoc = 8;
  int llama_cache_linesize = 4;
  int llama_cache_load_ports;
  int llama_cache_store_ports;
  int llama_prefetch_distance = 0;
  int llama_num_prefetched_lines = 1;

  int commBuff_size=64; //comm buff size
  int commQ_size=512; //comm queue size
  int SAB_size=128; //store address buffer size
  int SVB_size=128; //store value buffer size
  int term_buffer_size=32; //max size of terminal load buffer
  int desc_min_latency=5; //desc queue latency
  int num_prefetched_lines=1;
  bool SimpleDRAM=0;
  int dram_bw=12; //GB/s
  int dram_latency=300; //cycles
  int chip_freq=2000; //MHz
  int technology_node = 5;  // in nm  -- for now we support 22nm, 14nm, 5nm
  int num_accels = 8;
  int num_IS = 8;
  long long mem_chunk_size=1024; 
  
  // eviction
  int eviction_policy = 0;
  int llama_eviction_policy = 0;
  
  // caching strategies
  int graphfire = 0;
  int stmu = 0;
  int stmu_latency = 3;
  int num_stmu_blocks = 16;
  int aft_max = 63;
  int remote_latency = 0;

  int partition_L1 = 0;
  int partition_L2 = 0;
  int cache_by_temperature = 0;
  int node_degree_threshold = 0;
  int l2_cache_by_temperature = 0;
  int l2_node_degree_threshold = 0;

  int cache_by_signature = 0;
  int partition_ratio = 2;
  int perfect_llama = 0;
  int llama_node_id = -100;

  int record_evictions = 0;

  map<string, int> param_map = {{"lsq_size",0},{"cf_mode",1},{"mem_speculate",2},{"mem_forward",3},{"max_active_contexts_BB",4},
                {"ideal_cache",5},{"cache_size",6},{"cache_load_ports",7},{"cache_store_ports",8},{"mem_read_ports",9},
                {"mem_write_ports",10}, {"cache_latency",11}, {"cache_assoc",12}, {"cache_linesize",13}, {"window_size",14}, 
                {"issueWidth",15}, {"commBuff_size", 16}, {"commQ_size",17}, {"term_buffer_size",18}, {"SAB_size",19}, 
                {"desc_min_latency",20}, {"SVB_size",21}, {"branch_predictor", 22}, {"misprediction_penalty", 23}, 
                {"prefetch_distance", 24}, {"num_prefetched_lines",25}, {"SimpleDRAM",26}, {"dram_bw",27}, {"dram_latency",28}, 
                {"technology_node",29}, {"chip_freq",30}, {"num_accels",31}, {"num_IS",32}, {"mem_chunk_size",33},
                {"llama_ideal_cache", 34}, {"llama_cache_size", 35}, {"llama_cache_assoc", 36}, {"llama_cache_linesize", 37},
                {"llama_cache_load_ports", 38}, {"llama_cache_store_ports", 39}, {"llama_prefetch_distance", 40}, {"llama_num_prefetched_lines", 41},
                {"eviction_policy", 42}, {"llama_eviction_policy", 43}, {"partition_L1", 44}, {"partition_L2", 45}, {"cache_by_temperature", 46}, {"node_degree_threshold", 47},
                {"cache_by_signature", 48}, {"partition_ratio", 49}, {"perfect_llama", 50}, {"record_evictions", 51},
                {"use_l2", 52}, {"l2_ideal_cache", 53}, {"l2_cache_latency", 54}, {"l2_cache_size", 55}, {"l2_cache_assoc", 56}, {"l2_cache_linesize", 57}, 
                {"l2_cache_load_ports", 58}, {"l2_cache_store_ports", 59}, {"l2_prefetch_distance", 60}, {"l2_num_prefetched_lines", 61}, 
                {"l2_cache_by_temperature", 62}, {"l2_node_degree_threshold", 63}, {"llama_node_id", 64}, {"mshr_size", 65}, {"is_llc", 66}, {"is_l1", 67}, 
                {"graphfire", 68}, {"stmu", 69}, {"stmu_latency", 70}, {"num_stmu_blocks", 71}, {"aft_max", 72}, {"remote_latency", 73}, {"bht_size", 74}, {"gshare_global_hist", 75}, {"bank_size", 76}};
  
  //this converts the text in the config file to the variable using the getCfg function above
  
  Config();
};

class CacheConfig {
public:
  CacheConfig();
  void read(string name);
  vector<string> split(const string &s, char delim);
  void getCfg(int id, int val);

  // Config parameters
  int  verbLevel; // verbosity level
  bool cf_mode; // 0: one at a time / 1: all together
  bool mem_speculate;
  bool mem_forward;
  bool perfect_mem_spec;
  // Resources
  int lsq_size;
  int cache_load_ports;
  int cache_store_ports;
  int outstanding_load_requests;
  int outstanding_store_requests;
  int max_active_contexts_BB;
  // FUs
  int instr_latency[NUM_INST_TYPES];
  int num_units[NUM_INST_TYPES];
  // cache
  bool ideal_cache;
  int cache_latency = 1;  // cycles
  int cache_size;     // KB
  int cache_assoc = 8; 
  int cache_linesize = 64; // bytes
  int mshr_size = 32;
  int bank_size = 2*1024*1024;
  // l2 cache
  bool use_l2 = 0;
  bool l2_ideal_cache;
  int l2_cache_latency = 9;
  int l2_cache_size = 0;
  int l2_cache_assoc = 8;
  int l2_cache_linesize = 64;
  int l2_cache_load_ports;
  int l2_cache_store_ports;
  // llama cache
  bool llama_ideal_cache;
  int llama_cache_size;
  int llama_cache_assoc = 8;
  int llama_cache_linesize = 64;
  int llama_cache_load_ports;
  int llama_cache_store_ports;
  // eviction policy
  int eviction_policy = 0;
  int llama_eviction_policy = 0;
  // caching strategies
  int graphfire = 0;
  int stmu = 0;
  int stmu_latency = 3;
  int num_stmu_blocks = 16;
  int aft_max = 63;
  int remote_latency = 0;

  int partition_L1 = 0;
  int partition_L2 = 0;
  int cache_by_temperature = 0;
  int node_degree_threshold = 0;
  int l2_cache_by_temperature = 0;
  int l2_node_degree_threshold = 0;

  int cache_by_signature = 0;
  int partition_ratio = 2;
  int perfect_llama = 0;
};

#endif
