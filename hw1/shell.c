/*
Student No.: 110652019
Student Name: 林楷傑
Email: kjl0508.sc10@nycu.edu.tw
SE tag: xnxcxtxuxoxsx
Statement: I am fully aware that this program is not supposed to be posted to a public server, such as a public GitHub repository or a public web page.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

void run(char **words, int zombie){
	if (zombie){
		pid_t pid;
		pid = fork();
		if (pid < 0){
			fprintf(stderr, "Fork Failed");
			exit(-1);
		}
		else if (pid == 0){
			pid = fork();
			if (pid < 0){
				fprintf(stderr, "Fork Failed");
				exit(-1);
			}
			else if (pid == 0){
				sleep(2);
				execvp(words[0], words);
				exit(0);
			}
			else{
				exit(0);
			}
		}
		else{
			wait(NULL);
		}

	}
	else {
		pid_t pid;
		pid = fork();
		if (pid < 0){
			fprintf(stderr, "Fork Failed");
			exit(-1);
		}
		else if (pid == 0){
			execvp(words[0], words);
			exit(0);
		}
		else{
			wait(NULL);
		}
	}
}

void runPipe(char **words1, char **words2){
	pid_t pid1, pid2;
	int pipe_fd[2];
	pipe(pipe_fd);

	pid1 = fork();
	if(pid1<0){
        fprintf(stderr, "fork failed");
        exit(-1);
    }
    else if(pid1==0){
        dup2(pipe_fd[1], 1);
        close(pipe_fd[0]);
		execvp(words1[0], words1);
    }

	//Second Process
	pid2 = fork();
	if(pid2<0){
        fprintf(stderr, "fork failed");
        exit(-1);
    }
    else if(pid2==0){
		dup2(pipe_fd[0], 0);
		close(pipe_fd[1]);
        execvp(words2[0], words2);
    }
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	wait(NULL);
	wait(NULL);
}

void redirect(char **words, char *file, int mode){
	pid_t pid;
    pid = fork();
    if(pid < 0){
        fprintf(stderr, "fork failed");
		exit(-1);
	}
	else if(pid == 0) {
        int file_open = (mode == 2 ? 
		open(file, O_CREAT|O_RDWR|O_TRUNC, S_IRUSR|S_IWUSR) : 
		open(file, O_RDONLY, S_IRUSR|S_IWUSR));
        
        dup2(file_open, (mode ==2 ? STDOUT_FILENO : STDIN_FILENO));
        execvp(words[0], words);
    }
	else {
		wait(NULL);
	}
}


int main(){
	char input[100];
	char **words = malloc(30 * sizeof(char *));
	char **words1 = malloc(30 * sizeof(char *));
	for(int i=0;i<30;i++) {words[i] = malloc(50);words1[i] = malloc(50);}
	printf("> ");
	while(fgets(input, 100, stdin)){
		if(strcmp(input, "\n") == 0) {printf("> ");continue;}
		if(strcmp(input, "exit\n") == 0) break;

		//string parser
		char *delim = " \n";
		char *word = strtok(input, delim);
		int n = 0, m = 0, zombie = 0, mode = 0;
		while(word != NULL){
			if(!strcmp(word, "|")) mode = 1;
			else if(!strcmp(word, ">")) mode = 2;
			else if(!strcmp(word, "<")) mode = 3;
			else if(mode) words1[m++] = word;
			else words[n++] = word;
			word = strtok(NULL, delim);
		}
		if (strcmp(words[n-1], "&") == 0){
			zombie = 1;
			n--;
		}
		words[n] = NULL;
		if(mode){
			if (strcmp(words1[m-1], "&") == 0){
				zombie = 1;
				m--;
			}
			words1[m] = NULL;
		}

		//fork another process
		if(mode == 0) run(words, zombie);
		else if(mode == 1) runPipe(words, words1);
		else if(mode == 2 || mode == 3) redirect(words, words1[0], mode);
		

		printf("> ");
	}
	return 0;
}
