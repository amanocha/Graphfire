#include <map>
#include <unordered_map>
#include <vector>
#include <set>
#include <bitset>
#include <math.h>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <time.h>

#include "Hawkeye.h"

// DRRIP
#define PSEL_MAX 1023
#define PSEL_THRS 511
#define BIP_MAX 32

// Hawkeye
#define SAMPLER_SETS 350
#define SAMPLER_HIST 8

// Graphfire
#define RRVT_SIZE 32
#define RRV_MAX 15
#define RRV_THRS 7
#define MAX_FREQ 15
#define L1_PAGE_TAG 9
#define L2_PAGE_TAG 6
#define L3_PAGE_TAG 4
#define L1_STMU_BLKS 10
#define L2_STMU_BLKS 11
#define L3_STMU_BLKS 12
#define AFT_SIZE 1024
#define AFT_MAX 1024
#define LFU_MAX 255

using namespace std;

struct AFTEntry
{
  uint64_t address=0;
  int graphNodeId;
  int count;
  AFTEntry* next;
  AFTEntry* prev;
};

struct CacheLine
{
  uint64_t addr;
  uint64_t offset;
  CacheLine* prev;
  CacheLine* next;
  int *used;
  int nodeId;
  int graphNodeId;
  int graphNodeDeg;
  bool rmw;
  int rrpv;
  bool dirty=false;

  int SHiP_sig;
  bool SHiP_outcome=false;

  bool prefetching=false;
  int sample_sig=0;

  int setid=-1;
  bool full_block=true;
  int sb_idx=0;
  unordered_map<uint64_t, CacheLine*> sub_tag_map;
  vector<CacheLine*> freeSubBlocks;
  CacheLine* subBlockHead;
  CacheLine* subBlockTail;
  CacheLine* subBlockEntries;
  int numAccesses=0;
};

class CacheSet
{
public:
  CacheLine *head;
  CacheLine *tail;
  CacheLine *entries;
  std::vector<CacheLine*> freeEntries;
  std::unordered_map<uint64_t, CacheLine*> addr_map;
  
  int llama_size;
  CacheLine **llamaHeads;
  CacheLine **llamaTails;
  CacheLine **llamaEntries;
  std::vector<CacheLine*> llamaFreeEntries[16];
  std::unordered_map<uint64_t, CacheLine*> llama_addr_map;
 
  int associativity; 
  int num_addresses;
  int llama_num_addresses;
  int cache_by_signature;
  int eviction_policy;
  int cache_by_temperature;
  int node_degree_threshold;
  int l2_node_degree_threshold;
  int perfect_llama;
  int log_linesize;
  int log_llama_linesize;
  bool isLLC;
  bool isL1;

  int RRPV_MAX;
  
  //DRRIP
  int isLeader;
  int* PSEL;
  int* bip_counter;

  //SHiP
  unordered_map<int, int>* SHCT;

  //Hawkeye
  int set_timer;
  int TIME;
  std::unordered_map<int, addr_history> cache_history_sampler;
  OPTgen optgen_occup_vector;
  Hawkeye_Predictor* predictor_demand;
  Hawkeye_Predictor* predictor_prefetch;

  //GRASP
  int LLCsize;
  int RRPV_H;
  int RRPV_P;
  int RRPV_I;

  //Graphfire
  int graphfire;
  unordered_map<int, tuple<int, int, int>>* RRVT; //track reuse of PCs
  int setcount;
  int curr_id;
  bool* learningRRVT;
  std::unordered_map<uint64_t, CacheLine*> base_tag_map;
  int stmu;
  int num_stmu_blocks;
  int aft_max;
  int num_page_bits;
  unordered_map<uint64_t, AFTEntry*>* AFT;
  AFTEntry* AFT_head; //head for tracking frequency of irregular accesses
  AFTEntry* AFT_tail; //tail for tracking frequency of irregular accesses

  std::unordered_map<int, int> access_freq_map;

  CacheSet(int size, int cache_line_size, int llama_cache_line_size, int cache_by_signature_input, int partition_ratio, int eviction_policy_input, int cache_by_temperature_input, int node_degree_threshold_input, int l2_node_degree_threshold_input, int perfect_llama_input, int LLCsize_input, bool isLLC_input, bool isL1_input, int isLeader_input, int* PSEL_input, int* bip_counter_input, unordered_map<int, int>* SHCT_input, Hawkeye_Predictor* predictor_demand_input, Hawkeye_Predictor* predictor_prefetch_input, int setcount_input, int setid_input, int graphfire_input, int stmu_input, int l2_page_bits, int aft_max_input, unordered_map<uint64_t, AFTEntry*>* AFT_input, AFTEntry** AFT_head_input, AFTEntry** AFT_tail_input)
  {
    associativity = size;
    num_addresses = cache_line_size/4;
    llama_num_addresses = llama_cache_line_size/4;
    int num_blocks = cache_line_size/llama_cache_line_size;
    cache_by_signature = cache_by_signature_input;
    eviction_policy = eviction_policy_input;
    cache_by_temperature = cache_by_temperature_input;
    node_degree_threshold = node_degree_threshold_input;
    l2_node_degree_threshold = l2_node_degree_threshold_input;
    perfect_llama = perfect_llama_input;
    log_linesize = log2(cache_line_size);
    log_llama_linesize = log2(llama_cache_line_size);
   
    LLCsize = LLCsize_input;
    isLLC = isLLC_input;
    isL1 = isL1_input;
   
    if (eviction_policy == 4 || eviction_policy == 5) {
      RRPV_MAX = 7;
    } else {
      RRPV_MAX = 3;
    }

    //DRRIP
    isLeader = isLeader_input;
    PSEL = PSEL_input;
    bip_counter = bip_counter_input;

    //SHiP
    SHCT = SHCT_input;  

    //Hawkeye
    set_timer = 0;
    TIME = 1024;
    optgen_occup_vector.init(size-2);
    predictor_demand = predictor_demand_input;
    predictor_prefetch = predictor_prefetch_input;

    //GRASP
    RRPV_H = 0; // priority hit promotion
    RRPV_P = 1; // priority insertion
    RRPV_I = RRPV_MAX - 1; // intermediate insertion

    //Graphfire
    graphfire = graphfire_input;
    setcount = setcount_input;
    curr_id = setid_input;
    stmu = stmu_input;
    aft_max = aft_max_input;
    AFT = AFT_input;
    AFT_head = *AFT_head_input;
    AFT_tail = *AFT_tail_input;

    num_page_bits = l2_page_bits;
    if (isL1) {
      num_page_bits = l2_page_bits + 3;
    } else if (isLLC) {
      num_page_bits = l2_page_bits - 2;
    }
    int num_freq_bits = 0;
    if (eviction_policy == 8) {
      num_freq_bits = log2(aft_max+1);
    }
    num_stmu_blocks = (int) floor(512/(num_page_bits + 38 + num_freq_bits));

    int normal_size; // number of entries in normal list
    CacheLine *c;
    if (cache_by_signature == 1) {
      normal_size = size/(partition_ratio + 1);
      llama_size = (size - normal_size);
      llamaEntries = new CacheLine*[num_blocks];
      llamaHeads = new CacheLine*[num_blocks];
      llamaTails = new CacheLine*[num_blocks];
      for(int n=0; n<num_blocks; n++) {
        llamaEntries[n] = new CacheLine[llama_size];
        llamaHeads[n] = new CacheLine;
        llamaTails[n] = new CacheLine;
        for(int i=0; i<llama_size; i++) {
          c = &llamaEntries[n][i];
          c->used = new int[llama_num_addresses];
          for(int j=0; j<llama_num_addresses; j++) {
            c->used[j] = 0;
          }
          llamaFreeEntries[n].push_back(c); 
        }
        llamaHeads[n]->prev = NULL;
        llamaHeads[n]->next = llamaTails[n];
        llamaTails[n]->next = NULL;
        llamaTails[n]->prev = llamaHeads[n];
      } 
    } else {
      normal_size = size;
    }

    entries = new CacheLine[normal_size];
    for(int i=0; i<normal_size; i++) {
      c = &entries[i];
      c->used = new int[num_addresses];
      for(int j=0; j<num_addresses; j++) {
        c->used[j] = 0;
      }
      freeEntries.push_back(c);
    }
    head = new CacheLine;
    tail = new CacheLine;
    head->prev = NULL;
    head->next = tail;
    tail->next = NULL;
    tail->prev = head;
  }
  ~CacheSet()
  {
    delete head;
    delete tail;
    delete [] entries;
    //delete [] llamaEntries;
  }

  bool access(uint64_t address, uint64_t offset, int nodeId, int graphNodeId, int graphNodeDeg, bool isLoad, bool isRMW, bool isPrefetch, bool isFull)
  {
    if (cache_by_temperature == 1 && graphNodeId != -1 && graphNodeDeg < node_degree_threshold) {
      return false;
    }
    if (cache_by_signature == 1 && graphNodeId != -1 && llama_size == 0) {
      return false;
    }

    int index = offset >> log_llama_linesize;
    CacheLine *c;
    if (cache_by_signature == 1 && graphNodeId != -1) { 
      c = llama_addr_map[(address << (log_linesize - log_llama_linesize)) + index];
    } else {
      c = addr_map[address];
    }

    //cout << "SET " << curr_id << ": ACCESSING " << nodeId << ", " << isL1 << " " << isLLC << " " << isPrefetch << endl;
    //cout << graphNodeId << endl;
    //cout << "LEARNING: " << *learningRRVT << endl;
    if (graphfire > 0 && stmu == 1 && !(*learningRRVT)) { // STMU can be used
      //bool full_block = get<0>((*RRVT)[nodeId]) >= RRV_THRS || isFull;
      if (!isFull) {      
        CacheLine* way = base_tag_map[(address >> num_page_bits)];
        if (way) { 
          //cout << "BASE TAG HIT, SUB TAG ACCESS: " << getSubTag(address, offset) << endl;
          uint64_t sub_tag = getSubTag(address, offset);
          CacheLine* sb = way->sub_tag_map[sub_tag];
          if (!sb) { 
            //cout << "SUB TAG MISS" << endl;
            return false;
          }
          //cout << "SUB TAG HIT" << endl;
          if(!isLoad)
            sb->dirty = true;
          sb->addr = address;
          sb->offset = offset;
          sb->nodeId = nodeId;
          sb->graphNodeId = graphNodeId;
          sb->graphNodeDeg = graphNodeDeg;
          sb->rmw = isRMW;
          if (sb->numAccesses < aft_max) {
            sb->numAccesses++;
          }
          deleteNode(sb);
          assert(sb!=way->subBlockHead);
          assert(sb!=way->subBlockTail);
          if (eviction_policy == 8) { // hit on sub-block
            insertByNumAccesses(sb, way->subBlockHead, way->subBlockTail);
          } else if (eviction_policy == 9) {
            insertByDegree(sb, way->subBlockHead, way->subBlockTail);
          } else {
            insertFront(sb, way->subBlockHead); // hit on sub-block, need to update MRU within sub-blocks
          }
          //assert(get<0>((*RRVT)[nodeId]) < RRV_THRS);
          bool isPIA = RRVT->find(nodeId) != RRVT->end() && (get<2>((*RRVT)[nodeId]) == RRV_THRS);
          if (eviction_policy == 7 && isLoad && isLLC && isPIA) {
            int num_set_bits = 11;
            uint64_t full_addr = (address << (num_set_bits + 6)) + (curr_id << 6) + offset;
            if (AFT->find(full_addr) != AFT->end()) {
              //cout << "FOUND MATCH" << endl;
              AFTEntry* entry = (*AFT)[full_addr];
              if (entry->count < AFT_MAX) {
                entry->count++;
              }
              deleteNodeAFT(entry);
              insertAFT(entry, AFT_head, AFT_tail);
            } else {
              addToAFT(full_addr, graphNodeId, AFT_head, AFT_tail);
            }
          }

          // update base block
          way->nodeId = nodeId;
          way->full_block = 0;
          if (way->numAccesses < aft_max) {
            way->numAccesses++;
          }
          deleteNode(way); 
          if (eviction_policy == 10 || (eviction_policy == 8)) { // hit on sub-block, update base block
            insertByNumAccesses(way, head, tail);
          } else if (eviction_policy == 9) {
            insertByDegree(way, head, tail);
          } else {
            insertFront(way, head); 
          }
          return true;
        } else { 
          //cout << "BASE TAG MISS" << endl; 
          return false;
        }
      } // otherwise, normal block and don't need to do anything extra
    }
 
    if(c) { // Hit
      //cout << "NORMAL BLOCK HIT" << endl;
      deleteNode(c);

      if (cache_by_signature == 1) { // use evict by degree for llamas, LRU for non-llamas
        if (graphNodeDeg != -1) { // llama access
          if (eviction_policy == 0 || eviction_policy == 2) {
            insertFront(c, llamaHeads[index]);
          } else if (eviction_policy == 1) {
            insertByNumAccesses(c, llamaHeads[index], llamaTails[index]);
          } else if (eviction_policy == 3) {
            insertByWeightedAccesses(c, llamaHeads[index], llamaTails[index], access_freq_map);
          } else if (eviction_policy == 4) {
            insertByDegree(c, llamaHeads[index], llamaTails[index]);
          }
        } else {
          if (perfect_llama == 1) {
            if (eviction_policy == 0 || eviction_policy == 2) {
              insertFront(c, head);
            } else if (eviction_policy == 1) {
              insertByNumAccesses(c, head, tail);
            } else if (eviction_policy == 3) {
              insertByWeightedAccesses(c, head, tail, access_freq_map);
            }
          } else { 
            insertFront(c, head);
          }
        }
      } else { // Policies for base tags
        if (eviction_policy == 0 || eviction_policy == 7 || eviction_policy == 8 || eviction_policy == 9) {  // LRU
          insertFront(c, head); 
        } else if (eviction_policy == 1) { // DRRIP
          c->rrpv = 0;
          insertFront(c, head);
        } else if (eviction_policy == 2) { // SHiP
          if (isLLC) {
            if (SHCT->find(nodeId) == SHCT->end()) {
              (*SHCT)[nodeId] = 0;
              c->SHiP_sig = nodeId;
            }
            c->SHiP_outcome = true;
            (*SHCT)[nodeId]++;
          }
          c->rrpv = 0;
          insertFront(c, head);
        } else if (eviction_policy == 3) { // Hawkeye
          if (isLLC) {
            if (!isPrefetch) {
              c->prefetching = false;
            }
            c->sample_sig = nodeId;
            if (isLeader == 0 || isLeader == 1) { // sampled set
              updateHawkeye(address, nodeId, isPrefetch);
            }
            bool prediction = predictor_demand->get_prediction(nodeId); // get Hawkeye prediction
            if (isPrefetch){
              prediction = predictor_prefetch->get_prediction(nodeId);
            }
            if (!prediction) {
              c->rrpv = RRPV_MAX;
            } else {
              c->rrpv = 0;
            }
          }
          insertFront(c, head);
        } else if (eviction_policy == 4) { // GRASP approx
          if (isLLC) {
            if (graphNodeId >= 0 && graphNodeDeg > node_degree_threshold) { // high reuse region
              c->rrpv = RRPV_H;
            } else { // moderate or low reuse region
              if (c->rrpv > 0) {
                c->rrpv = c->rrpv - 1;
              }
            }
          }
          insertFront(c, head);
        } else if (eviction_policy == 5) { // GRASP
          if (isLLC) {
            if (graphNodeId >= 0 && graphNodeId < LLCsize) { // high reuse region
              c->rrpv = RRPV_H;
            } else { // moderate or low reuse region
              if (c->rrpv > 0) {
                c->rrpv = c->rrpv - 1;
              }
            }
          }
          insertFront(c, head);       
        } else if (eviction_policy == 6) {
          assert(graphfire > 0);
          if (*learningRRVT) {
            insertFront(c, head);
          } else {
            if (c->full_block) {
              insertRegular(c, head, tail);
            } else {
              insertFront(c, head); 
            }
          }
        //} else if (eviction_policy == 9) {
        //  insertByWeightedAccesses(c, head, tail, access_freq_map);
        } else if (eviction_policy == 10) { // LFU on base blocks for hit
          if (c->numAccesses < aft_max) {
            c->numAccesses++;
          }
          insertByNumAccesses(c, head, tail);
        }
      }

      if (graphfire > 0) {
        assert(isLLC || RRVT->find(nodeId) != RRVT->end());
        if (!isPrefetch && *learningRRVT) {
          if (isL1 /*&& !isLLC*/ && get<0>((*RRVT)[nodeId]) < RRV_MAX && get<1>((*RRVT)[nodeId]) == c->setid) {
            get<0>((*RRVT)[nodeId])++;
          } else if (isL1 /*&& !isLLC*/ && get<0>((*RRVT)[nodeId]) > 0 && get<1>((*RRVT)[nodeId]) != c->setid /*&& (*RRVT)[nodeId]->first < RRV_MAX*/) {
            get<0>((*RRVT)[nodeId])--;
          }
          if (isL1) {
            get<1>((*RRVT)[nodeId]) = c->setid;
          }
        }
        c->full_block = 1; //(*RRVT)[nodeId]->first > RRV_THRS;
      }

      if(!isLoad)
        c->dirty = true;
      c->offset = offset;
      c->nodeId = nodeId;
      c->graphNodeId = graphNodeId;
      c->graphNodeDeg = graphNodeDeg;
      c->rmw = isRMW;
      if (graphNodeId != -1 && eviction_policy == 3) {
        access_freq_map[graphNodeId]++;
      }
      if (cache_by_signature == 1 && graphNodeDeg != -1) {
        c->used[0] = 1; 
      } else { // Graphfire: covers full block case
        c->used[offset/4] = 1;
      }

      return true;
    } else {
      //cout << "NORMAL BLOCK MISS" << endl;
      return false;
    }
  }

  bool check_full_block(uint64_t address, uint64_t offset) 
  {
    CacheLine *c;
    c = addr_map[address];
 
    if(c) { // full block address exists in cache
      return true;
    } else { // check sub block
      if (graphfire > 0 && stmu == 1 && !(*learningRRVT)) { // STMU can be used
        CacheLine* way = base_tag_map[(address >> num_page_bits)];
        if (way) { 
          uint64_t sub_tag = getSubTag(address, offset);
          CacheLine* sb = way->sub_tag_map[sub_tag];
          if (sb) {
            return true;
          }
        }
      }
      return false;
    }
  }

  void insert(uint64_t address, uint64_t offset, int nodeId, int graphNodeId, int graphNodeDeg, bool isLoad, bool isRMW, bool isPrefetch, bool isFull, int *dirtyEvict, int64_t *evictedAddr, uint64_t *evictedOffset, int *evictedNodeId, int *evictedGraphNodeId, int *evictedGraphNodeDeg, float *unusedSpace, CacheLine *mergedBlock, CacheLine **evictedMergeBlock, bool *evictedFull, bool *evictedRMW)
  {
    if (cache_by_temperature == 1 && graphNodeId != -1 && graphNodeDeg < node_degree_threshold) {
      return;
    }
    if (cache_by_signature == 1 && graphNodeId != -1 && llama_size == 0) {
      return;
    }

    CacheLine *c;
    int eviction = 0;
    int index = offset >> log_llama_linesize;

    /*
    cout << "SET " << curr_id << ": INSERTING " << nodeId << " IN " << isL1 << " " << isLLC << " " << isPrefetch << endl;
    cout << address << " " << ((address << 12) + (curr_id << 6) + offset) << endl;
    */

    bool full_block = true;
    if (graphfire > 0 && stmu == 1 && !(*learningRRVT)) { // STMU can be used
      full_block = isFull; //get<0>((*RRVT)[nodeId]) >= RRV_THRS || isFull;
    }
    //cout << "FULL BLOCK STATUS: " << full_block << endl;

    // Determine if eviction is necessary
    if (cache_by_signature == 1 && graphNodeId != -1) {  // LLAMAs
      c = llama_addr_map[(address << (log_linesize - log_llama_linesize)) + index];
      if (llamaFreeEntries[index].size() == 0) {   
        eviction = 1;
      } else {
        c = llamaFreeEntries[index].back();
        llamaFreeEntries[index].pop_back();
      }
    } else { 
      // inserting 64B
      if (graphfire > 0 && stmu == 1 && mergedBlock) { 
        //cout << "64B MERGED BLOCK INSERTED" << endl;
        CacheLine* way = base_tag_map[(address >> num_page_bits)];
        if (way) { // base tag hit, need to evict block
          //cout << "FOUND MATCHING SUB TAG, MUST EVICT" << endl;
          *evictedMergeBlock = way; // preserve cache line info      

          // Measured unused space
          for(int i=0; i<num_stmu_blocks; i++) {
            *unusedSpace+= way->used[i];
            way->used[i] = 0;
          }
          *unusedSpace = 4*(num_addresses-*unusedSpace); // unused space in bytes

          deleteNode(way);
          way = mergedBlock; 
          insertFront(way, head);
          return; 
        }
      }
      c = addr_map[address];
      if(freeEntries.size() == 0) {
        eviction = 1;
        //cout << "NO FREE BLOCKS" << endl;
      } 

      if (!full_block) {  // inserting 4B
        //cout << "CHECKING FOR BASE TAG HIT ON MISS" << endl;
        CacheLine* way = base_tag_map[(address >> num_page_bits)];
        //cout << "BASE TAG: " << (address >> num_page_bits) << endl;
        if (way) { 
          //cout << "BASE TAG HIT, LOOKING FOR FREE SUB BLOCK" << endl;
          //cout << "SUB TAG: " << getSubTag(address, offset) << endl;
          CacheLine* sb;
          if (way->freeSubBlocks.size() == 0) {
            //cout << "NO FREE SUB BLOCK FOUND, EVICTING" << endl;
            assert(way->sb_idx == num_stmu_blocks);
            //assert(get<0>((*RRVT)[nodeId]) < RRV_THRS);
            bool isPIA = RRVT->find(nodeId) != RRVT->end() && (get<2>((*RRVT)[nodeId]) == RRV_THRS);
            if (eviction_policy == 7 && isLLC && isPIA) {
              sb = way->subBlockTail->prev;
              CacheLine* curr = sb;
              int num_set_bits = 11;
              uint64_t tmp_address =  (curr->addr << (num_set_bits + 6)) + (curr_id << 6) + curr->offset;
              while(curr != way->subBlockHead && AFT->find(tmp_address) != AFT->end() && ((*AFT)[tmp_address])->count >= (AFT_MAX+1)/2) {
                curr = curr->prev;
                tmp_address =  (curr->addr << (num_set_bits + 6)) + (curr_id << 6) + curr->offset;
              }
              if (curr != way->subBlockHead) {
                sb = curr;
              }
            } else {
              sb = way->subBlockTail->prev;
            }
            assert(sb!=way->subBlockHead);
            assert(sb!=way->subBlockTail);
            way->sub_tag_map.erase(getSubTag(sb->addr, sb->offset)); // removing sub-tag
            deleteNode(sb);
            *evictedAddr = sb->addr;
            *evictedOffset = sb->offset;
            *evictedNodeId = sb->nodeId;
            *evictedGraphNodeId = sb->graphNodeId;
            *evictedGraphNodeDeg = sb->graphNodeDeg;
            *evictedRMW = sb->rmw;
            *evictedFull = false;
   
            if(sb->dirty) {
              *dirtyEvict = 1;
            } else {
              *dirtyEvict = 0;
            }
            sb->dirty = false;
          } else {
            //cout << "FOUND FREE SUB BLOCK" << endl;
            sb = way->freeSubBlocks.back();
            way->freeSubBlocks.pop_back();
            way->used[way->sb_idx] = 1; 
            way->sb_idx = way->sb_idx + 1;
          }
          sb->addr = address;
          sb->offset = offset;
          sb->nodeId = nodeId;
          sb->graphNodeId = graphNodeId;
          sb->graphNodeDeg = graphNodeDeg;
          sb->rmw = isRMW;
          sb->numAccesses = 0;

          // Write-back cache insertion
          if (!isLoad) {
            sb->dirty = true; 
          } else {
            sb->dirty = false;
          }
 
          // Insert into address map
          uint64_t sub_tag = getSubTag(address, offset);
          way->sub_tag_map[sub_tag] = sb; // record sub tag
          way->nodeId = nodeId;
          way->full_block = full_block;
          if (way->numAccesses < aft_max) {
            way->numAccesses++;
          }
          if (eviction_policy == 8) {
            insertByNumAccesses(sb, way->subBlockHead, way->subBlockTail);
          } else if (eviction_policy == 9) {
            insertByDegree(sb, way->subBlockHead, way->subBlockTail);
          } else {
            insertFront(sb, way->subBlockHead); // update MRU for sub blocks
          }
          deleteNode(way);
          if (eviction_policy == 10 || eviction_policy == 8) {
            insertByNumAccesses(way, head, tail);
          } else if (eviction_policy == 9) {
            insertByDegree(way, head, tail); 
          } else {
            insertFront(way, head); // update MRU for blocks
          }
          return;
        } 
        //cout << "BASE TAG MISS, NEED 64B INSERTION" << endl;
      } 
      //cout << "64B INSERTION" << endl;      
    }

    if (eviction == 1) { // 64B eviction
      int size;
      if (cache_by_signature == 1 && graphNodeId != -1) {
        if (eviction_policy == 2) {
          c = RRIP(llamaHeads[index], llamaTails[index], 3);
        } else {
          c = llamaTails[index]->prev;
        }
        assert(c!=llamaHeads[index]);
        llama_addr_map.erase((c->addr << (log_linesize - log_llama_linesize)) + index);
        size = llama_num_addresses;
      } else {
        //cout << "64B EVICTION, FINDING LRU" << endl;
        c = tail->prev; // LRU
        if (cache_by_signature == 0 || perfect_llama == 1) {
          if (isLLC && (eviction_policy > 0 && eviction_policy <= 5)) {
            c = RRIP(head, tail, RRPV_MAX);
            if (eviction_policy == 2) { // SHiP
              assert(SHCT->find(c->SHiP_sig) != SHCT->end()); // sig should be in SHCT
              if (!c->SHiP_outcome && (*SHCT)[c->SHiP_sig] > 0) { // evicted cacheline was not reused, SHCT needs to be decremented
                (*SHCT)[c->SHiP_sig]--;
              }
            } else if (eviction_policy == 3) { // Hawkeye
              if (isLeader == 0 || isLeader == 1) { // sampled set
                if(c->prefetching) {
                  predictor_prefetch->decrease(c->sample_sig);
                } else {
                  predictor_demand->decrease(c->sample_sig);
                }
              }
            } else if (eviction_policy == 7 || eviction_policy == 8 || eviction_policy == 9) { // Graphfire
              bool isPIA = false; 
              CacheLine* curr = c; // LRU
              while (curr != head) {
                isPIA = RRVT->find(curr->nodeId) != RRVT->end() && !(*learningRRVT) && (get<0>((*RRVT)[curr->nodeId]) < RRV_THRS && get<2>((*RRVT)[curr->nodeId]) == RRV_THRS);
                if (!isPIA) {
                  break;
                }
                curr = curr->prev;
              }
              if (curr != head) {
                c = curr;
              }
            }
          } 
        }
        assert(c!=head);
        if (!c->full_block) { // merged block got replaced with normal block
          //cout << "EVICTING MERGED BLOCK" << endl;
          (*evictedMergeBlock) = new CacheLine; // preserve cache line info
          (*evictedMergeBlock)->addr = c->addr;
          (*evictedMergeBlock)->offset = c->offset;
          (*evictedMergeBlock)->nodeId = c->nodeId;
          (*evictedMergeBlock)->dirty = c->dirty;
          (*evictedMergeBlock)->full_block = c->full_block;

          // deep copy sub blocks
          (*evictedMergeBlock)->subBlockHead = new CacheLine;
          (*evictedMergeBlock)->subBlockHead->prev = NULL;
          CacheLine* new_curr = (*evictedMergeBlock)->subBlockHead;
          CacheLine* curr = c->subBlockHead->next;
          base_tag_map.erase(curr->addr >> num_page_bits); // removing base
          int num_sb = 0;
          while (curr != c->subBlockTail) {
            CacheLine* new_cl = new CacheLine;
            new_cl->addr = curr->addr * setcount + curr_id;
            new_cl->offset = curr->offset;
            new_cl->nodeId = curr->nodeId;
            new_cl->graphNodeId = curr->graphNodeId;
            new_cl->graphNodeDeg = curr->graphNodeDeg;
            new_cl->rmw = curr->rmw;
            new_cl->dirty = curr->dirty;
            new_cl->prev = new_curr;            
            new_curr->next = new_cl;
            c->sub_tag_map.erase(getSubTag(curr->addr, curr->offset)); // removing sub-tag
            new_curr = new_curr->next;
            curr = curr->next;
            num_sb++;
          }
          assert(num_sb <= num_stmu_blocks);
          (*evictedMergeBlock)->subBlockTail = new CacheLine;
          (*evictedMergeBlock)->subBlockTail->next = NULL;
          (*evictedMergeBlock)->subBlockTail->prev = new_curr;
          new_curr->next = (*evictedMergeBlock)->subBlockTail;
          
          //clean up
          delete [] c->subBlockEntries;
          free(c->subBlockHead);
          free(c->subBlockTail);
          
          // Measured unused space
          for(int i=0; i<num_stmu_blocks; i++) {
            *unusedSpace+= c->used[i];
            c->used[i] = 0;
          }
          *unusedSpace = 64.0/num_stmu_blocks*(num_stmu_blocks-*unusedSpace); // unused space in bytes - NEED TO UPDATE COEFF
          deleteNode(c);
        } else {
          //cout << "EVICTING A NORMAL BLOCK WITH NODE ID " << c->nodeId << endl;
          addr_map.erase(c->addr); // removing tag

          *evictedAddr = c->addr;
          *evictedOffset = c->offset;
          *evictedNodeId = c->nodeId;
          *evictedGraphNodeId = c->graphNodeId;
          *evictedGraphNodeDeg = c->graphNodeDeg;
          *evictedRMW = c->rmw;
          *evictedFull = true;
          deleteNode(c);
 
          if(c->dirty) {
            *dirtyEvict = 1;
          } else {
            *dirtyEvict = 0;
          }
          c->dirty = false;
        
          size = num_addresses;
          // Measured unused space for normal block
          for(int i=0; i<size; i++) {
            *unusedSpace += c->used[i];
            c->used[i] = 0;
          }
          *unusedSpace = 4*(num_addresses-*unusedSpace); // unused space in bytes
        }
      }     
    } else { // there is space, no need for eviction
      c = freeEntries.back(); // Get from Free Entries
      freeEntries.pop_back();
    }

    if (!full_block) {  // inserting 4B into new block
      //cout << "CREATING NEW BLOCK WITH EMPTY SUB BLOCKS" << endl;
      // set up merged block
      CacheLine* curr;
      CacheLine* entries = new CacheLine[num_stmu_blocks];
      c->subBlockEntries = entries;
      c->freeSubBlocks.clear();
      for(int i=0; i<num_stmu_blocks; i++) {
        curr = &entries[i];
        c->freeSubBlocks.push_back(curr);
      }
      c->subBlockHead = new CacheLine;
      c->subBlockTail = new CacheLine;
      c->subBlockHead->prev = NULL;
      c->subBlockHead->next = c->subBlockTail;
      c->subBlockTail->next = NULL;
      c->subBlockTail->prev = c->subBlockHead;

      CacheLine* sb = c->freeSubBlocks.back();
      c->freeSubBlocks.pop_back();
      sb->addr = address;
      sb->offset = offset;
      sb->nodeId = nodeId;
      sb->graphNodeId = graphNodeId;
      sb->graphNodeDeg = graphNodeDeg;
      sb->rmw = isRMW;
      sb->numAccesses = 0;

      // Write-back cache insertion
      if (!isLoad) {
        sb->dirty = true; 
      } else {
        sb->dirty = false;
      }
 
      // Insert into address map
      uint64_t sub_tag = getSubTag(address, offset);
      c->sub_tag_map[sub_tag] = sb; // record sub tag
      if (eviction_policy == 8) {
        insertByNumAccesses(sb, c->subBlockHead, c->subBlockTail);
      } else if (eviction_policy == 9) {
        insertByDegree(sb, c->subBlockHead, c->subBlockTail);
      } else {
        insertFront(sb, c->subBlockHead); // update MRU for sub blocks
      }

      // update merged block data
      c->numAccesses = 0;
      if (eviction_policy == 10 || eviction_policy == 8) {
        insertByNumAccesses(c, head, tail); 
      } else if (eviction_policy == 9) {
        insertByDegree(c, head, tail);
      } else {
        insertFront(c, head); // update MRU for blocks
      }
      c->nodeId = nodeId;
      c->full_block = full_block; 
      c->dirty = false;
      base_tag_map[(address >> num_page_bits)] = c; // record merged block base tag
      c->sb_idx = 0;
      c->used[c->sb_idx] = 1;
      c->sb_idx = c->sb_idx + 1;
      return;
    } 

    //cout << "INSERTING NEW NORMAL BLOCK" << endl;
    // Insert into address map
    if (cache_by_signature == 1 && graphNodeId != -1) {
      llama_addr_map[(address << (log_linesize - log_llama_linesize)) + index] = c;
    } else {
      addr_map[address] = c;
    }

    c->addr = address;
    c->offset = offset;
    c->nodeId = nodeId;
    c->graphNodeId = graphNodeId;
    c->graphNodeDeg = graphNodeDeg;
    c->rmw = isRMW;
    c->numAccesses = 0;   

    if (graphNodeId != -1 && eviction_policy == 3) {
      if (access_freq_map.find(graphNodeId) == access_freq_map.end()) { // never seen node before
        access_freq_map[graphNodeId] = c->numAccesses;
      } else {
        access_freq_map[graphNodeId] += c->numAccesses;
      }
    }
    c->rrpv = RRPV_MAX-1;

    // Determine regularity  
    if (graphfire > 0) { 
      if (c->setid == -1) {
        c->setid = curr_id;
      } 
      assert(isLLC || RRVT->find(nodeId) != RRVT->end());
      if (!isPrefetch && *learningRRVT) {
        if (isL1 && !isLLC && get<0>((*RRVT)[nodeId]) > 0) {
          get<0>((*RRVT)[nodeId])--;
        }
        if (isL1 && !isLLC) {
          get<1>((*RRVT)[nodeId]) = c->setid;
        }
      }
      c->full_block = 1; //(*RRVT)[nodeId]->first > RRV_THRS;
    }

    // Write-back cache insertion
    if (!isLoad) {
      c->dirty = true; 
    } else {
      c->dirty = false;
    }
 
    // Track space used
    if (cache_by_signature == 1 && graphNodeDeg != -1) {
      c->used[0] = 1;
    } else {
      c->used[offset/4] = 1;
    }
    
    // Insertion policies
    if (cache_by_signature == 1) { // use evict by degree for llamas, LRU for non-llamas
      if (graphNodeDeg != -1) { // llama access
        if (eviction_policy == 0 || eviction_policy == 2) {
          insertFront(c, llamaHeads[index]);
        } else if (eviction_policy == 1) {
          insertByNumAccesses(c, llamaHeads[index], llamaTails[index]);
        } else if (eviction_policy == 3) {
          insertByWeightedAccesses(c, llamaHeads[index], llamaTails[index], access_freq_map);
        } else if (eviction_policy == 4) {
          insertByDegree(c, llamaHeads[index], llamaTails[index]);
        }
      } else {
        if (perfect_llama == 1) {
          if (eviction_policy == 0 || eviction_policy == 2) {
            insertFront(c, head);
          } else if (eviction_policy == 1) {
            insertByNumAccesses(c, head, tail);
          } else if (eviction_policy == 3) {
            insertByWeightedAccesses(c, head, tail, access_freq_map);
          }
        } else { 
          insertFront(c, head);
        }
      }
    } else {
      if (eviction_policy == 0 || eviction_policy == 7 || eviction_policy == 8 || eviction_policy == 9) {  // LRU FOR INSERTION
        insertFront(c, head);
      } else if (eviction_policy == 1) { // DRRIP
        if (isLLC) {
          if (isLeader == 0) { // BRRIP
            if (*PSEL > 0) {
              *PSEL = *PSEL - 1;
            }
            c->rrpv = RRPV_MAX;
            *bip_counter = *bip_counter + 1;
            if (*bip_counter == BIP_MAX) {
              *bip_counter = 0;
              c->rrpv = RRPV_MAX-1;
            }
          } else if (isLeader == 1) { // SRRIP
            if (*PSEL < PSEL_MAX) {
              *PSEL = *PSEL + 1;
            }
            c->rrpv = RRPV_MAX-1;
          } else { // follower
            if (*PSEL > PSEL_THRS) { // follow BRRIP
              c->rrpv = RRPV_MAX;
              *bip_counter = *bip_counter + 1;
              if (*bip_counter == BIP_MAX) {
                *bip_counter = 0;
                c->rrpv = RRPV_MAX-1;
              }
            } else { // follow SRRIP
              c->rrpv = RRPV_MAX-1;
            }
          }
        }
        insertFront(c, head);
      } else if (eviction_policy == 2) { // SHiP
        if (isLLC) {
          if (SHCT->find(nodeId) == SHCT->end()) { // add sig to SHCT
              (*SHCT)[nodeId] = 0;
              c->SHiP_sig = nodeId;
          }
          if ((*SHCT)[c->SHiP_sig] == 0) {
            c->rrpv = RRPV_MAX;
          }
        }
        insertFront(c, head);
      } else if (eviction_policy == 3) { // Hawkeye
        if (isLLC) {
          c->prefetching = isPrefetch;
          c->sample_sig = nodeId;
          if (isLeader == 0 || isLeader == 1) { // sampled set
            updateHawkeye(address, nodeId, isPrefetch);
          }
          bool prediction = predictor_demand->get_prediction(nodeId); // get Hawkeye prediction
          if (isPrefetch){
            prediction = predictor_prefetch->get_prediction(nodeId);
          }
          if (!prediction) {
            c->rrpv = RRPV_MAX;
          } else {
            c->rrpv = 0;
          }
        }
        insertFront(c, head);
      } else if (eviction_policy == 4) { // GRASP approx
        if (isLLC) {
          if (graphNodeId >= 0 && graphNodeDeg > node_degree_threshold) { // high reuse region
            c->rrpv = RRPV_P;
          } else { // moderate or low reuse region
            if (graphNodeId >= 0 && graphNodeDeg > l2_node_degree_threshold) { // moderate reuse region
              c->rrpv = RRPV_I;
            } else { // low reuse region
              c->rrpv = RRPV_MAX;
            }
          }
        }
        insertFront(c, head);
      } else if (eviction_policy == 5) { // GRASP
        if (isLLC) {
          if (graphNodeId >= 0 && graphNodeId < LLCsize) { // high reuse region
            c->rrpv = RRPV_P;
          } else { // moderate or low reuse region
            if (graphNodeId >= 0 && graphNodeId < 2*LLCsize) { // moderate reuse region
              c->rrpv = RRPV_I;
            } else { // low reuse region
              c->rrpv = RRPV_MAX;
            }
          }
        }
        insertFront(c, head);
      } else if (eviction_policy == 6) { // Graphfire INSERTION
        assert(graphfire > 0);
        if (*learningRRVT) {
          insertFront(c, head);
        } else {
          if (c->full_block) {
            insertRegular(c, head, tail);
          } else {
            insertFront(c, head); 
          }
        }
      } else if (eviction_policy == 10) { // FBR
        insertByNumAccesses(c, head, tail);
      }
    }
  }

  //returns isDirty to determine if you should store back data up cache hierarchy
  //designing evict functionality, unfinished!!!
  bool evict(uint64_t address, uint64_t offset, int64_t *evictedAddr, uint64_t *evictedOffset, int *evictedNodeId, int *evictedGraphNodeId, int *evictedGraphNodeDeg, bool *evictedRMW, float *unusedSpace, int *isFull) {
    CacheLine *c = addr_map[address]; 
      if(c) {
        *isFull = 1;
        assert(c->full_block);
        for(int i=0; i<num_addresses; i++)
        {
          *unusedSpace+=c->used[i];
          c->used[i] = 0;
        }
        *unusedSpace = 4*(num_addresses-*unusedSpace); // unused space in bytes

        addr_map.erase(address);
        deleteNode(c);
        freeEntries.push_back(c);
        *evictedAddr = c->addr;
        *evictedOffset = c->offset;
        *evictedNodeId = c->nodeId;
        *evictedGraphNodeId = c->graphNodeId;
        *evictedGraphNodeDeg = c->graphNodeDeg;
        *evictedRMW = c->rmw;
        if(c->dirty) { //you want to know if eviction actually took place
          c->dirty = false;
          return true;
        }
    } else {
      *isFull = 0;
      if (graphfire > 0 && stmu == 1 && !(*learningRRVT)) { // STMU can be used
        CacheLine* way = base_tag_map[(address >> num_page_bits)];
        if (way) { 
          uint64_t sub_tag = getSubTag(address, offset);
          CacheLine* sb = way->sub_tag_map[sub_tag];
          if (sb) {
            assert(sb!=way->subBlockHead);
            assert(sb!=way->subBlockTail);
            assert(!way->full_block);

            way->sub_tag_map.erase(sub_tag); // removing sub-tag
            deleteNode(sb);
            way->freeSubBlocks.push_back(sb);
            if ((int)way->freeSubBlocks.size() == num_stmu_blocks) {
              way->subBlockTail->addr = address;
            }
            way->sb_idx = way->sb_idx - 1;
            *evictedAddr = sb->addr;
            *evictedOffset = sb->offset;
            *evictedNodeId = sb->nodeId;
            *evictedGraphNodeId = sb->graphNodeId;
            *evictedGraphNodeDeg = sb->graphNodeDeg;
            *evictedRMW = sb->rmw;
            *unusedSpace = 0; 
            if(sb->dirty) {
              sb->dirty = false;
              return true;
            }
          }
        }
      }
    } 
    return false;
  }

  uint64_t getSubTag(uint64_t address, uint64_t offset) {
    uint64_t mask = (1 << (num_page_bits + 1)) - 1;
    uint64_t page_tag = address & mask;
    uint64_t block_tag = offset >> 2;
    uint64_t sub_tag = (page_tag << 4) + block_tag;
    return sub_tag;
  }

  // Insert such that MRU is first
  void insertFront(CacheLine *c, CacheLine *currHead)
  {
    c->next = currHead->next;
    c->prev = currHead;
    currHead->next = c;
    c->next->prev = c;
  }

  void insertRegular(CacheLine *c, CacheLine *currHead, CacheLine *currTail)
  {
    CacheLine* curr = currTail->prev;
    while (curr->full_block && curr != currHead) {
      curr = curr->prev;
    }
    c->next = curr->next;
    c->prev = curr;
    curr->next = c;
    c->next->prev = c;
  }
  
  // Insert such that highest degree is first
  void insertByDegree(CacheLine *c, CacheLine *currHead, CacheLine *currTail)
  {
    assert(c);
    CacheLine *curr = currHead->next;   
    while (curr != currTail && curr->graphNodeDeg > c->graphNodeDeg) {
      curr = curr->next;
    }
    c->next = curr;
    c->prev = curr->prev;
    curr->prev->next = c;
    curr->prev = c;
  }

  // Insert such that highest number of accesses is first
  void insertByNumAccesses(CacheLine *c, CacheLine *currHead, CacheLine *currTail)
  {
    assert(c);
    CacheLine *curr = currHead->next;   
    while (curr != currTail && curr->numAccesses > c->numAccesses) {
      curr = curr->next;
    }
    c->next = curr;
    c->prev = curr->prev;
    curr->prev->next = c;
    curr->prev = c;
  }

  // Insert such that highest number of accesses is first
  void insertByWeightedAccesses(CacheLine *c, CacheLine *currHead, CacheLine *currTail, std::unordered_map<int, int> access_freq_map)
  {
    CacheLine *curr = currHead->next;   
    while (curr != currTail && access_freq_map[curr->graphNodeId] > access_freq_map[c->graphNodeId]) {
      curr = curr->next;
    }
    c->next = curr;
    c->prev = curr->prev;
    curr->prev->next = c;
    curr->prev = c;
  }

  CacheLine* RRIP(CacheLine *currHead, CacheLine *currTail, int dist) {
    CacheLine* c = currTail->prev;
    while (c != currHead && c->rrpv != dist) {
      c = c->prev;
    }
    if (c == currHead) {
      c = currTail->prev;
      while (c != currHead) {
        if (c->rrpv < RRPV_MAX) {
          c->rrpv++;
        }
        c = c->prev;
      }
      return RRIP(currHead, currTail, dist);
    } else {
      return c;
    }
  }

  void update_cache_history(int curr_val) {
    for(std::unordered_map<int, addr_history>::iterator it = cache_history_sampler.begin(); it != cache_history_sampler.end(); it++){
      if((it->second).lru < curr_val){
        (it->second).lru++;
      }
    }
  }

  void updateHawkeye(int addr, int PC, bool isPrefetch) {
    int curr_val = set_timer % OPTGEN_SIZE;
    if (!isPrefetch && (cache_history_sampler.find(addr) != cache_history_sampler.end())) { // cache line has been used before, so ignore prefetching
      int curr_time = set_timer;
      if(curr_time < cache_history_sampler[addr].previousVal){
        curr_time += TIME;
      }
      int prev_val = cache_history_sampler[addr].previousVal % OPTGEN_SIZE;
      bool isWrap = (curr_time - cache_history_sampler[addr].previousVal) > OPTGEN_SIZE;

      // train positively since OPTgen predicts hit
      if(!isWrap && optgen_occup_vector.is_cache(curr_val, prev_val)){ 
        if(cache_history_sampler[addr].prefetching){ // prefetch
          predictor_prefetch->increase(cache_history_sampler[addr].PCval);
        } else { // demand
          predictor_demand->increase(cache_history_sampler[addr].PCval);
        }
      } else { // train negatively since OPTgen predicts miss
        if(cache_history_sampler[addr].prefetching){ // prefetch
          predictor_prefetch->decrease(cache_history_sampler[addr].PCval);
        } else { // demand
          predictor_demand->decrease(cache_history_sampler[addr].PCval);
        }
      }

      optgen_occup_vector.set_access(curr_val);
      update_cache_history(cache_history_sampler[addr].lru);
      cache_history_sampler[addr].prefetching = false; // demand access
    } else if (cache_history_sampler.find(addr) != cache_history_sampler.end()) { // cache line has not been used before
      if (cache_history_sampler.size() == SAMPLER_HIST) { // need to replace in cache history
        int addr_val;
        for(std::unordered_map<int, addr_history>::iterator it = cache_history_sampler.begin(); it != cache_history_sampler.end(); it++){
          if((it->second).lru == (SAMPLER_HIST-1)){ // find LRU cache history entry
            addr_val = it->first;
            break;
          }
        }
        cache_history_sampler.erase(addr_val);
      }

      cache_history_sampler[addr].init(); // create new entry
      if (isPrefetch) { // mark as prefetch
        cache_history_sampler[addr].set_prefetch();
      } 
      optgen_occup_vector.set_access(curr_val);
      update_cache_history(SAMPLER_HIST-1);
    } else { // current access is a prefetch
      int prev_val = cache_history_sampler[addr].previousVal % OPTGEN_SIZE;
      if (set_timer - cache_history_sampler[addr].previousVal < 5) {
        if (optgen_occup_vector.is_cache(curr_val, prev_val)) { // OPTgen predicts hit
          if (cache_history_sampler[addr].prefetching) { // prefetch
            predictor_prefetch->increase(cache_history_sampler[addr].PCval);
          } else { // demand access
            predictor_demand->increase(cache_history_sampler[addr].PCval);
          }
        }
      }
      cache_history_sampler[addr].set_prefetch();
      optgen_occup_vector.set_access(curr_val);
      update_cache_history(cache_history_sampler[addr].lru);
    }
    cache_history_sampler[addr].update(set_timer, PC);
    cache_history_sampler[addr].lru = 0;
    set_timer = (set_timer + 1) % TIME;
  }

  void setGraphfire(unordered_map<int, tuple<int, int, int>>* RRVT_input, bool* learningRRVT_input) {
    RRVT = RRVT_input;
    learningRRVT = learningRRVT_input;
  }

  // Insert such that highest number of accesses is first
  void insertAFT(AFTEntry *e, AFTEntry *currHead, AFTEntry *currTail)
  {
    AFTEntry *curr = currHead->next;   
    while (curr != currTail && curr->count > e->count) {
      curr = curr->next;
    }
    e->next = curr;
    e->prev = curr->prev;
    curr->prev->next = e;
    curr->prev = e;
  }

  void addToAFT(uint64_t address, int graphNodeId, AFTEntry *currHead, AFTEntry *currTail) {
    AFTEntry* new_entry;
    if (AFT->size() == AFT_SIZE) {
      new_entry = currTail->prev;
      uint64_t min_address = new_entry->address;
      AFT->erase(min_address);
      deleteNodeAFT(new_entry);
    } else {
      new_entry = new AFTEntry;
    }
    new_entry->address = address;
    new_entry->graphNodeId = graphNodeId;
    new_entry->count = 0;
    (*AFT)[address] = new_entry;
    insertAFT(new_entry, currHead, currTail);

    /*AFTEntry* curr = currHead->next;
    cout << "printing AFT" << endl;
    while(curr != currTail) {
      cout << "SIZE, " << AFT->size() << ", ADDRESS: " << curr->address << ", GRAPH NODE: " << curr->graphNodeId << ", COUNT: " << curr->count << endl;
      curr = curr->next;
    }*/
  }

  void deleteNode(CacheLine *c)
  {
    assert(c->next);
    assert(c->prev);
    c->prev->next = c->next;
    c->next->prev = c->prev;
  }
  
  void deleteNodeAFT(AFTEntry *e)
  {
    e->prev->next = e->next;
    e->next->prev = e->prev;
  }
};

class FunctionalCache
{
public:
  int line_count;
  int set_count;
  int log_set_count;
  int cache_line_size;
  int log_line_size;
  std::vector<CacheSet*> sets;
  int cache_by_signature;
  int perfect_llama;
  int llama_cache_line_size;
  int log_llama_line_size;

  int SDM_SIZE = 32;

  FunctionalCache(int size, int assoc, int line_size, int llama_line_size, int cache_by_signature_input, int partition_ratio, int perfect_llama_input, int eviction_policy, int cache_by_temperature, int node_degree_threshold, int l2_node_degree_threshold, bool isLLC, bool isL1, int graphfire_input, int stmu_input, int num_stmu_blocks_input, int aft_max_input)
  {
    cache_by_signature = cache_by_signature_input;
    perfect_llama = perfect_llama_input;
    cache_line_size = line_size;
    llama_cache_line_size = llama_line_size;
    line_count = size / cache_line_size;
    set_count = line_count / assoc;
    log_set_count = log2(set_count);
    log_line_size = log2(cache_line_size);
    log_llama_line_size = log2(llama_cache_line_size);

    //DRRIP
    std::vector<int> leaderIDs;
    int PSEL = 0;
    int bip_counter = 0;
    int isLeader = -1;
    if ((eviction_policy == 1 || eviction_policy == 3) && isLLC) {
      int id;
      srand(time(NULL));
      for(int i = 0; i < 2*SDM_SIZE; i++) {
        //Generate random set IDs for SRRIP and BRRIP sets
        id = rand() % set_count;  
        while(std::find(leaderIDs.begin(), leaderIDs.end(), id) != leaderIDs.end()) { //set ID is already a leader
          id = rand() % set_count;
        }
        leaderIDs.push_back(id);         
      }
    } 

    //SHiP
    unordered_map<int, int>* SHCT = new unordered_map<int, int>();
    
    //Hawkeye
    Hawkeye_Predictor* predictor_demand = new Hawkeye_Predictor();
    Hawkeye_Predictor* predictor_prefetch = new Hawkeye_Predictor();
   
    // Graphfire
    unordered_map<uint64_t, AFTEntry*>* AFT = new unordered_map<uint64_t, AFTEntry*>();
    AFTEntry* AFT_head = new AFTEntry;
    AFTEntry* AFT_tail = new AFTEntry;
    AFT_head->prev = NULL;
    AFT_head->next = AFT_tail;
    AFT_tail->prev = AFT_head;
    AFT_tail->next = NULL;
 
    for(int i=0; i<set_count; i++)
    {
      if ((eviction_policy == 1 || eviction_policy == 3) && isLLC) {
        std::vector<int>::iterator it = std::find(leaderIDs.begin(), leaderIDs.end(), i);
        int idx;
        if(it != leaderIDs.end()) { // leader set
          idx = std::distance(leaderIDs.begin(), it);
          if (idx < SDM_SIZE) {
            isLeader = 0;
          } else {
            isLeader = 1;
          }
        } else { // follower set
          isLeader = -1;
        }
      }
      CacheSet* set = new CacheSet(assoc, cache_line_size, llama_cache_line_size, cache_by_signature, partition_ratio, eviction_policy, cache_by_temperature, node_degree_threshold, l2_node_degree_threshold, perfect_llama, size/4, isLLC, isL1, isLeader, &PSEL, &bip_counter, SHCT, predictor_demand, predictor_prefetch, set_count, i, graphfire_input, stmu_input, num_stmu_blocks_input, aft_max_input, AFT, &AFT_head, &AFT_tail);
      sets.push_back(set);
      //sets.push_back(new CacheSet(assoc, cache_line_size, llama_cache_line_size, cache_by_signature, partition_ratio, eviction_policy, cache_by_temperature, node_degree_threshold, l2_node_degree_threshold, perfect_llama, size/4, isLLC, isL1, isLeader, &PSEL, &bip_counter, predictor_demand, predictor_prefetch));
    }
  } 

  uint64_t extract(int max, int min, uint64_t address) // inclusive
  {
      uint64_t maxmask = ((uint64_t)1 << (max+1))-1;
      uint64_t minmask = ((uint64_t)1 << (min))-1;
      uint64_t mask = maxmask - minmask;
      uint64_t val = address & mask;
      val = val >> min;
      return val;
  }

  bool access(uint64_t address, int nodeId, int graphNodeId, int graphNodeDeg, bool isLoad, bool isRMW, bool isPrefetch, bool isFull)
  {
    //cout << "accessing " << address << " " << set_count << ", is full = " << isFull << endl;
    if (perfect_llama == 1 && graphNodeId != -1) {
      return true;
    }

    uint64_t offset = extract(log_line_size-1, 0, address);
    uint64_t setid = extract(log_set_count+log_line_size-1, log_line_size, address);
    uint64_t tag = extract(63, log_set_count+log_line_size, address);
    CacheSet *c = sets.at(setid);
    bool res = c->access(tag, offset, nodeId, graphNodeId, graphNodeDeg, isLoad, isRMW, isPrefetch, isFull);

    return res;
  }

  bool check_full_block(uint64_t address)
  {
    uint64_t offset = extract(log_line_size-1, 0, address);
    uint64_t setid = extract(log_set_count+log_line_size-1, log_line_size, address);
    uint64_t tag = extract(63, log_set_count+log_line_size, address);
    CacheSet *c = sets.at(setid);
    bool res = c->check_full_block(tag, offset);
    return res;
  }

  void insert(uint64_t address, int nodeId, int graphNodeId, int graphNodeDeg, bool isLoad, bool isRMW, bool isPrefetch, bool isFull, int *dirtyEvict, int64_t *evictedAddr, uint64_t *evictedOffset, int *evictedNodeId, int *evictedGraphNodeId, int *evictedGraphNodeDeg, float *unusedSpace, CacheLine *mergedBlock, CacheLine **evictedMergeBlock, bool *evictedFull, bool *evictedRMW)
  {
    uint64_t offset = extract(log_line_size-1, 0, address);
    uint64_t setid = extract(log_set_count-1+log_line_size, log_line_size, address);
    uint64_t tag = extract(63, log_set_count+log_line_size, address);
    CacheSet *c = sets.at(setid);
    int64_t evictedTag = -1;

    //cout << "inserting " << address << " " << set_count << ", is full = " << isFull << ", set = " << setid << ", isLoad = " << isLoad << endl;
    c->insert(tag, offset, nodeId, graphNodeId, graphNodeDeg, isLoad, isRMW, isPrefetch, isFull, dirtyEvict, &evictedTag, evictedOffset, evictedNodeId, evictedGraphNodeId, evictedGraphNodeDeg, unusedSpace, mergedBlock, evictedMergeBlock, evictedFull, evictedRMW);

    if(evictedAddr && evictedTag != -1) {
      *evictedAddr = evictedTag * set_count + setid;
    }
  }

  // reserved for decoupling
  bool evict(uint64_t address, int64_t *evictedAddr, uint64_t *evictedOffset, int *evictedNodeId, int *evictedGraphNodeId, int *evictedGraphNodeDeg, bool *evictedRMW, float *unusedSpace, int *isFull) { 
    uint64_t offset = extract(log_line_size-1, 0, address);
    uint64_t setid = extract(log_set_count-1+log_line_size, log_line_size, address); 
    uint64_t tag = extract(63, log_set_count+log_line_size, address);
    CacheSet *c = sets.at(setid);
    int64_t evictedTag = -1;
    
    bool dirtyEvict = c->evict(tag, offset, &evictedTag, evictedOffset, evictedNodeId, evictedGraphNodeId, evictedGraphNodeDeg, evictedRMW, unusedSpace, isFull);

    if(evictedAddr && evictedTag != -1)
      *evictedAddr = evictedTag * set_count + setid;

    return dirtyEvict;
  }
 
  void setGraphfire(unordered_map<int, tuple<int, int, int>>* RRVT_input, bool* learningRRVT_input /*, unordered_map<uint64_t, AFTEntry**>* AFT, AFTEntry** AFT_head, AFTEntry** AFT_tail, int* num_AFT_entries*/) {
    for (std::vector<CacheSet*>::iterator it = sets.begin(); it != sets.end(); ++it) {
      CacheSet* set = *it;
      set->setGraphfire(RRVT_input, learningRRVT_input/*, AFT, AFT_head, AFT_tail, num_AFT_entries*/);
    }
  }
};
