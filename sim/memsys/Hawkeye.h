#include <vector>
#include <map>

#define OPTGEN_SIZE 128
#define MAX_PCMAP 31
#define PCMAP_SIZE 2048

using namespace std;

struct OPTgen{
  vector<int> liveness_intervals;
  int cache_size;

  //Initialize values
  void init(int size){
    liveness_intervals.resize(OPTGEN_SIZE, 0);
    cache_size = size;
  }

  void set_access(int val){
    liveness_intervals[val] = 0;
  }

  //Return if hit or miss
  bool is_cache(int val, int endVal){
    bool cache = true;
    int count = endVal;
    while (count != val) {
      if(liveness_intervals[count] >= cache_size){ // too many different tags in the cache
        cache = false; // miss
        break;
      }
      count = (count+1) % liveness_intervals.size();
    }

    if (cache) { // hit
      count = endVal;
      while (count != val) { // update liveness counts
        liveness_intervals[count]++;
        count = (count+1) % liveness_intervals.size();
      }
    }

    return cache;
  }
};

struct addr_history {
  int PCval;
  int previousVal;
  int lru;
  bool prefetching;

  void init(){
    PCval = 0;
    previousVal = 0;
    lru = 0;
    prefetching = false;
  }

  void update(int currentVal, int PC){
    previousVal = currentVal;
    PCval = PC;
  }

  void set_prefetch(){
    prefetching = true;
  }
};

class Hawkeye_Predictor {
private:
  std::unordered_map<int, int> PC_Map;

public:
  //Return prediction for PC Address
  bool get_prediction(int PC) {
    int result = PC % PCMAP_SIZE;
    if(PC_Map.find(result) != PC_Map.end() && PC_Map[result] < ((MAX_PCMAP+1)/2)){
      return false;
    }
    return true;
  }

  void increase(int PC){
    int result = PC % PCMAP_SIZE;
    if(PC_Map.find(result) == PC_Map.end()){
      PC_Map[result] = (MAX_PCMAP+1)/2;
    }
    if(PC_Map[result] < MAX_PCMAP){
      PC_Map[result] = PC_Map[result]+1;
    } 
  }

  void decrease(int PC){
    int result = PC % PCMAP_SIZE;
    if(PC_Map.find(result) == PC_Map.end()){
      PC_Map[result] = (MAX_PCMAP+1)/2;
    }
    if(PC_Map[result] > 0){
      PC_Map[result] = PC_Map[result]-1;
    }
  }
};
