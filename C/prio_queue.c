#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "llistprioqueue.h"
#include "prioqueue.h"

#define BUFSIZE 128

int cmp(void *a, void *b){
	if(*(int *)a < *(int *)b)
		return -1;
	else
		return 0;
}

void freePrio(void *p){
	if(p != NULL)
		free((int *)p);
}

void freeValue(void *v){
	if(v != NULL)
		free((char *)v);
}

int main(int argc, char *argv[]){
	const PrioQueue *pq = NULL;
	const Iterator *it = NULL;
	char buffer[BUFSIZE];
	int status = EXIT_FAILURE;
	FILE *pFile = NULL;
	int *priority = NULL;
	char *word = NULL;
	long lineno = 0;
		
	pq = LListPrioQueue(cmp, freePrio, freeValue);
	if(pq == NULL){
		fprintf(stderr, "Unable to create priority queue\n");
		goto cleanup;
	}

	if(argc != 2){
		fprintf(stderr, "Uage: %s fileName \n", argv[0]);
		goto cleanup;
	}

	pFile = fopen(argv[1], "r");

	if(pFile == NULL){
		fprintf(stderr, "Unable to open file: %s !\n", argv[1]);
		goto cleanup;
	}else{
		while(fgets(buffer, BUFSIZE, pFile) != NULL){
			lineno++;
			char *intString = strtok(buffer, " ");
			char *charString = strtok(NULL, " ");

			priority = (int *)malloc(sizeof(int));
			word = (char *)malloc(sizeof(char) * 30);
			*priority = atoi(intString);
			strcpy(word, charString);

			if(pq->insert(pq, (void *)priority, (void *)word) == 0){
				fprintf(stderr, "Error inserting line %ld into priority queque\n", lineno);
				goto cleanup;
			}
		}
		fclose(pFile);
	}

	it = pq->itCreate(pq);
	if(it != NULL){
		while(it->hasNext(it)) {
			char *s;
			(void)it->next(it, (void **)&s);
			fputs(s, stdout);
		}
		it->destroy(it);
	} else {
		fprintf(stderr, "Unable to create iterator over priority queue\n");
		goto cleanup;
	}


	(void)pq->removeMin(pq, (void *)&priority, (void *)&word);
	printf("Remove first element of priority %d from pq: %s\n", *priority, word);
	(void)pq->min(pq, (void *)&word);
	printf("Min after removed previous Min: %s\n", word);

	status = EXIT_SUCCESS;

	cleanup:
	if(pq != NULL)
		pq->destroy(pq);
			
	return status;
}
