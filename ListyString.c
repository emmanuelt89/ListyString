#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ListyString.h"

int main (int argc, char **argv)
{
	processInputFile(argv[1]);

	return 0;
}


// ======================================================================================
int processInputFile(char *filename)
{
	// some code copied from lecture notes on "File I/O"
	char word[1024], str[1024], key[1];
	int i;
	ListyString *listy;

	FILE *filePtr = NULL;

	if ((filePtr = fopen(filename, "r")) == NULL)
		return 1;

	i = 0;	

	if (fscanf(filePtr, "%s", word) != EOF)
		listy = createListyString(word);


	while (fscanf(filePtr, "%s", word) != EOF)
	{
		if (word[0] == '@')
		{
			fscanf(filePtr, "%s%s",  key, str);
			replaceChar(listy, key[0], str);	
		}
		if (word[0] == '+')
		{
			fscanf(filePtr, "%s", str);
			listyCat(listy, str);
		}
		if (word[0] == '-')
		{
			fscanf(filePtr, "%s", key);
			replaceChar(listy, key[0], "");
		}
		if (word[0] == '~')
		{
			reverseListyString(listy);
		}
		if (word[0] == '?')
		{
			if (listyLength(listy) != -1)
				printf("%d\n", listy->length);
		}
		if (word[0] == '!')
		{
			printListyString(listy);
		}
	}
	
	fclose(filePtr);

	return 0;
}


// ======================================================================================
// createNode helper function
ListyNode *createNode(char listyChar)
{
	ListyNode *node = malloc(sizeof(ListyNode));

	node->data = listyChar;
	node->next = NULL;

	return node;
}
// ======================================================================================
ListyString *createListyString(char *str)
{
	int len, i;

	ListyString *listy = malloc(sizeof(ListyString));
	ListyNode *temp;

	if (str == NULL || str == '\0' || strlen(str) == 0)
	{
		listy->length = 0;
		listy->head = NULL;
		return listy;
	}

	len = strlen(str);
	listy->length = len;

	listy->head = createNode(str[0]);
	temp = listy->head;

	for (i = 1; i < len; i++)
	{
		temp->next = createNode(str[i]);
		temp = temp->next;
	}	

	return listy;
}


// ======================================================================================
ListyString *destroyListyString(ListyString *listy)
{
	int i;
	ListyNode *temp;

	if (listy == NULL || listy->head == NULL)
		return NULL;

	for (i = 0, temp = listy->head->next; i < listy->length; i++, listy->head = temp)
	{
		free(listy->head);
	}

	free(listy);
	free(temp);

	return NULL;
}


// ======================================================================================
ListyString *cloneListyString(ListyString *listy)
{
	int i;
	ListyNode *temp, *temp2;

	if (listy == NULL)
		return NULL;

	ListyString *clone = malloc(sizeof(ListyString));

	if (listy->head == NULL)
	{
		clone->head = NULL;
		clone->length = 0;

		return clone;
	}

	clone->head = createNode(listy->head->data);
	temp = listy->head;
	temp2 = clone->head;

	for ( ; temp->next != NULL; temp = temp->next, temp2 = temp2->next)
	{
		temp2->next = createNode(temp->next->data);
	}

	clone->length = listy->length;

	return clone;
}


// ======================================================================================
void replaceChar(ListyString *listy, char key, char *str)
{
	int i, len, flag;
	ListyNode *current, *head, *temp, *prev, *temp2, *nextToHead;


	if (listy == NULL || listy->head == NULL)
		return;

	if (str == NULL)
		len = 0;
	else
		len = strlen(str);

	flag = 0;
	current = listy->head;
	while (current != NULL)
	{	
	
	// if key occurs in the head
		if (listy->head->data == key && flag == 0)
		{

			// if deleting
			if (str == "" || str == NULL)
			{
				// make temp head node
				head = listy->head;
				// set new head
				listy->head = listy->head->next;

				// free old head
				free(head);
				// update length
				listy->length--;
				// update current
				current = listy->head;
				continue;
			}


			// if replacing
			else
			{

				// temp to hold original head
				nextToHead = listy->head->next;
				// new node to later replace listy->head
				listy->head->data = str[0];
				// create next set of nodes
				temp = listy->head;

				for (i = 1; i < len; i++)
				{	
					temp->next = createNode(str[i]);
					temp = temp->next;
				}
				// link last new node to the previous head->next
				temp->next = nextToHead;

				// update length
				listy->length += (len - 1);

				// update current for the while-loop
				current = temp;
			}
		}
		


		
		// if key occurs anywhere else
		else if (current->data == key)
		{

			// if deleting
			if (str[0] == '\0' || str == NULL || str == "")
			{	
				// set current temp
				temp = current;

				// set correct ->next
				prev->next = current->next;
				// free temp
				free(temp);
				// update current for correct node traversal
				current = prev;

				listy->length--;

			}


			// if replacing
			else
			{
				// set current temp
				temp = current;
				// create new head node and set to prev->next
				prev->next = createNode(str[0]);
				temp2 = prev->next;
				// create new nodes if needed
				for (i = 1; i < len; i++)
				{
					temp2->next = createNode(str[i]);
					temp2 = temp2->next;
				}
				// attach last new node to correct node & free
				temp2->next = temp->next;
				free(temp);
				// update current & prev
				current = temp2;
				prev = temp2;

				listy->length += (len - 1);
			}
		}

		prev = current;
		current = current->next;
		flag = 1;
	} // end of while

}


// ======================================================================================
void reverseListyString(ListyString *listy)
{	
	if (listy == NULL || listy->head == NULL || listy->length < 2)
		return;

	ListyNode *temp, *previous, *current;

	previous = NULL;
	current = listy->head;

	while (current != NULL)
	{
		temp = current->next;
		current->next = previous;
		previous = current;
		current = temp;
	}

	listy->head = previous;
}


// ======================================================================================
ListyString *listyCat(ListyString *listy, char *str)
{
	int i, len;
	ListyNode *temp;


	if (str == NULL || str == '\0')
		return listy;
	if (listy == NULL && str != '\0')
		return createListyString(str);
	if (listy == NULL && str == NULL)
		return NULL;
	if (listy == NULL && (str != NULL && str == '\0'))
		return createListyString(str);

	
	len = strlen(str);

	temp = listy->head;

	if (listy->head == NULL)
	{
		listy->head = createNode(str[0]);
		temp = listy->head;

		for (i = 1; i < len; i++)
		{
			temp->next = createNode(str[i]);
			temp = temp->next;
		}

	}

	else if (listy->head != NULL) 
	{
		for (temp = listy->head; temp->next != NULL; temp = temp->next)
			;

		for (i = 0; i < len; i++)
		{
			temp->next = createNode(str[i]);
			temp = temp->next;
		}
	}

	listy->length += len;

	return listy;
}


// ======================================================================================
int listyCmp(ListyString *listy1, ListyString *listy2)
{
	int i;
	ListyNode *temp1, *temp2;

	// check for all possible listy NULL combinations first
	if (listy1 == NULL && listy2 != NULL)
		return 1;
	if (listy2 == NULL && listy1 != NULL)
		return 1;
	if (listy1 == NULL && listy2 == NULL)
		return 0;
	if (listy1->length != listy2->length)
		return 1;
	if (listy1->head == NULL && listy2->head != NULL)
		return 1;
	if (listy2->head == NULL && listy1->head != NULL)
		return 1;


	if (listy1->head == NULL && listy2->head == NULL)
		return 0;

	temp1 = listy1->head;
	temp2 = listy2->head;

	if (listy1->length == listy2->length)
	{
		for (i = 0; i < listy1->length; i++)
		{
			if (temp1->data != temp2->data)
				return 1;

			temp1 = temp1->next;
			temp2 = temp2->next;
		}
	}

	return 0;
}


// ======================================================================================
int listyLength(ListyString *listy)
{
	if (listy == NULL)
		return -1;
	
	return listy->length;
}


// ======================================================================================
void printListyString(ListyString *listy)
{
	ListyNode *temp;

	if (listy == NULL || listy->head == NULL)
	{
		printf("(empty string)\n");
		return; 
	}

	for (temp = listy->head; temp != NULL; temp = temp->next)
		printf("%c", temp->data);
	printf("\n");
}


// ======================================================================================
double difficultyRating(void)
{
	return 5.0;
}


// =====================================================================================
double hoursSpent(void)
{
	return 20.0;
}

