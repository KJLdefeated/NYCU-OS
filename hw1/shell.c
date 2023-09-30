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

int main(){
	char input[100];
	char **words = malloc(50 * sizeof(char *));
	for(int i=0;i<50;i++) words[i] = malloc(20);
	printf("> ");
	while(fgets(input, 100, stdin)){
		if(strcmp(input, "\n") == 0) {printf("> ");continue;}
		if(strcmp(input, "exit\n") == 0) break;

		//string parser
		char *delim = " \n";
		char *word = strtok(input, delim);
		int i = 0, zombie;
		while(word != NULL){
			words[i++] = word;
			word = strtok(NULL, delim);
		}
		if (strcmp(words[i-1], "&") == 0){
			zombie = 1;
			i--;
		}
		words[i] = NULL;

		//fork another process
		run(words, zombie);

		printf("> ");
	}
	return 0;
}
