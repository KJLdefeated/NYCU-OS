/*
Student No.: 110652019
Student Name: 林楷傑
Email: kjl0508.sc10@nycu.edu.tw
SE tag: xnxcxtxuxoxsx
Statement: I am fully aware that this program is not supposed to be posted to a public server, such as a public GitHub repository or a public web page.
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/wait.h>
#include<sys/time.h>

int main(){
    int n;
    printf("Input the matrix dimension: ");
    scanf("%d", &n);
    u_int32_t *A = (u_int32_t*)malloc(n*n*sizeof(u_int32_t));
    for(int i=0;i<n;i++)for(int j=0;j<n;j++)A[i*n+j] = i*n + j;
    int shm_id = shmget(0, n*n*sizeof(u_int32_t), IPC_CREAT|0600);
    int *mat = shmat(shm_id, NULL, 0);
    struct timeval start, end;
    for(int i=1;i<=16;i++){
        gettimeofday(&start, 0);
        pid_t pid;
        //Create i process to calculate parallel
        for(int j=1;j<=i;j++){
            int m = n/i;
            int k = (j == i ? n : m*j), tmp=0;
            pid = fork();
            if(pid==0){
                for(int r=m*(j-1); r<k; r++)
                    for(int c=0;c<n;c++){
                        tmp=0;
                        for(int h=0;h<n;h++) tmp += A[r*n+h] * A[h*n+c];
                        mat[r*n+c] = tmp;
                    }
                exit(0);
            }
        }
        for(int j=0;j<i;j++)wait(NULL);
        u_int32_t sum = 0;
        for(int j=0;j<n;j++)for(int k=0;k<n;k++) sum+=mat[j*n+k];
        // Calculate spent time of i process
        gettimeofday(&end, 0);
        int sec = end.tv_sec - start.tv_sec;
        int usec = end.tv_usec - start.tv_usec;
        printf("Multiplying matrices using %d processes\nElapsed time %f sec, Checksum: %d\n", i, sec+(usec/1000000.0), sum);
    }
    shmdt(mat);
    shmctl(shm_id, IPC_RMID, NULL);
    return 0;
}