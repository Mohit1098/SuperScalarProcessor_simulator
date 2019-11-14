#include<bits/stdc++.h>
using namespace std;
#define int long long
ifstream iFile("input.txt"); 
ofstream oFile("output.txt");

class Block{
public:
    int tag,
        size,
        data,
        valid,
        dirty;

    Block(){
        tag=
        size=
        data=
        valid=
        dirty=-1;
    }
};

class Cache{
public:
    int size,blocks,
        block_size,
        ways,sets,
        rep_ploicy,
        no_cache_access,
        no_read_access,
        no_write_access,
        no_cache_misses,
        no_comp_miss,
        no_cap_miss,
        no_confl_miss,
        no_read_miss,
        no_write_miss,
        no_dirty_blocks_evict;

    set<int>address;
    Block **cache;
    int oper_mask,offset_mask;

    Cache(){
        blocks=
        size=
        block_size=
        ways=
        sets=
        rep_ploicy=
        no_cache_access=
        no_read_access=
        no_write_access=
        no_cache_misses=
        no_comp_miss=
        no_cap_miss=
        no_confl_miss=
        no_read_miss=
        no_write_miss=
        no_dirty_blocks_evict=0;
    }

    void set_parameters(){
        blocks=size/block_size;
        if(ways==0){
            ways=blocks;
        }

        sets=blocks/ways;
        oper_mask=(int)(pow(2,31));
        cache=new Block*[sets];
        for(int i=0;i<sets;++i){
            cache[i]=new Block[ways];
        }
    }

    // used for LRU
    void LRUmovefront(int i,int set_num){
        Block temp=cache[set_num][i];
        for(int j=i;j>0;--j)
            cache[set_num][j]=cache[set_num][j-1];
        cache[set_num][0]=temp;
    }

    void read_com(int temptag){

        int set_num=temptag%sets;
        int tag=temptag/sets;
        
        for(int i=0;i<ways;++i){
            if(cache[set_num][i].tag==tag){
                if(rep_ploicy>0)
                    LRUmovefront(i,set_num);
                return ;
            }
            if(cache[set_num][i].tag==-1){
                cache[set_num][i].tag=tag;
                no_read_miss++;
                no_cache_misses++;
                cache[set_num][i].dirty=0;
                if(rep_ploicy>0)
                    LRUmovefront(i,set_num);
                return ;
            }
        }
        no_read_miss++;
        no_cache_misses++;

        int pos;
        if(rep_ploicy>0)
            pos=ways-1;
        else
            pos=rand()%ways;

        if(cache[set_num][pos].dirty==1)
            no_dirty_blocks_evict++;
        cache[set_num][pos].tag=tag;
        cache[set_num][pos].dirty=0;

        if(rep_ploicy>0)
            LRUmovefront(pos,set_num);

        return ;
    }

    void write_com(int temptag){

        int set_num=temptag%sets;
        int tag=temptag/sets;
        
        for(int i=0;i<ways;++i){
            if(cache[set_num][i].tag==tag){
                cache[set_num][i].dirty=1;
                if(rep_ploicy>0)
                    LRUmovefront(i,set_num);
                return;
            }

            if(cache[set_num][i].tag==-1){
                no_write_miss++;
                no_cache_misses++;
                cache[set_num][i].tag=tag;
                cache[set_num][i].dirty=1;
                if(rep_ploicy>0)
                    LRUmovefront(i,set_num);
                return;
            }
        }

        no_write_miss++;
        no_cache_misses++;

        int pos;

        if(rep_ploicy>0)
            pos=ways-1;
        else
            pos=rand()%ways;

        if(cache[set_num][pos].dirty==1)
            no_dirty_blocks_evict++;
        cache[set_num][pos].tag=tag;
        cache[set_num][pos].dirty=1;

        if(rep_ploicy>0)
            LRUmovefront(pos,set_num);

    }

    void command(int com){
        int op=com&oper_mask;
        com=com&(oper_mask-1);

        int temptag=com/block_size;

        no_cache_access++;

        if(address.find(temptag)==address.end())
            no_comp_miss++;
        address.insert(temptag);

        if(op==0){
            no_read_access++;
            read_com(temptag);
        }
        else{
            no_write_access++;
            write_com(temptag);
        }
    }

    void print(){
        if(sets>1)
            no_confl_miss=no_cache_misses-no_comp_miss;
        else
            no_cap_miss=no_cache_misses-no_comp_miss;

        oFile<<size<<endl<<
        block_size<<endl<<
        ((sets==1)?0:ways)<<endl<<
        rep_ploicy<<endl<<
        no_cache_access<<endl<<
        no_read_access<<endl<<
        no_write_access<<endl<<
        no_cache_misses<<endl<<
        no_comp_miss<<endl<<
        no_confl_miss<<endl<<
        no_cap_miss<<endl<<
        no_read_miss<<endl<<
        no_write_miss<<endl<<
        no_dirty_blocks_evict<<endl;
    }

};

int32_t main(){
    Cache cache;

    iFile>>cache.size>>cache.block_size>>cache.ways>>cache.rep_ploicy;
    cache.set_parameters();
    srand(time(NULL));
    while (true) {
        int x;
        iFile>>hex>>x;
        if( iFile.eof()) break;
        cache.command(x);
    }

    cache.print();

}