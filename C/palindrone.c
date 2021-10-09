#include "ADTs/stack.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

void palindrome(FILE *fp, const Stack *st){
	
	char buf[BUFSIZ];
	while(fgets(buf, sizeof buf, fp) != NULL){
		int i = 0;
		int len = strlen(buf)-1; //buf includes '\n'
		int mid = len / 2;
		bool isPalindrome;
		
		for(i = 0; i < mid; i++){
			char c = buf[i];
			(void)st->push(st, (void *)(long)c);
		}

		if(len % 2 != 0) //for odd situation
			i++;

		while(buf[i] != '\n'){
			isPalindrome = true;
			char c;
			st->pop(st, (void **)&c);

			if(c != buf[i]){
				isPalindrome = false;
			}
			i++;
		}
		
		if(isPalindrome){
			fprintf(stdout, "Palindrome: ");
			fprintf(stdout, "%s", buf);
		} else {
			fprintf(stdout, "Not Palindrome: ");
			fprintf(stdout, "%s", buf);
		}
	}
}


int main(int argc, char *argv[]){
	FILE * fp = NULL;
	const Stack * st = Stack_create(free);
	int exitStatus = EXIT_FAILURE;

	if (argc != 2){
		fprintf(stderr, "usage: %s file\n", argv[0]);
		goto cleanup;
	}

	if((fp = fopen(argv[1], "r")) == NULL){
		fprintf(stderr, "cannot open file %s\n", argv[0]);
		goto cleanup;
	}

	if(st == NULL){
		fprintf(stderr, "%s: cannot create stack ADT instance\n", argv[0]);
		goto cleanup;
	}

	palindrome(fp, st);

	exitStatus = EXIT_SUCCESS;


cleanup:
	if(st != NULL)
		st->destroy(st);
	if(fp != NULL)
		fclose(fp);
	return exitStatus;
}
