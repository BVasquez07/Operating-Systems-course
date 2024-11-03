#include <stdio.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>



/*struct to define a Node*/

struct Node{
	char* data;
	struct Node *next;
	struct Node *prev;
};

/*Helper functions*/
void prompt(){
	printf("What else do you want? To insert a new node press 1, to insert a new node after a given node press 2, to delete a node press 3, to exit press 0: \n");
};

void insertAtEnd(char* data, struct Node* head){
	struct Node *newNode;
	newNode = malloc(sizeof(struct Node));
	newNode->data = strdup(data);
	newNode->next = NULL;
	struct Node *temp = head;
	while(temp->next != NULL){
		temp = temp->next;
	}
	temp->next = newNode;
	newNode->prev = temp;
}



void insertAfter(char* data, int previous, struct Node *head){
	struct Node *newNode;
	newNode = malloc(sizeof(struct Node));
	newNode->data = strdup(data);


	struct Node *currentNode = head;
	int count = 1;
	while(currentNode != NULL && count != previous){
		currentNode = currentNode->next;
		count++;
	};
	if(currentNode != NULL){
		newNode->next = currentNode->next;
		currentNode->next->prev = newNode;
		currentNode->next = newNode;
		newNode->prev = currentNode;
	};
}

void deleteAtHead(struct Node* head){
	if(head == NULL){
		printf("\n");
	}

    	head = head->next;                  
}

void deleteNode(int node_num, struct Node *head){
	struct Node *currentNode = head;
	struct Node *prevNode = NULL;
	if(node_num == 1){
		deleteAtHead(head);
	}
	int count = 1;
	while(currentNode != NULL){
		if(node_num == count){
			prevNode->next = currentNode->next;
			break;
		}
		count++;
		prevNode = currentNode;
		currentNode = currentNode->next;
	}

};

void printList(struct Node* head){
	struct Node *temp = head;
	while(temp != NULL) {
		printf(temp->data);
		temp = temp->next;
	}
}

void printReverseList(struct Node* head){
	struct Node *temp = head;
	while(temp->next != NULL) {
                temp = temp->next;
        }

	while(temp != NULL){
		printf(temp->data);
		temp = temp->prev;
	};
}



struct Node* buildLL(char* sourceFile){
 	size_t buffer_size = 128;
        size_t length = 0;
        char* char_buff = (char*)malloc(buffer_size);
        int source_f = open(sourceFile, O_RDONLY);
        size_t bytes_read;

        if(source_f == -1){
                perror("Failed to open file");
                exit(1);
        };
        if(char_buff == NULL){
                perror("Memory allocation failed");
                close(source_f);
                exit(1);
        };

	struct Node *head = NULL;
	struct Node *currNode = head;

        while ((bytes_read = read(source_f, char_buff + length, buffer_size - length - 1)) > 0) {	
		length += bytes_read;
		char_buff[length] = '\0';

		struct Node* newNode = malloc(sizeof(struct Node));
		if(newNode == NULL){
			perror("Node memory allocation failed");
			free(char_buff);
			close(source_f);
			exit(1);
		}

		
		newNode->data = strdup(char_buff);
		newNode->next = NULL;
		if (head == NULL) {
            		head = newNode;
			newNode->prev = NULL;
        	} else {
            		currNode->next = newNode;
			newNode->prev = currNode;/*might not work....*/
        	}
        	currNode = newNode;  /*move currPointer to the latest node*/


                if(length >= buffer_size - 1){
                        buffer_size *= 2;
                        char* new_buff = (char*)realloc(char_buff, buffer_size);
                        if(new_buff == NULL){
                                perror("Memory reallocation failed");
                                free(char_buff);
                                close(source_f);
                                exit(1);
                        };
                        char_buff = new_buff;
                }

		length = 0;
        };

        close(source_f);
	free(char_buff);

	return head;
}


/*Main prog...*/

int main(int argc, char *argv[]){
	struct Node *head = malloc(sizeof(struct Node)); /*need our head pointer to keep track of our list*/
	head = buildLL(argv[1]); /*use build LL which will retrn a head pointer*/
	char userInput = -1;
		
	while(userInput != '0'){
		prompt();	
		scanf("  %c", &userInput);
		printf("Your input is: %c\n", userInput);
		if(userInput == '1'){
			char input_data[256];
			printf("Input the data for the node.\n");
		       	scanf("  %255s", input_data);
			insertAtEnd(input_data, head);
			printf("\n");
			printList(head);
		}
		else if(userInput == '2'){
			char input_data[256];
			int input_prev;
			printf("Input the data for the node.\n");
                        scanf("  %255s", input_data);
			printf("Input the node number where you would like to enter this new node.");
			scanf("  %d", &input_prev);
                        
			insertAfter(input_data, input_prev, head);
                        printf("\n");
			printList(head);
		
		}
		else if (userInput == '3'){
			int input_loc;
                        printf("Input the node number of the node you would like to delete");
                        scanf("  %d", &input_loc);
			deleteNode(input_loc, head);
			printList(head);
		}
		else{
			printList(head);
			printf("\n");
			printReverseList(head);
			printf("\n");
			break;
		}
	
	};
}

