/*
Student No.: 110652019
Student Name: 林楷傑
Email: linkai0508@gmail.com
SE tag: xnxcxtxuxoxsx
Statement: I am fully aware that this program is not
supposed to be posted to a public server, such as a
public GitHub repository or a public web page.
*/
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <sys/time.h>
#include <list>
#include <cstring>
#include <algorithm>
using namespace std;

#define MAX_SIZE 140005

int main(int argc, char** argv){
    ifstream in;
    timeval start, end;
    printf("LFU policy:\n");
    printf("Frame\tHit\t\tMiss\t\tPage fault ratio\n");
    gettimeofday(&start, 0);
    for(int frame = 64; frame <= 512; frame *= 2){
        int hit=0, miss=0, lst_freq=1;
        int page;
        int size_= 0;
        int cnt = 0;
        list<int> freq_lst[500];
        pair<int, list<int>::iterator> hash_map[MAX_SIZE] = {};
        //in.open("zipf.txt");
        in.open(argv[1]);  
        while(in>>page){
            if(hash_map[page].first){
                hit++;
                if(hash_map[page].first < 500)
                    freq_lst[hash_map[page].first].erase(hash_map[page].second);
                if((hash_map[page].first >= 500 || freq_lst[hash_map[page].first].empty()) && lst_freq == hash_map[page].first) lst_freq++;
                hash_map[page].first++;
                if(hash_map[page].first < 500){
                    freq_lst[hash_map[page].first].push_front(page);
                    hash_map[page].second = freq_lst[hash_map[page].first].begin();
                }
                cnt = max(cnt, lst_freq);
            }
            else{
                miss++;
                if(size_ == frame){
                    hash_map[*freq_lst[lst_freq].rbegin()].first=0;
                    freq_lst[lst_freq].pop_back();                    
                    freq_lst[1].push_front(page);
                    hash_map[page].first++;
                    hash_map[page].second = freq_lst[1].begin();
                    lst_freq=1;
                }
                else{
                    size_++;
                    freq_lst[1].push_front(page);                    
                    hash_map[page].first++;
                    hash_map[page].second = freq_lst[1].begin();
                    lst_freq=1;
                }
            }
        }
        //printf("%d\n", cnt);
        printf("%d\t%d\t\t%d\t\t%.10f\n", frame, hit, miss, miss*1./(hit+miss));
        in.close();
    }
    gettimeofday(&end, 0);
	double sec = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
	printf("Total elapsed tme %.4f sec\n\n", sec);
    //LRU
    printf("LRU policy:\n");
    printf("Frame\tHit\t\tMiss\t\tPage fault ratio\n");
    gettimeofday(&start, 0);
    for(int frame = 64; frame <= 512; frame *= 2){
        int hit=0, miss=0, lst_freq=1;
        int page;
        int size_= 0;
        int cnt = 0;
        list<int> freq_lst;
        pair<int, list<int>::iterator> hash_map[MAX_SIZE] = {};
        in.open(argv[1]);  
        while(in>>page){
            if(hash_map[page].first){
                hit++;
                freq_lst.erase(hash_map[page].second);
                freq_lst.push_front(page);
                hash_map[page].first++;
                hash_map[page].second = freq_lst.begin();
            }
            else{
                miss++;
                if(size_ == frame){
                    hash_map[*freq_lst.rbegin()].first = 0;
                    freq_lst.pop_back();
                    freq_lst.push_front(page);
                    hash_map[page].first = 1;
                    hash_map[page].second = freq_lst.begin();
                }
                else{
                    size_++;
                    freq_lst.push_front(page);                    
                    hash_map[page].first++;
                    hash_map[page].second = freq_lst.begin();
                }
            }
        }
        //printf("%d\n", cnt);
        printf("%d\t%d\t\t%d\t\t%.10f\n", frame, hit, miss, miss*1./(hit+miss));
        in.close();
    }
    gettimeofday(&end, 0);
	sec = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
	printf("Total elapsed tme %.4f sec\n\n", sec);
    return 0;
}