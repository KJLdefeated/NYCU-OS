/*
Student No.: 110652019
Student Name: 林楷傑
Email: linkai0508@gmail.com
SE tag: xnxcxtxuxoxsx
Statement: I am fully aware that this program is not
supposed to be posted to a public server, such as a
public GitHub repository or a public web page.
*/
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

typedef struct block {
    size_t size;
    int free;
    struct block *prev;
    struct block *next;
}block;

void *add = 0;
block *head = 0;
static const size_t align_to = 32;

void *malloc(size_t size){
    block *cur=0, *tmp=0;
    if(size){
        size = (size + (align_to - 1) & ~ (align_to - 1));
        if(!add){
            add = mmap(NULL, 20000, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
            head = add;
            head->size = 20000-32;
            head->free = 1;
            head->prev = NULL;
            head->next = NULL;
        }
        cur=head;
        size_t maxsize = (1<<16);
        while(cur != NULL){
            if(cur->free && cur->size < maxsize && cur->size >= size){
                maxsize = cur->size;
                tmp = cur;
            }
            cur = cur->next;
        }
        cur = tmp;
        if(cur->size == size){
            cur->free=0;
            return cur+1;
        }
        else{
            tmp = cur + 1 + size/32;
            tmp->free=1;
            tmp->prev=cur;
            tmp->next=cur->next;
            tmp->size=cur->size-32-size;
            if(cur->next != NULL) cur->next->prev = tmp;
            cur->next = tmp;
            cur->free = 0;
            cur->size = size;
            return cur+1;
        }
    }
    else{
        cur=head;
        size_t maxsize=0;
        while(cur!=NULL){
            maxsize = (cur->size>maxsize&&cur->free) ? cur->size : maxsize;
            cur=cur->next;
        }
        char ans[50];
        sprintf(ans, "Max Free Chunk Size = %ld\n", maxsize);
        write(1, ans, 50);
        return 0;
    }
}

void free(void *pos){
    block *cur;
    cur = pos;
    cur-=1;
    cur->free=1;
    if(cur->next != NULL && cur->next->free){
        cur->size += 32 + cur->next->size;
        if(cur->next->next != NULL) cur->next->next->prev = cur;
        cur->next=cur->next->next;
    }
    if(cur->prev != NULL && cur->prev->free){
        cur->prev->size += 32 + cur->size;
        if(cur->next != NULL) cur->next->prev = cur->prev;
        cur->prev->next = cur->next;
    }
}