#include "../tile/DynamicNode.h"
#include "Cache.h"
#include "DRAM.h"
#include "../tile/Core.h"

string l1_accesses="l1_accesses"; // loads + stores
string l1_hits="l1_hits"; // load_hits + store_hits
string l1_misses="l1_misses"; // load_misses + store_misses
string l1_primary_misses="l1_primary_misses"; // primary_load_misses + primary_store_misses
string l1_secondary_misses="l1_secondary_misses"; // secondary_load_misses + secondary_store_misses
string l1_loads="l1_loads"; // load_hits + load_misses
string l1_load_hits="l1_load_hits";
string l1_load_misses="l1_load_misses";
string l1_primary_load_misses="l1_primary_load_misses";
string l1_secondary_load_misses="l1_secondary_load_misses";
string l1_stores="l1_stores"; // store_hits + store_misses
string l1_store_hits="l1_store_hits";
string l1_store_misses="l1_store_misses";
string l1_evictions="l1_evictions";
string l1_eviction_hits="l1_eviction_hits";
string l1_eviction_misses="l1_eviction_misses";
string l1_primary_store_misses="l1_primary_store_misses";
string l1_secondary_store_misses="l1_secondary_store_misses";
string l1_prefetches="l1_prefetches"; // prefetch_hits + prefetch_misses
string l1_prefetch_hits="l1_prefetch_hits";
string l1_prefetch_misses="l1_prefetch_misses";
string l1_rmws="l1_rmws";
string l1_rmw_hits="l1_rmw_hits";
string l1_rmw_misses="l1_rmw_misses";
string l1_total_accesses="l1_total_accesses"; // l1_accesses + l1_prefetches
string l1_dirty_evicts="l1_dirty_evicts";
string l1_clean_evicts="l1_clean_evicts";
string l1_evicts="l1_evicts";

string l2_accesses="l2_accesses"; // loads + stores
string l2_hits="l2_hits"; // load_hits + store_hits
string l2_misses="l2_misses"; // load_misses + store_misses
string l2_loads="l2_loads"; // load_hits + load_misses
string l2_load_hits="l2_load_hits";
string l2_load_misses="l2_load_misses";
string l2_stores="l2_stores"; // store_hits + store_misses
string l2_store_hits="l2_store_hits";
string l2_store_misses="l2_store_misses";
string l2_evictions="l2_evictions";
string l2_eviction_hits="l2_eviction_hits";
string l2_eviction_misses="l2_eviction_misses";
string l2_prefetches="l2_prefetches"; // prefetch_hits + prefetch_misses
string l2_prefetch_hits="l2_prefetch_hits";
string l2_prefetch_misses="l2_prefetch_misses";
string l2_rmws="l2_rmws";
string l2_rmw_hits="l2_rmw_hits";
string l2_rmw_misses="l2_rmw_misses";
string l2_rmw_misses_full="l2_rmw_misses_full";
string l2_rmw_misses_merged="l2_rmw_misses_merged";
string l2_total_accesses="l2_total_accesses"; // l2_accesses + l2_prefetches
string l2_dirty_evicts="l2_dirty_evicts";
string l2_clean_evicts="l2_clean_evicts";
string l2_evicts="l2_evicts";

string l3_accesses="l3_accesses"; // loads + stores
string llc_local_accesses="llc_local_accesses";
string llc_remote_accesses="llc_remote_accesses";
string l3_hits="l3_hits"; // load_hits + store_hits
string l3_misses="l3_misses"; // load_misses + store_misses
string l3_loads="l3_loads"; // load_hits + load_misses
string l3_load_hits="l3_load_hits";
string l3_load_misses="l3_load_misses";
string l3_stores="l3_stores"; // store_hits + store_misses
string l3_store_hits="l3_store_hits";
string l3_store_misses="l3_store_misses";
string l3_evictions="l3_evictions";
string l3_eviction_hits="l3_eviction_hits";
string l3_eviction_misses="l3_eviction_misses";
string l3_prefetches="l3_prefetches"; // prefetch_hits + prefetch_misses
string l3_prefetch_hits="l3_prefetch_hits";
string l3_prefetch_misses="l3_prefetch_misses";
string l3_rmws="l3_rmws";
string l3_rmw_hits="l3_rmw_hits";
string l3_rmw_misses="l3_rmw_misses";
string l3_total_accesses="l3_total_accesses"; // l3_accesses + l3_prefetches
string l3_dirty_evicts="l3_dirty_evicts";
string l3_clean_evicts="l3_clean_evicts";
string l3_evicts="l3_evicts";

string cache_evicts="cache_evicts";
string cache_access="cache_access";
string dram_accesses_bypass="dram_accesses_bypass";
string dram_accesses_l1_bypass="dram_accesses_l1_bypass";
string dram_accesses_l2_bypass="dram_accesses_l2_bypass";
string dram_accesses_l3="dram_accesses_l3";
string dram_accesses_bypass_forwards="dram_accesses_bypass_forwarded_evicts";
string dram_accesses_l1_bypass_forwards="dram_accesses_l1_bypass_forwarded_evicts";
string dram_accesses_l2_bypass_forwards="dram_accesses_l2_bypass_forwarded_evicts";
string dram_accesses_l3_forwards="dram_accesses_l3_forwarded_evicts";
string dram_accesses_bypass_evicts="dram_accesses_bypass_evicts";
string dram_accesses_l1_bypass_evicts="dram_accesses_l1_bypass_evicts";
string dram_accesses_l2_bypass_evicts="dram_accesses_l2_bypass_evicts";
string dram_accesses_l3_evicts="dram_accesses_l3_evicts";
string dram_accesses_rmw_evicts="dram_accesses_rmw_evicts";
string dram_accesses_back_invalidations="dram_accesses_back_invalidations";

string l1_cache_invalidations="l1_cache_invalidations";
string l1_cache_invalidations_dirty="l1_cache_invalidations_dirty";
string l1_cache_invalidations_clean="l1_cache_invalidations_clean";
string l1_cache_invalidations_full="l1_cache_invalidations_full";
string l1_cache_invalidations_full_dirty="l1_cache_invalidations_full_dirty";
string l1_cache_invalidations_full_clean="l1_cache_invalidations_full_clean";
string l1_cache_invalidations_merged="l1_cache_invalidations_merged";
string l1_cache_invalidations_merged_dirty="l1_cache_invalidations_merged_dirty";
string l1_cache_invalidations_merged_clean="l1_cache_invalidations_merged_clean";
string l2_cache_invalidations="l2_cache_invalidations";
string l2_cache_invalidations_dirty="l2_cache_invalidations_dirty";
string l2_cache_invalidations_clean="l2_cache_invalidations_clean";
string l2_cache_invalidations_full="l2_cache_invalidations_full";
string l2_cache_invalidations_full_dirty="l2_cache_invalidations_full_dirty";
string l2_cache_invalidations_full_clean="l2_cache_invalidations_full_clean";
string l2_cache_invalidations_merged="l2_cache_invalidations_merged";
string l2_cache_invalidations_merged_dirty="l2_cache_invalidations_merged_dirty";
string l2_cache_invalidations_merged_clean="l2_cache_invalidations_merged_clean";

void Cache::evict(uint64_t addr, bool backInvalidation, int id) {
  int64_t evictedAddr = -1;
  uint64_t evictedOffset = 0;
  int evictedNodeId = -1;
  int evictedGraphNodeId = -1;
  int evictedGraphNodeDeg = -1;
  bool evictedRMW = false;
  float unusedSpace = 0;
  int isFull = 0;

  assert(!isLLC);
  assert(core);

  if (backInvalidation) {
    return;
  }

  if(fc->evict(addr, &evictedAddr, &evictedOffset, &evictedNodeId, &evictedGraphNodeId, &evictedGraphNodeDeg, &evictedRMW, &unusedSpace, &isFull)) { //evicts from the cache returns isDirty, in which case must write back to L2
    MemTransaction* t = new MemTransaction(id, evictedNodeId, evictedNodeId, addr, false, evictedGraphNodeId, evictedGraphNodeDeg);
    t->isEviction = true;
    t->isFull = isFull;
    t->isRMW = evictedRMW;
    
    if (isL1) {
      stat.update(l1_dirty_evicts);
      core->local_stat.update(l1_dirty_evicts);
    } else {
      stat.update(l2_dirty_evicts);
      if (useL2) {
        core->local_stat.update(l2_dirty_evicts);
      }
    }

    bool bypass_hierarchy = false;
    int cacheline_size = 64;
    /*if(graphfire > 0 && !sim->learningRRVT) {
      int nodeId = evictedNodeId;
      bool isPIA = stmu == 1 && get<0>((*(sim->RRVT))[nodeId]) < RRV_THRS && get<2>((*(sim->RRVT))[nodeId]) == RRV_THRS;
      bool irreg_bypass = isL1 && !isPIA;
      bypass_hierarchy = isFull || irreg_bypass; //sim->RRVT[nodeId]->first >= RRV_THRS;
      if (stmu == 1 && !isFull) {
        cacheline_size = 4;
      }
    }*/
    
    if(bypass_hierarchy || backInvalidation) {
      if (backInvalidation) {
        stat.update(dram_accesses_back_invalidations);
        core->local_stat.update(dram_accesses_back_invalidations);
      } else {
        stat.update(dram_accesses_rmw_evicts);
        core->local_stat.update(dram_accesses_rmw_evicts);
      }
      if (memInterface->willAcceptTransaction(addr, false)) { //forwarded evict1ion, will be treated as just a write, nothing to do
        memInterface->addTransaction(NULL, addr, false, cacheline_size, cycles);
        //collect mlp stats
        sim->curr_epoch_accesses++;
        //it = to_send.erase(it);
        delete t;           
      } else {
        to_send.push_back(t);
      }
    } else {
      // if(parent_cache->willAcceptTransaction(t)) {
      t->cache_q->push_front(this);
      if (!backInvalidation && isL1) {
        parent_cache->parent_cache->addTransaction(t);
      } else { 
        parent_cache->addTransaction(t); //send eviction to parent cache
      }
    }
    
    if (isL1) {
      stat.update(l1_cache_invalidations_dirty);
      core->local_stat.update(l1_cache_invalidations_dirty);
      if (isFull == 1) {
        stat.update(l1_cache_invalidations_full_dirty);  
        core->local_stat.update(l1_cache_invalidations_full_dirty);  
      } else {
        stat.update(l1_cache_invalidations_merged_dirty);
        core->local_stat.update(l1_cache_invalidations_merged_dirty);
      }
    } else {
      stat.update(l2_cache_invalidations_dirty);
      core->local_stat.update(l2_cache_invalidations_dirty);
      if (isFull == 1) {
        stat.update(l2_cache_invalidations_full_dirty);  
        core->local_stat.update(l2_cache_invalidations_full_dirty);  
      } else {
        stat.update(l2_cache_invalidations_merged_dirty);
        core->local_stat.update(l2_cache_invalidations_merged_dirty);
      }
    }
  } else if (evictedAddr != -1) {
    assert(evictedAddr >= 0);

    if (isL1) {
      stat.update(l1_clean_evicts);
      core->local_stat.update(l1_clean_evicts);
    } else {
      stat.update(l2_clean_evicts);
      core->local_stat.update(l2_clean_evicts);
    }
    
    if (isL1) {
      stat.update(l1_cache_invalidations_clean);
      core->local_stat.update(l1_cache_invalidations_clean);
      if (isFull == 1) {
        stat.update(l1_cache_invalidations_full_clean);  
        core->local_stat.update(l1_cache_invalidations_full_clean);  
      } else {
        stat.update(l1_cache_invalidations_merged_clean);
        core->local_stat.update(l1_cache_invalidations_merged_clean);
      }
    } else {
      stat.update(l2_cache_invalidations_clean);
      core->local_stat.update(l2_cache_invalidations_clean);
      if (isFull == 1) {
        stat.update(l2_cache_invalidations_full_clean);  
        core->local_stat.update(l2_cache_invalidations_full_clean);  
      } else {
        stat.update(l2_cache_invalidations_merged_clean);
        core->local_stat.update(l2_cache_invalidations_merged_clean);
      }
    }
  }    

  if(evictedAddr != -1) {
    assert(evictedAddr >= 0);
    
    if (isL1) {
      stat.update(l1_cache_invalidations);
      core->local_stat.update(l1_cache_invalidations);
      if (isFull == 1) {
        stat.update(l1_cache_invalidations_full);  
        core->local_stat.update(l1_cache_invalidations_full);  
      } else {
        stat.update(l1_cache_invalidations_merged);
        core->local_stat.update(l1_cache_invalidations_merged);
      }
    } else {
      stat.update(l2_cache_invalidations);
      core->local_stat.update(l2_cache_invalidations);
      if (isFull == 1) {
        stat.update(l2_cache_invalidations_full);  
        core->local_stat.update(l2_cache_invalidations_full);  
      } else {
        stat.update(l2_cache_invalidations_merged);
        core->local_stat.update(l2_cache_invalidations_merged);
      }
    }

    stat.update(cache_evicts);
    core->local_stat.update(cache_evicts);
    if (isL1) {
      stat.update(l1_evicts);
      core->local_stat.update(l1_evicts);
    } else {
      stat.update(l2_evicts);
      core->local_stat.update(l2_evicts);
    }
          
    if (sim->recordEvictions) {         
      cacheStat cache_stat;
      cache_stat.cacheline = addr;
      cache_stat.cycle = cycles;
      cache_stat.offset = evictedOffset;
      cache_stat.nodeId = evictedNodeId;
      cache_stat.newNodeId = -2;
      cache_stat.graphNodeId = evictedGraphNodeId;
      cache_stat.graphNodeDeg = evictedGraphNodeDeg;
      cache_stat.unusedSpace = unusedSpace;
      if (isL1) {
        cache_stat.cacheLevel = 1;
      } else {
        cache_stat.cacheLevel = 2;
      }
      sim->evictStatsVec.push_back(cache_stat);  
    }
  }  
}

bool Cache::process() {
  next_to_execute.clear();
  for(auto t:to_execute) {
    execute(t);
  }
  
  while(pq.size() > 0) {
    if(pq.top().second > cycles)
      break;
    MemTransaction* t= static_cast<MemTransaction*>(pq.top().first);
    execute(t);
    
    pq.pop();
  }
  to_execute=next_to_execute;
  
  vector<MemTransaction*> next_to_send;
  
  for(auto it = to_send.begin(); it!= to_send.end(); ++it) { 
    MemTransaction *t = *it;

    if (isL1 && !t->isEviction && t->checkMSHR) {
      uint64_t cacheline = t->addr/size_of_cacheline;
      if (!t->isFull) {
        //assert(graphfire > 0 && !sim->learningRRVT);
        cacheline = t->addr/4;
      }
      bool mshrMiss = mshr.find(cacheline)==mshr.end();
      if(mshrMiss && num_mshr_entries < mshr_size) {  // not present in MSHRs and there is space for 1 more entry
        if (size > 0 || ideal) {
          if (t->isLoad) {
            stat.update(l1_primary_load_misses);
            core->local_stat.update(l1_primary_load_misses);
          } else {
            stat.update(l1_primary_store_misses);
            core->local_stat.update(l1_primary_store_misses);
          }
          stat.update(l1_primary_misses);
          core->local_stat.update(l1_primary_misses);
          mshr[cacheline]=MSHR_entry();
          mshr[cacheline].insert(t);
          mshr[cacheline].hit=0; 
          num_mshr_entries++;
          t->checkMSHR=false;
        }
      } else if (!mshrMiss) {   // it is already present in MSHRs
        if (t->isLoad) {
          stat.update(l1_secondary_load_misses);
          core->local_stat.update(l1_secondary_load_misses);
        } else {
          stat.update(l1_secondary_store_misses);
          core->local_stat.update(l1_secondary_store_misses);
        }
        stat.update(l1_secondary_misses);
        core->local_stat.update(l1_secondary_misses);
        mshr[cacheline].insert(t);
        t->checkMSHR=false;
        continue;
      } 
      else if (mshr_size == num_mshr_entries) {  // not present in MSHRs but they are FULL
        t->checkMSHR=true;   
        next_to_send.push_back(t);
        continue;
      }
    }
      
    uint64_t dramaddr = t->addr/size_of_cacheline * size_of_cacheline;
    /*int cacheline_size;
    if (cacheBySignature == 0 || t->graphNodeId == -1) {
      cacheline_size = size_of_cacheline;
    } else {
      cacheline_size = 4;
    }*/

    bool bypass_hierarchy = false;
    if(graphfire > 0 && !sim->learningRRVT) {
      int nodeId = t->src_id;
      if (!t->isEviction) {
        nodeId = t->d->n->id;
      }
      bool isPIA = get<0>((*(sim->RRVT))[nodeId]) < RRV_THRS && get<2>((*(sim->RRVT))[nodeId]) == RRV_THRS;
      bool irreg_bypass = isL1 && !isPIA;
      if (stmu == 1) {
        bypass_hierarchy = (graphfire == 2) && (t->isFull || irreg_bypass); 
      } else {
        bypass_hierarchy = (graphfire == 2) && (get<0>((*(sim->RRVT))[nodeId]) >= RRV_THRS || irreg_bypass);
      }
      /*if (stmu == 1 && !t->isFull) {
        cacheline_size = 4;
      }*/
    }
    if(isLLC || bypass_hierarchy) {
      
      if((!t->isEviction) && memInterface->willAcceptTransaction(dramaddr, t->isLoad)) {
        
        //assert(t->isLoad);
        t->cache_q->push_front(this);
        
        if (!isLLC && bypass_hierarchy) {      
          stat.update(dram_accesses_bypass);
          core->local_stat.update(dram_accesses_bypass);
          if (isL1) {
            stat.update(dram_accesses_l1_bypass);
            core->local_stat.update(dram_accesses_l1_bypass);
          } else {
            stat.update(dram_accesses_l2_bypass);
            core->local_stat.update(dram_accesses_l2_bypass);
          }
        } else {
          stat.update(dram_accesses_l3);
        }
        memInterface->addTransaction(t, dramaddr, t->isLoad, 64 /*cacheline_size*/, cycles);

        //collect mlp stats
        sim->curr_epoch_accesses++;
        
        //it = to_send.erase(it);        
      } else if ((t->isEviction) && memInterface->willAcceptTransaction(dramaddr, false)) { //forwarded evict1ion, will be treated as just a write, nothing to do
        if (!isLLC && bypass_hierarchy) {      
          stat.update(dram_accesses_bypass_forwards);
          core->local_stat.update(dram_accesses_bypass_forwards);
          if (isL1) {
            stat.update(dram_accesses_l1_bypass_forwards);
            core->local_stat.update(dram_accesses_l1_bypass_forwards);
          } else {
            stat.update(dram_accesses_l2_bypass_forwards);
            core->local_stat.update(dram_accesses_l2_bypass_forwards);
          }
        } else {
          stat.update(dram_accesses_l3_forwards);
        }
        memInterface->addTransaction(NULL, dramaddr, false, 64 /*cacheline_size*/, cycles);
        //collect mlp stats
        sim->curr_epoch_accesses++;
        //it = to_send.erase(it);
        delete t;           
      } else {
        next_to_send.push_back(t);
        //++it;
      }
    } else {
      if(size == 0 || parent_cache->willAcceptTransaction(t)) {
        if (!t->isEviction) {
          t->cache_q->push_front(this);
        }
        parent_cache->addTransaction(t);

        //it=to_send.erase(it);
      } else {
        next_to_send.push_back(t);
        //++it;
      }
    }
  }

  to_send=next_to_send;

  vector<tuple<uint64_t, int, int, int, int, int, int, bool, bool, bool>> next_to_evict;
  
  for(auto it = to_evict.begin(); it!= to_evict.end();++it) {
    uint64_t eAddr = get<0>(*it);
    int id = get<1>(*it);
    int src_id = get<2>(*it);
    int dst_id = get<3>(*it);
    int graphNodeId = get<4>(*it);
    int graphNodeDeg = get<5>(*it);
    int cacheline_size = get<6>(*it);
    bool isFull = get<7>(*it);
    bool isMerged = get<8>(*it);
    bool isRMW = get<9>(*it);

    bool bypass_hierarchy = false;
    if(graphfire > 0 && !sim->learningRRVT) {
      //assert(sim->RRVT->find(src_id) != sim->RRVT->end());
      bool isPIA = (sim->RRVT->find(src_id) != sim->RRVT->end()) && get<0>((*(sim->RRVT))[src_id]) < RRV_THRS && get<2>((*(sim->RRVT))[src_id]) == RRV_THRS;
      bool irreg_bypass = isL1 && !isPIA;
      if (stmu == 1) {
        bypass_hierarchy = (graphfire == 2) && (isFull || irreg_bypass); 
      } else {
        bypass_hierarchy = (graphfire == 2) && (get<0>((*(sim->RRVT))[src_id]) >= RRV_THRS || irreg_bypass);
      }
    }
    if(isLLC || bypass_hierarchy) {
      if(memInterface->willAcceptTransaction(eAddr/size_of_cacheline * size_of_cacheline, false)) {
        
        if (!isLLC && bypass_hierarchy) {      
          stat.update(dram_accesses_bypass_evicts);
          core->local_stat.update(dram_accesses_bypass_evicts);
          if (isL1) {
            stat.update(dram_accesses_l1_bypass_evicts);
            core->local_stat.update(dram_accesses_l1_bypass_evicts);
          } else {
            stat.update(dram_accesses_l2_bypass_evicts);
            core->local_stat.update(dram_accesses_l2_bypass_evicts);
          }
        } else {
          stat.update(dram_accesses_l3_evicts);
        }
       
        memInterface->addTransaction(NULL, eAddr/size_of_cacheline * size_of_cacheline, false, 64 /*cacheline_size*/, cycles);
        //it = to_evict.erase(it);
      }
      else {
        next_to_evict.push_back(make_tuple(eAddr, id, src_id, dst_id, graphNodeId, graphNodeDeg, cacheline_size, isFull, isMerged, isRMW));
        //++it;
      }
    }
    else {
      MemTransaction* t = new MemTransaction(id, src_id, dst_id, eAddr, false, graphNodeId, graphNodeDeg);
      t->isEviction = true;
      t->isFull = isFull;
      t->isMerged = isMerged;
      t->isRMW = isRMW;
      if(size == 0 || parent_cache->willAcceptTransaction(t)) {
        t->cache_q->push_front(this); 
        parent_cache->addTransaction(t);
        //it = to_evict.erase(it);
      } else {
        next_to_evict.push_back(make_tuple(eAddr, id, src_id, dst_id, graphNodeId, graphNodeDeg, cacheline_size, isFull, isMerged, isRMW));
        //++it;
      }
    }
  }
  
  to_evict=next_to_evict;
  cycles++;

  //bandwidth collection
  if(cycles % sim->bw_epoch==0 && cycles!=0) {
    sim->bw_per_epoch.push_back((float) sim->curr_bytes_accessed/(sim->bw_epoch/2));
    sim->curr_bytes_accessed=0;
  }

  //reset mlp stats collection
  if(cycles % sim->mlp_epoch==0 && cycles!=0) {
    sim->accesses_per_epoch.push_back(sim->curr_epoch_accesses);
    sim->curr_epoch_accesses=0;
  }
    
  
  free_load_ports = load_ports;
  free_store_ports = store_ports;
  return (pq.size() > 0);  
}

void Cache::execute(MemTransaction* t) {
  int nodeId = t->src_id;
  int graphNodeId = t->graphNodeId;
  int graphNodeDeg = t->graphNodeDeg;
  if(!t->isEviction) {
    nodeId = t->d->n->id;
  } else {
    nodeId = t->src_id;
  }
 
  if(isL1 && t->d) { //testing, remove false!!!
    DynamicNode* d=t->d;
    
    if(!t->isPrefetch && d->atomic) { //don't acquire locks based on accesses spurred by prefetch 
      if(!sim->lockCacheline(d)) {
         next_to_execute.push_back(t);
         return;
      }
    } else if(sim->isLocked(d)) {
      next_to_execute.push_back(t);
      return;
    }
  }
  uint64_t dramaddr = t->addr; // /size_of_cacheline * size_of_cacheline;
  bool res = true;   
  bool bypassCache = (size == 0);// || (cache_by_temperature == 1 && graphNodeDeg != -1 && graphNodeDeg < node_degree_threshold); //set as false to turn this off

  if(!ideal) {
    if (!bypassCache) {
      res = fc->access(dramaddr, nodeId, graphNodeId, graphNodeDeg, t->isLoad /*|| t->isRMW*/, t->isRMW, t->isPrefetch, t->isFull);   
    } else {
      res = false;
    }
  }

  //luwa change, just testing!!!
  //go to dram
  /*
  if(t->src_id!=-1 && t->d->type==LD) {
    res=true;
  }
  else {
    res=false;
  }
  */
 
  if (res) {
    //d->print("Cache Hit", 1);
    if(!t->isEviction) { //just normal hit, not an eviction from lower cache         
      if(isL1) {

        //don't do anything with prefetch instructions
        /*if(!t->isPrefetch) {
          sim->accessComplete(t);         
          }*/
                
        uint64_t cacheline=t->addr/size_of_cacheline;
        if (!t->isFull) {
          //assert(graphfire > 0 && !sim->learningRRVT);
          cacheline = t->addr/4;
        }
        mshr[cacheline]=MSHR_entry();
        mshr[cacheline].insert(t);
        mshr[cacheline].hit=1;
        
        TransactionComplete(t);
        
        stat.update(l1_total_accesses);
        core->local_stat.update(l1_total_accesses);
        if(t->isPrefetch) {
          stat.update(l1_prefetch_hits);
          core->local_stat.update(l1_prefetch_hits);
          stat.update(l1_prefetches);
          core->local_stat.update(l1_prefetches);  
        } else {    
          stat.update(l1_hits);
          core->local_stat.update(l1_hits);
          if(t->isLoad) {
            stat.update(l1_load_hits);
            core->local_stat.update(l1_load_hits);
            stat.update(l1_loads);
            core->local_stat.update(l1_loads);
          } else if (t->isRMW) {
            stat.update(l1_rmw_hits);
            core->local_stat.update(l1_rmw_hits);
            stat.update(l1_rmws);
            core->local_stat.update(l1_rmws);
          } else {
            stat.update(l1_store_hits);
            core->local_stat.update(l1_store_hits);
            stat.update(l1_stores);
            core->local_stat.update(l1_stores);
          }
          if (t->isEviction) {
            //assert(!t->isLoad);
            stat.update(l1_eviction_hits);
            core->local_stat.update(l1_eviction_hits);
            stat.update(l1_evictions);
            core->local_stat.update(l1_evictions);
          }
          stat.update(l1_accesses);
          core->local_stat.update(l1_accesses);
        }
      } else { //for l2 and l3 cache
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
        CacheLine* mergeBlock = NULL; 
        CacheLine* evictedMergeBlock = NULL;
        bool evictedFull = false;
        bool evictedRMW = false;
        bool clean = t->isLoad;

        Cache* c=t->cache_q->front();
        t->cache_q->pop_front();
        
        if (!t->cache_q->empty()) { //upper levels to go
          clean = true;
        }

        bool bypassCache = (c->size == 0); // || (c->cache_by_temperature == 1 && c->isL1 && graphNodeDeg < c->node_degree_threshold);
        if (!bypassCache) {    
          c->fc->insert(dramaddr, nodeId, graphNodeId, graphNodeDeg, clean /*|| t->isRMW*/, t->isRMW, t->isPrefetch, t->isFull, &dirtyEvict, &evictedAddr, &evictedOffset, &evictedNodeId, &evictedGraphNodeId, &evictedGraphNodeDeg, &unusedSpace, mergeBlock, &evictedMergeBlock, &evictedFull, &evictedRMW); 
        }
        
        if(evictedAddr != -1) {
          uint64_t eAddr = evictedAddr*c->size_of_cacheline + evictedOffset;
          assert(evictedAddr >= 0 || evictedMergeBlock != NULL);
          stat.update(cache_evicts);
          if (c->core) {
            c->core->local_stat.update(cache_evicts);
          }
          if (c->isL1) {
            stat.update(l1_evicts);
            c->core->local_stat.update(l1_evicts);
          } else if (c->isLLC) {
            assert(false);
            if (useL2) {
              stat.update(l3_evicts);
            } else {
              stat.update(l2_evicts);
            }
          } else {
            stat.update(l2_evicts);
            c->core->local_stat.update(l2_evicts);
          }
          
          bool isFull = evictedFull;

          int cacheline_size;
          if (cacheBySignature == 0 || evictedGraphNodeDeg == -1) {
            cacheline_size = c->size_of_cacheline;
            if (graphfire > 0 && stmu == 1 && !isFull) {
              cacheline_size = 4;
            }
          } else {
            cacheline_size = 4;
          }

          if (dirtyEvict) {
            c->to_evict.push_back(make_tuple(eAddr, t->id, evictedNodeId, evictedNodeId, evictedGraphNodeId, evictedGraphNodeDeg, cacheline_size, isFull, false, evictedRMW));
          
            if (c->isL1) {
              stat.update(l1_dirty_evicts);
              c->core->local_stat.update(l1_dirty_evicts);
            } else if (c->isLLC) {
              if (useL2) {
                stat.update(l3_dirty_evicts);
              } else {
                stat.update(l2_dirty_evicts);
              }
            } else {
              stat.update(l2_dirty_evicts);
              c->core->local_stat.update(l2_dirty_evicts);
              c->child_cache->evict(eAddr, true, t->id);
            }
          } else {
            if (c->isL1) {
              stat.update(l1_clean_evicts);
              c->core->local_stat.update(l1_clean_evicts);
            } else if (c->isLLC) {
              if (useL2) {
                stat.update(l3_clean_evicts);
              } else {
                stat.update(l2_clean_evicts);
              }
            } else {
              stat.update(l2_clean_evicts);
              c->core->local_stat.update(l2_clean_evicts);
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
                  stat.update(l1_dirty_evicts);
                  c->core->local_stat.update(l1_dirty_evicts);
                } else if (c->isLLC) {
                  assert(false);
                  if (useL2) {
                    stat.update(l3_dirty_evicts);
                  } else {
                    stat.update(l2_dirty_evicts);
                  }
                } else {
                  stat.update(l2_dirty_evicts);
                  c->core->local_stat.update(l2_dirty_evicts);
                  c->child_cache->evict(eAddr, true, t->id);
                }

                if (sim->recordEvictions) {
                  cacheStat cache_stat;
                  cache_stat.cacheline = curr->addr*c->size_of_cacheline + curr->offset;
                  cache_stat.cycle = cycles;
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
                  if (c->isL1) {
                    cache_stat.cacheLevel = 1;
                  } else {
                    cache_stat.cacheLevel = 2;
                  }
                  sim->evictStatsVec.push_back(cache_stat);         
                  //cout << "CHECK 1: MERGED BLOCK, " << cache_stat.cacheline << ", " << unusedSpace << ", " << cache_stat.offset << ", " << cache_stat.nodeId << endl;
                }
                isMerged = true;
              } else {
                if (c->isL1) {
                  stat.update(l1_clean_evicts);
                  c->core->local_stat.update(l1_clean_evicts);
                } else if (c->isLLC) {
                  if (useL2) {
                    stat.update(l3_clean_evicts);
                  } else {
                    stat.update(l2_clean_evicts);
                  }
                } else {
                  stat.update(l2_clean_evicts);
                  c->core->local_stat.update(l2_clean_evicts);
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
              cache_stat.cacheline = evictedAddr*c->size_of_cacheline + evictedOffset;
              cache_stat.cycle = cycles;
              cache_stat.offset = evictedOffset;
              cache_stat.nodeId = evictedNodeId;
              cache_stat.newNodeId = nodeId;
              cache_stat.graphNodeId = evictedGraphNodeId;
              cache_stat.graphNodeDeg = evictedGraphNodeDeg;
              cache_stat.unusedSpace = unusedSpace;
              if (c->isL1) {
                cache_stat.cacheLevel = 1;
              } else {
                cache_stat.cacheLevel = 2;
              }
              sim->evictStatsVec.push_back(cache_stat);  
              //cout << "CHECK 1: NORMAL BLOCK, " << cache_stat.cacheline << ", " << unusedSpace << ", " << cache_stat.offset << ", " << cache_stat.nodeId << endl;
            }        
          }
        }
       
        c->TransactionComplete(t);

        if (isLLC && useL2) {
          stat.update(l3_total_accesses);
          if(t->isPrefetch) {
            stat.update(l3_prefetch_hits);
            stat.update(l3_prefetches);
          } else {
            stat.update(l3_hits);
            if(t->isLoad) {
              stat.update(l3_load_hits);
              stat.update(l3_loads);
            } else if (t->isRMW) {
              stat.update(l3_rmw_hits);
              stat.update(l3_rmws);
            } else {
              stat.update(l3_store_hits);
              stat.update(l3_stores);
            }
            if(t->isEviction) {
              stat.update(l3_eviction_hits);
              stat.update(l3_evictions);
            }
            stat.update(l3_accesses);
          }
        } else {
          stat.update(l2_total_accesses);
          if (useL2) {
            core->local_stat.update(l2_total_accesses);
          }
          if(t->isPrefetch) {
            stat.update(l2_prefetch_hits);
            stat.update(l2_prefetches);
            if (useL2) {
              core->local_stat.update(l2_prefetch_hits);
              core->local_stat.update(l2_prefetches);
            }
          } else {
            stat.update(l2_hits);
            if (useL2) {
              core->local_stat.update(l2_hits);
            }
            if(t->isLoad) {
              stat.update(l2_load_hits);
              stat.update(l2_loads);
              if (useL2) {
                core->local_stat.update(l2_load_hits);
                core->local_stat.update(l2_loads);
              }
            } else if (t->isRMW) {
              stat.update(l2_rmw_hits);
              stat.update(l2_rmws);
              if (useL2) {
                core->local_stat.update(l2_rmw_hits);
                core->local_stat.update(l2_rmws);
              }
            } else {
              stat.update(l2_store_hits);
              stat.update(l2_stores);
              if (useL2) {
                core->local_stat.update(l2_store_hits);
                core->local_stat.update(l2_stores);
              }
            }
            if(t->isEviction) {
              //assert(!t->isLoad);
              stat.update(l2_eviction_hits);
              stat.update(l2_evictions);
              if (useL2) {
                core->local_stat.update(l2_eviction_hits);
                core->local_stat.update(l2_evictions);
              }
            }
            stat.update(l2_accesses);
            if (useL2) {
              core->local_stat.update(l2_accesses);
            }
          }
        }
      }       
    } else { // eviction from lower cache, no need to do anything, since it's a hit, involves no DN
      delete t; 
    }
  } //misses
  else {
    if(isL1) {
      stat.update(l1_total_accesses);
      core->local_stat.update(l1_total_accesses);
      if(t->isPrefetch) {
        stat.update(l1_prefetch_misses);
        core->local_stat.update(l1_prefetch_misses);
        stat.update(l1_prefetches);
        core->local_stat.update(l1_prefetches);  
      } else {    
        stat.update(l1_misses);
        core->local_stat.update(l1_misses);
        if(t->isLoad) {
          stat.update(l1_load_misses);
          core->local_stat.update(l1_load_misses);
          stat.update(l1_loads);
          core->local_stat.update(l1_loads);
        } else if (t->isRMW) {
          stat.update(l1_rmw_misses);
          core->local_stat.update(l1_rmw_misses);
          stat.update(l1_rmws);
          core->local_stat.update(l1_rmws);
        } else {
          stat.update(l1_store_misses);
          core->local_stat.update(l1_store_misses);
          stat.update(l1_stores);
          core->local_stat.update(l1_stores);
        }
        if (t->isEviction) {
          assert(!t->isLoad);
          stat.update(l1_eviction_misses);
          core->local_stat.update(l1_eviction_misses);
          stat.update(l1_evictions);
          core->local_stat.update(l1_evictions);
        }
        stat.update(l1_accesses);
        core->local_stat.update(l1_accesses);
      }
      t->checkMSHR=true;
    } else if (isLLC && useL2) {
      stat.update(l3_total_accesses);
      if(t->isPrefetch) {
        stat.update(l3_prefetch_misses);
        stat.update(l3_prefetches);
      } else {
        stat.update(l3_misses);
        if(t->isLoad) {
          stat.update(l3_load_misses);
          stat.update(l3_loads);
        } else if (t->isRMW) {
          stat.update(l3_rmw_misses);
          stat.update(l3_rmws);
        } else {
          stat.update(l3_store_misses);
          stat.update(l3_stores);
        }
        if(t->isEviction) {
          assert(!t->isLoad);
          stat.update(l3_eviction_misses);
          stat.update(l3_evictions);
        }
        stat.update(l3_accesses);
      }
    } else {
      stat.update(l2_total_accesses);
      if (useL2) {
        core->local_stat.update(l2_total_accesses);
      }
      if(t->isPrefetch) {
        stat.update(l2_prefetch_misses);
        stat.update(l2_prefetches);
        if (useL2) {
          core->local_stat.update(l2_prefetch_misses);
          core->local_stat.update(l2_prefetches);
        }
      } else {
        stat.update(l2_misses);
        if (useL2) {
          core->local_stat.update(l2_misses);
        }
        if(t->isLoad) {
          stat.update(l2_load_misses);
          stat.update(l2_loads);
          if (useL2) {
            core->local_stat.update(l2_load_misses);
            core->local_stat.update(l2_loads);
          }
        } else if (t->isRMW) {
          stat.update(l2_rmw_misses);
          stat.update(l2_rmws);
          if (t->isFull) {
            stat.update(l2_rmw_misses_full);
          } else {
            stat.update(l2_rmw_misses_merged);
          }
          if (useL2) {
            core->local_stat.update(l2_rmw_misses);
            core->local_stat.update(l2_rmws);
          }
        } else {
          stat.update(l2_store_misses);
          stat.update(l2_stores);
          if (useL2) {
            core->local_stat.update(l2_store_misses);
            core->local_stat.update(l2_stores);
          }
        }
        if(t->isEviction) {
          assert(!t->isLoad);
          stat.update(l2_eviction_misses);
          stat.update(l2_evictions);
          if (useL2) {
            core->local_stat.update(l2_eviction_misses);
            core->local_stat.update(l2_evictions);
          }
        }
        stat.update(l2_accesses);
        if (useL2) {
          core->local_stat.update(l2_accesses);
        }
      }
    }
 
    to_send.push_back(t); //send higher up in hierarchy
    //d->print(Cache Miss, 1);
    //if(!t->isPrefetch) {    
    //}
  }
}
  
uint64_t Cache::get_home_bank(uint64_t address) {
  int bank_count = bank_size / size_of_cacheline / assoc;
  int set_count = size / size_of_cacheline / assoc;
  int min = log2(size_of_cacheline) + log2(bank_count);
  int max = log2(size_of_cacheline) + log2(set_count);
  uint64_t maxmask = ((uint64_t)1 << (max))-1;
  uint64_t minmask = ((uint64_t)1 << (min))-1;
  uint64_t mask = maxmask - minmask;
  uint64_t val = address & mask;
  val = val >> min;
  return val;
}

void Cache::addTransaction(MemTransaction *t) {
  /*if(isL1 && t->src_id!=-1) { //not eviction
    uint64_t cacheline = t->addr/size_of_cacheline;
    if(mshr.find(cacheline)==mshr.end()) {
      if (size > 0 || ideal) {
        mshr[cacheline]=MSHR_entry();
      }
      pq.push(make_pair(t, cycles+latency)); //add transaction only if it's the 1st
    }
    if (size > 0 || ideal) {
      mshr[cacheline].insert(t); 
    }
  } else {
     pq.push(make_pair(t, cycles+latency));
  }*/

  int nodeId = t->src_id;
  if(!t->isEviction) {
    nodeId = t->d->n->id;
  } else {
    nodeId = t->src_id;
  }
  if (isL1 && !isLLC) {
    if (graphfire > 0) {
      t->isFull = !(!sim->learningRRVT && get<0>((*(sim->RRVT))[nodeId]) < RRV_THRS); // access granularity is decided in L1 and remains throughout the hierarchy
    } /*else if (graphfire == 1) {
      t->isFull = !(!sim->learningRRVT && get<0>((*(sim->RRVT))[nodeId]) < RRV_THRS && get<2>((*(sim->RRVT))[nodeId]) == RRV_THRS && (t->isLoad || t->isRMW)); // access granularity is decided in L1 and remains throughout the hierarchy
    }*/
  }
  int sb_latency = latency;
  if (!t->isFull) { // will require STMU
    sb_latency += stmu_latency;
  }
  if (t->isMerged) {
    sb_latency = 0;
  }
  int home_bank = (int) get_home_bank(t->addr);
  if (isLLC && home_bank != t->id) {
    sb_latency += remote_latency;
    stat.update(llc_remote_accesses);
  } else {
    stat.update(llc_local_accesses); 
  }
  /*if (isLLC) {
    cout << "home bank = " << home_bank << ", id = " << t->id << ", latency = " << sb_latency << endl;
  }*/
  if (size == 0 || (fc->sets.at(0)->cache_by_temperature == 1 && t->graphNodeDeg != -1 && t->graphNodeDeg < fc->sets.at(0)->node_degree_threshold)) sb_latency = 0;
  pq.push(make_pair(t, cycles+sb_latency));
  stat.update(cache_access);
  if(t->isLoad)
    free_load_ports--;
  else
    free_store_ports--;

  //for prefetching, don't issue prefetch for evict or for access with outstanding prefetches or for access that IS  prefetch or eviction
  if(isL1 && t->src_id>=0 && num_prefetched_lines>0 && t->graphNodeId == -1) {
    //int cache_line = t->d->addr/size_of_cacheline;
    bool pattern_detected=false;
    bool single_stride=true;
    bool double_stride=true;
    int stride=min_stride;

    //detect single stride
    for(int i=1; i<(pattern_threshold+1); i++) {
      if(prefetch_set.find(t->d->addr-stride*i)==prefetch_set.end()) {
        single_stride=false;
        break;
      }
    }
    //detect double stride
    if(!single_stride) {
      stride=2*min_stride;
      for(int i=1; i<(pattern_threshold+1); i++) {
        if(prefetch_set.find(t->d->addr-stride*i)==prefetch_set.end()) {
          double_stride=false;
          break;
        }
      }      
    }
    pattern_detected=single_stride || double_stride;
 
    //don't prefetch or insert prefectch instrs into prefetch set
    if(!t->issuedPrefetch && !t->isPrefetch) {
      if(pattern_detected) { 

        for (int i=0; i<num_prefetched_lines; i++) {
          MemTransaction* prefetch_t = new MemTransaction(t->id, -2, -2, t->addr + size_of_cacheline*(prefetch_distance+i), true, -1, -1); //prefetch some distance ahead
          prefetch_t->d=t->d;
          prefetch_t->isPrefetch=true;
          prefetch_t->isFull=true;
          //pq.push(make_pair(prefetch_t, cycles+latency));
          this->addTransaction(prefetch_t);
        }
        
        if(t->d->type==LD_PROD) {
          //cout << "PREFETCHING LD_PROD addr: " << t->d->addr << endl;
        }
        
        prefetch_set.insert(t->d->addr);
        prefetch_set.erase(t->d->addr-stride*pattern_threshold); 
       
      }
      else { //keep prefetch set size capped
        int current_size = prefetch_queue.size();
        if(current_size >= prefetch_set_size) {
          prefetch_set.erase(prefetch_queue.front());
          prefetch_queue.pop();
          prefetch_set_size--;
        }
        prefetch_set.insert(t->d->addr);
        prefetch_queue.push(t->d->addr);
        prefetch_set_size++; 
      }      
    }   
  }
}

bool Cache::willAcceptTransaction(MemTransaction *t) {  
  if(!t || t->isEviction) { //eviction or null transaction
    return free_store_ports > 0 || store_ports==-1;
  }
  else if(t->isLoad) {
    return free_load_ports > 0 || load_ports==-1;
  }
  else {
    return free_store_ports > 0 || store_ports==-1;
  }
}

void Cache::TransactionComplete(MemTransaction *t) { 
  if(isL1) {
    
    uint64_t cacheline=t->addr/size_of_cacheline;
    if(!t->isFull) {
      //assert(graphfire > 0 && !sim->learningRRVT);
      cacheline = t->addr/4;
    }

    //should be part of an mshr entry
    // ANINDA COMMENT
    /*if((size > 0 || ideal) && mshr.find(cacheline)==mshr.end()) {
      assert(false);
    }*/
    
    /*MSHR_entry mshr_entry;
    if (size > 0 || ideal) {
      mshr_entry=mshr[cacheline];
    } 
    }*/

    //update hit/miss stats, account for each individual access

    /*stat.update(l1_total_accesses,batch_size);
    
    if(mshr_entry.hit) {
      if(t->isprefetch) {
        stat.update(l1_prefetch_hits,batch_size-non_prefetch_size);
        stat.update(l1_prefetches,batch_size-non_prefetch_size);  
      } else {
        stat.update(l1_hits,non_prefetch_size);
        core->local_stat.update(l1_hits,non_prefetch_size);
        stat.update(l1_accesses,non_prefetch_size);
        core->local_stat.update(l1_accesses,non_prefetch_size);
      }
    } else {
      if(t->isprefetch) {
        stat.update(l1_prefetch_misses,batch_size-non_prefetch_size);
        stat.update(l1_prefetches,batch_size-non_prefetch_size);  
      } else {
        stat.update(l1_misses,non_prefetch_size);
        core->local_stat.update(l1_misses,non_prefetch_size);
        stat.update(l1_accesses,non_prefetch_size);
        core->local_stat.update(l1_accesses,non_prefetch_size);
      }
    }*/
    
    if (mshr.find(cacheline) == mshr.end()) {
      if(core->sim->debug_mode || core->sim->mem_stats_mode) {
        DynamicNode* d=t->d;
        assert(d!=NULL);
        assert(core->sim->load_stats_map.find(d)!=core->sim->load_stats_map.end());
        get<2>(core->sim->load_stats_map[d])=1;
      } 
    } else if (size > 0 || ideal) {
      MSHR_entry mshr_entry = mshr[cacheline];
      auto trans_set=mshr_entry.opset;
      //int batch_size=trans_set.size();
      //int non_prefetch_size=mshr_entry.non_prefetch_size;
    
      //process callback for each individual transaction in batch
      for (auto it=trans_set.begin(); it!=trans_set.end(); ++it) {
        MemTransaction* curr_t=*it;
        if(!curr_t->isPrefetch) {
          //record statistics on non-prefetch loads/stores
          if(core->sim->debug_mode || core->sim->mem_stats_mode) {
            DynamicNode* d=curr_t->d;
            assert(d!=NULL);
            assert(core->sim->load_stats_map.find(d)!=core->sim->load_stats_map.end());
            get<2>(core->sim->load_stats_map[d])=mshr_entry.hit;
            //get<2>(entry_tuple)=mshr_entry.hit;           
          }
           
        /*if(mshr_entry.hit) {     
          if(curr_t->isLoad) {
            stat.update(l1_load_hits);
            core->local_stat.update(l1_load_hits);
            stat.update(l1_loads);
            core->local_stat.update(l1_loads);
          } else {
            stat.update(l1_store_hits);
            core->local_stat.update(l1_store_hits);
            stat.update(l1_stores);
            core->local_stat.update(l1_stores);
          }
        } else {
          if(curr_t->isLoad) {
            stat.update(l1_load_misses);
            core->local_stat.update(l1_load_misses);
            stat.update(l1_loads);
            core->local_stat.update(l1_loads);
          } else {
            stat.update(l1_store_misses);
            core->local_stat.update(l1_store_misses);
            stat.update(l1_stores);
            core->local_stat.update(l1_stores);
          }
        }*/

          sim->accessComplete(curr_t);
        } else { //prefetches get no callback, tied to no dynamic node
          delete curr_t;
        }   
      }
      num_mshr_entries--;
      mshr.erase(cacheline); //clear mshr for that cacheline
    }
  } else {
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

    bool bypassCache = (c->size == 0); // || (c->cache_by_temperature == 1 && c->isL1 && graphNodeDeg < c->node_degree_threshold);
    if (!bypassCache) {    
      c->fc->insert(t->addr, nodeId, graphNodeId, graphNodeDeg, clean /*|| t->isRMW*/, t->isRMW, t->isPrefetch, t->isFull, &dirtyEvict, &evictedAddr, &evictedOffset, &evictedNodeId, &evictedGraphNodeId, &evictedGraphNodeDeg, &unusedSpace, NULL, &evictedMergeBlock, &evictedFull, &evictedRMW);
    }

    if(evictedAddr!=-1) {
      uint64_t eAddr = evictedAddr*c->size_of_cacheline + evictedOffset;

      assert(evictedAddr >= 0 || evictedMergeBlock != NULL);
      stat.update(cache_evicts);
      if (c->core) {
        c->core->local_stat.update(cache_evicts);
      }
      if (c->isL1) {
        stat.update(l1_evicts);
        c->core->local_stat.update(l1_evicts);
      } else if (c->isLLC) {
        assert(false);
        if (useL2) {
          stat.update(l3_evicts);
        } else {
          stat.update(l2_evicts);
        }
      } else { // must be L2
        stat.update(l2_evicts);
        c->core->local_stat.update(l2_evicts);
      }

      bool isFull = evictedFull;

      int cacheline_size;
      if (cacheBySignature == 0 || evictedGraphNodeDeg == -1) {
        cacheline_size = c->size_of_cacheline;
        if (graphfire > 0 && stmu == 1 && !isFull) {
          cacheline_size = 4;
        }
      } else {
        cacheline_size = 4;
      }

      if (dirtyEvict) {
        c->to_evict.push_back(make_tuple(eAddr, t->id, evictedNodeId, evictedNodeId, evictedGraphNodeId, evictedGraphNodeDeg, cacheline_size, isFull, false, evictedRMW));
        if (c->isL1) {
          stat.update(l1_dirty_evicts);
          c->core->local_stat.update(l1_dirty_evicts);
        } else if (c->isLLC) {
          if (useL2) {
            stat.update(l3_dirty_evicts);
          } else {
            stat.update(l2_dirty_evicts);
          }
        } else {
          stat.update(l2_dirty_evicts);
          c->core->local_stat.update(l2_dirty_evicts);
          c->child_cache->evict(eAddr, true, t->id);
        }
      } else {
        if (c->isL1) {
          stat.update(l1_clean_evicts);
          c->core->local_stat.update(l1_clean_evicts);
        } else if (c->isLLC) {
          if (useL2) {
            stat.update(l3_clean_evicts);
          } else {
            stat.update(l2_clean_evicts);
          }
        } else {
          stat.update(l2_clean_evicts);
          c->core->local_stat.update(l2_clean_evicts);
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
              stat.update(l1_dirty_evicts);
              c->core->local_stat.update(l1_dirty_evicts);
            } else if (c->isLLC) {
              if (useL2) {
                stat.update(l3_dirty_evicts);
              } else {
                stat.update(l2_dirty_evicts);
              }
            } else {
              stat.update(l2_dirty_evicts);
              c->core->local_stat.update(l2_dirty_evicts);
              c->child_cache->evict(eAddr, true, t->id);
            }

            if (sim->recordEvictions) {
              cacheStat cache_stat;
              cache_stat.cacheline = curr->addr*c->size_of_cacheline + curr->offset;
              cache_stat.cycle = cycles;
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
              if (c->isL1) {
                cache_stat.cacheLevel = 1;
              } else {
                cache_stat.cacheLevel = 2;
              }
              sim->evictStatsVec.push_back(cache_stat);         
              //cout << "CHECK 2: MERGED BLOCK, " << cache_stat.cacheline << ", " << unusedSpace << ", " << cache_stat.offset << ", " << cache_stat.nodeId << endl;
            }
            isMerged = true;
          } else {
            if (c->isL1) {
              stat.update(l1_clean_evicts);
              c->core->local_stat.update(l1_clean_evicts);
            } else if (c->isLLC) {
              if (useL2) {
                stat.update(l3_clean_evicts);
              } else {
                stat.update(l2_clean_evicts);
              }
            } else {
              stat.update(l2_clean_evicts);
              c->core->local_stat.update(l2_clean_evicts);
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
          cache_stat.cacheline = evictedAddr*c->size_of_cacheline + evictedOffset;
          cache_stat.cycle = cycles;
          cache_stat.offset = evictedOffset;
          cache_stat.nodeId = evictedNodeId;
          cache_stat.newNodeId = nodeId;
          cache_stat.graphNodeId = evictedGraphNodeId;
          cache_stat.graphNodeDeg = evictedGraphNodeDeg;
          cache_stat.unusedSpace = unusedSpace;
          if (c->isL1) {
            cache_stat.cacheLevel = 1;
          } else {
            cache_stat.cacheLevel = 2;
          }
          sim->evictStatsVec.push_back(cache_stat);         
          //cout << "CHECK 2: NORMAL BLOCK, " << cache_stat.cacheline << ", " << unusedSpace << ", " << cache_stat.offset << ", " << cache_stat.nodeId << endl;
        }
      } 
    }
    c->TransactionComplete(t);      
  }
}
