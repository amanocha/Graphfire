#include "DRAM.h"
#include "Cache.h"
#include "../tile/Core.h"
using namespace std;
#define UNUSED 0

string dram_accesses="dram_accesses";
string dram_reads_loads="dram_reads_loads";
string dram_reads_stores="dram_reads_stores";
string dram_writes_evictions="dram_writes_evictions";
string dram_bytes_accessed="dram_bytes_accessed";
string dram_total_read_latency="dram_total_read_latency";
string dram_total_write_latency="dram_total_write_latency";

void DRAMSimInterface::read_complete(unsigned id, uint64_t addr, uint64_t clock_cycle) {

  //assert(outstanding_read_map.find(addr) != outstanding_read_map.end());

  if(UNUSED)
    cout << id << clock_cycle;

  if (outstanding_read_map.find(addr) == outstanding_read_map.end()) {
    return;
  }

  queue<pair<Transaction*, uint64_t>> &q = outstanding_read_map.at(addr);

  while(q.size() > 0) {

    pair<Transaction*, uint64_t> entry = q.front();

    stat.update(dram_total_read_latency, clock_cycle-entry.second);

    MemTransaction* t = static_cast<MemTransaction*>(entry.first);
    int nodeId = t->d->n->id;
    int graphNodeId = t->graphNodeId;
    int graphNodeDeg = t->graphNodeDeg;
    int dirtyEvict = -1;
    int64_t evictedAddr = -1;
    uint64_t evictedOffset = 0;
    int evictedNodeId = -1;
    int evictedGraphNodeId = -1;
    int evictedGraphNodeDeg = -1;
    float unusedSpace = 0;
    CacheLine* evictedMergeBlock = NULL;
    bool evictedFull = false;
    bool evictedRMW = false;
    bool clean = t->isLoad;

    Cache* c = t->cache_q->front();
    
    t->cache_q->pop_front();
    if (!t->cache_q->empty()) { //upper levels to go
      clean = true;
    }

    c->fc->insert(t->addr, nodeId, graphNodeId, graphNodeDeg, clean /*|| t->isRMW*/, t->isRMW, t->isPrefetch, t->isFull, &dirtyEvict, &evictedAddr, &evictedOffset, &evictedNodeId, &evictedGraphNodeId, &evictedGraphNodeDeg, &unusedSpace, NULL, &evictedMergeBlock, &evictedFull, &evictedRMW);

    if(evictedAddr!=-1) {
      uint64_t eAddr = evictedAddr*c->size_of_cacheline + evictedOffset;

      int cacheline_size;
      if (c->cacheBySignature == 0 || evictedGraphNodeDeg == -1) {
        cacheline_size = c->size_of_cacheline;
      } else {
        cacheline_size = 4;
      }

      assert(evictedAddr >= 0 || evictedMergeBlock != NULL);
      stat.update("cache_evicts");
      if (c->isL1) {
        stat.update("l1_evicts");
        c->core->local_stat.update("l1_evicts");
      } else if (c->isLLC) {
        if (c->useL2) {
          stat.update("l3_evicts");
        } else {
          stat.update("l2_evicts");
        }
      } else {
        stat.update("l2_evicts");
        c->core->local_stat.update("l2_evicts");
      }

      if (dirtyEvict) {
        c->to_evict.push_back(make_tuple(eAddr, t->id, evictedNodeId, evictedNodeId, evictedGraphNodeId, evictedGraphNodeDeg, cacheline_size, evictedFull, false, evictedRMW));
      
        if (c->isL1) {
          stat.update("l1_dirty_evicts");
          c->core->local_stat.update("l1_dirty_evicts");
        } else if (c->isLLC) {
          c->child_cache->evict(eAddr, true, t->id);
          if (c->useL2) {
            stat.update("l3_dirty_evicts");
            c->child_cache->child_cache->evict(eAddr, true, t->id);
          } else {
            stat.update("l2_dirty_evicts");
          }
        } else {
          stat.update("l2_dirty_evicts");
          c->core->local_stat.update("l2_dirty_evicts");
          c->child_cache->evict(eAddr, true, t->id);
        }
      } else {
        if (c->isL1) {
          stat.update("l1_clean_evicts");
          c->core->local_stat.update("l1_clean_evicts");
        } else if (c->isLLC) {
          c->child_cache->evict(eAddr, true, t->id);
          if (c->useL2) {
            stat.update("l3_clean_evicts");
            c->child_cache->child_cache->evict(eAddr, true, t->id);
          } else {
            stat.update("l2_clean_evicts");
          }
        } else {
          stat.update("l2_clean_evicts");
          c->core->local_stat.update("l2_clean_evicts");
          c->child_cache->evict(eAddr, true, t->id);
        }
      }

      if (evictedMergeBlock != NULL) { // a merged block was evicted, need to evict each individual sub-block
        assert(!evictedMergeBlock->full_block);
        assert(!dirtyEvict);
        CacheLine* curr = evictedMergeBlock->subBlockTail->prev;
        bool isMerged = false;
        while(curr != evictedMergeBlock->subBlockHead) {
          uint64_t eAddr = curr->addr*c->size_of_cacheline + curr->offset;
          if (curr->dirty) {
            c->to_evict.push_back(make_tuple(eAddr, t->id, curr->nodeId, curr->nodeId, curr->graphNodeId, curr->graphNodeDeg, 4, false, isMerged, curr->rmw));
            
            if (c->isL1) {
              stat.update("l1_dirty_evicts");
              c->core->local_stat.update("l1_dirty_evicts");
            } else if (c->isLLC) {
              c->child_cache->evict(eAddr, true, t->id);
              if (c->useL2) {
                stat.update("l3_dirty_evicts");
                c->child_cache->child_cache->evict(eAddr, true, t->id);
              } else {
                stat.update("l2_dirty_evicts");
              }
            } else {
              stat.update("l2_dirty_evicts");
              c->core->local_stat.update("l2_dirty_evicts");
              c->child_cache->evict(eAddr, true, t->id);
            }

            if (sim->recordEvictions) {
              cacheStat cache_stat;
              cache_stat.cacheline = curr->addr*c->size_of_cacheline + curr->offset;
              cache_stat.cycle = c->cycles;
              cache_stat.offset = curr->offset;
              cache_stat.nodeId = curr->nodeId;
              cache_stat.newNodeId = nodeId;
              cache_stat.graphNodeId = curr->graphNodeId;
              cache_stat.graphNodeDeg = curr->graphNodeDeg;
              if (curr == evictedMergeBlock->subBlockTail->prev) { // check just the first one
                cache_stat.unusedSpace = unusedSpace;
              } else {
                cache_stat.unusedSpace = 0;
              }
              if (c->isLLC && !c->isL1) {
                cache_stat.cacheLevel = 3;
              } else if (c->isL1) {
                cache_stat.cacheLevel = 1;
              } else {
                cache_stat.cacheLevel = 2;
              }
              sim->evictStatsVec.push_back(cache_stat);         
              //cout << "CHECK 3: MERGED BLOCK, " << cache_stat.cacheline << ", " << unusedSpace << ", " << cache_stat.offset << ", " << cache_stat.nodeId << endl;
            }
            isMerged = true;
          } else {
            if (c->isL1) {
              stat.update("l1_clean_evicts");
              c->core->local_stat.update("l1_clean_evicts");
            } else if (c->isLLC) {
              c->child_cache->evict(eAddr, true, t->id);
              if (c->useL2) {
                stat.update("l3_clean_evicts");
                c->child_cache->child_cache->evict(eAddr, true, t->id);
              } else {
                stat.update("l2_clean_evicts");
              }
            } else {
              stat.update("l2_clean_evicts");
              c->core->local_stat.update("l2_clean_evicts");
              c->child_cache->evict(eAddr, true, t->id);
            }
          }
          curr = curr->prev;
          free(curr->next);
        }
        free(evictedMergeBlock->subBlockTail);
        free(evictedMergeBlock->subBlockHead);
      } else {
        if (sim->recordEvictions) {    
          cacheStat cache_stat;
          cache_stat.cacheline = t->addr*c->size_of_cacheline + evictedOffset;
          cache_stat.cycle = c->cycles;
          cache_stat.offset = evictedOffset;
          cache_stat.nodeId = evictedNodeId;
          cache_stat.newNodeId = nodeId;
          cache_stat.graphNodeId = evictedGraphNodeId;
          cache_stat.graphNodeDeg = evictedGraphNodeDeg;
          cache_stat.unusedSpace = unusedSpace;
          if (c->isL1) {
            cache_stat.cacheLevel = 1;
          } else if (c->isLLC) {
            if (c->useL2) {
              cache_stat.cacheLevel = 3;
            } else {
              cache_stat.cacheLevel = 2;
            }
          } else {
            cache_stat.cacheLevel = 2;
          }
          sim->evictStatsVec.push_back(cache_stat); 
          //cout << "CHECK 3: NORMAL BLOCK, " << cache_stat.cacheline << ", " << unusedSpace << ", " << cache_stat.offset << ", " << cache_stat.nodeId << endl;
        } 
      }
    }
   
    c->TransactionComplete(t);
    q.pop();      
  }
  if(q.size() == 0)
    outstanding_read_map.erase(addr);
}

void DRAMSimInterface::write_complete(unsigned id, uint64_t addr, uint64_t clock_cycle) {
  if(UNUSED)
    cout << id << addr << clock_cycle;

  assert(outstanding_write_map.find(addr) != outstanding_write_map.end());
  
  queue<pair<Transaction*, uint64_t>> &q = outstanding_write_map.at(addr);

  //while(q.size() > 0) {
  pair<Transaction*, uint64_t> entry = q.front();
  stat.update(dram_total_write_latency, clock_cycle-entry.second);
  q.pop();
  //}

  if(q.size() == 0)
    outstanding_write_map.erase(addr);
}

void DRAMSimInterface::addTransaction(Transaction* t, uint64_t addr, bool isRead, int cacheline_size, uint64_t issueCycle) {
  if(t!=NULL) { // this is a LD or a ST (whereas a NULL transaction means it is really an eviction)
                // Note that caches are write-back ones, so STs also "read" from dram 
    free_read_ports--;

    if(outstanding_read_map.find(addr) == outstanding_read_map.end()) {
      outstanding_read_map.insert(make_pair(addr, queue<pair<Transaction*, uint64_t>>()));      
    }
    outstanding_read_map.at(addr).push(make_pair(t, issueCycle));

    if(cfg.SimpleDRAM) {       
      simpleDRAM->addTransaction(false,addr);
    } else {
      mem->addTransaction(false, addr);
    }
      
    if(isRead) {
      stat.update(dram_reads_loads);
    } else {
      stat.update(dram_reads_stores);
    }
  } else { //eviction -> write iinto DRAM
    free_write_ports--;

    if(outstanding_write_map.find(addr) == outstanding_write_map.end()) {
      outstanding_write_map.insert(make_pair(addr, queue<pair<Transaction*, uint64_t>>()));
    }
    outstanding_write_map.at(addr).push(make_pair(t, issueCycle));

    if(cfg.SimpleDRAM) {
      simpleDRAM->addTransaction(true, addr);
    } else {
      mem->addTransaction(true, addr);
    }
    stat.update(dram_writes_evictions);
  }
  stat.update(dram_accesses);
  stat.update(dram_bytes_accessed,cacheline_size);
  sim->curr_bytes_accessed += cacheline_size;
}

bool DRAMSimInterface::willAcceptTransaction(uint64_t addr, bool isRead) {
  if((free_read_ports == 0 && isRead && read_ports!=-1)  || (free_write_ports == 0 && !isRead && write_ports!=-1))
    return false;
  else if(cfg.SimpleDRAM)
    return simpleDRAM->willAcceptTransaction(addr); 
  else
    return mem->willAcceptTransaction(addr);
}
