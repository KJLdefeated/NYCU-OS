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
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <vector>
#include <queue>
#include <pthread.h>
#include <sys/time.h>
#include <semaphore.h>
#include <unistd.h>
using namespace std;

int n, p[15], sub[8];
pair<int, int> c[15];
vector<int> a, b;
queue<int> jobs;
bool job_done[16];
sem_t finish;
sem_t Mutex;

void bubble_sort(int l, int r){
    for(int i=l;i<r;i++)
        for(int j=i+1;j<r;j++)
            if(a[i] > a[j]) swap(a[i], a[j]);
}

void merge(int l, int r){
    int mid = -1;
    for(int i=l+1;i<r;i++){
        if(a[i] < a[i-1]){
            mid = i;
            break;
        }
    }
    if(mid==-1)return;
    vector<int> tmp(r-l+1);
    int i=l,j=mid, cnt=0;
    while(i < mid && j < r) tmp[cnt++] = (a[i] < a[j] ? a[i++] : a[j++]);    
    while(i < mid) tmp[cnt++] = a[i++];
    while(j < r) tmp[cnt++] = a[j++];
    for(i=l;i<r;i++) a[i] = tmp[i-l];
}

void *handler(void *){
    sem_wait(&Mutex);
    while(jobs.size()){
        int work = jobs.front(); jobs.pop();
        //printf("%d\n", work);
        sem_post(&Mutex);
        if(work < 8){
            int l=0, i=0;
            for(i=0;i<work;i++) l += sub[i];
            bubble_sort(l, l+sub[i]);
        }
        else{
            int l=0, r=0, i=0;
            for(i=0;i<c[work].first;i++) l+=sub[i]; r=l;
            for(;i<=c[work].second;i++) r+=sub[i];
            //printf("%d %d\n", l, r);
            merge(l, r);
        }
        if(work < 14){
            sem_wait(&Mutex);
            job_done[work] = true;
            if(job_done[work+(work%2?-1:1)]) jobs.push(p[work]);
            sem_post(&Mutex);
        }
    }
    sem_post(&Mutex);
    sem_post(&finish);
    return NULL;
}

int main(){
    ifstream in;
    ofstream out;
    in.open("input.txt");
    in>>n;
    a = vector<int>(n);
    for(int i=0;i<n;i++)in>>a[i];
    b=a;

    int res = n%8;
    for(int i=0;i<8;i++)sub[i] = n/8 + (res-- > 0 ? 1 : 0);

    //Set dependency
    p[0] = p[1] = 8;
    p[2] = p[3] = 9;
    p[4] = p[5] = 10;
    p[6] = p[7] = 11;
    p[8] = p[9] = 12;
    p[10] = p[11] = 13;
    p[12] = p[13] = 14;
    p[14] = -1;
    c[8] =  {0, 1};
    c[9] =  {2, 3};
    c[10] = {4, 5};
    c[11] = {6, 7};
    c[12] = {0, 3};
    c[13] = {4, 7};
    c[14] = {0, 7};
    
    for(int n_threads = 1;n_threads <= 8;n_threads++){
        for(int i=0;i<16;i++)job_done[i] = false;
        sem_init(&finish, 0, 0);
        sem_init(&Mutex, 0, 1);
        pthread_t threads[n];
        struct timeval start, end;
        gettimeofday(&start, 0);
        for(int i=0;i<8;i++) jobs.push(i);
        for(int i=0;i<n_threads;i++) pthread_create(&threads[i], NULL, handler, NULL);
        for(int i=0;i<n_threads;i++)pthread_join(threads[i], NULL);
        sem_wait(&finish);
        gettimeofday(&end, 0);
        float ms = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;
        printf("worker thread #%d, elapsed %f ms\n", n_threads, ms);
        string str;
        str.append("output_").append(to_string(n_threads)).append(".txt");
        out.open(str);
        for(int i=0; i<n; i++)out<<a[i]<<" ";
        a=b;
        out.close();
        sem_destroy(&finish);
        sem_destroy(&Mutex);
    }

    return 0;
}