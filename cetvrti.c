#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#pragma warning(disable:4996)
#define MAX_LINE 1024
#define MAX_SIZE 128
struct _polinom;
typedef struct _polinom* position;
typedef struct _polinom
{
	int koef;
	int expo;
	position next;
}polinom;

int nameEntry(char* ime);
int readFile(char* file, position head1, position head2);
int findPairs(char* buffer, position head);
position createNew(int expo, int koef, position head);
int sortedInput(position head, position new);
int insertAfter(position after, position new);
int add(position head1, position head2, position headSum);
int multiply(position head1, position head2, position headMultiply);
int printList(position first);
int merge(position nakon, position new);
int DeleteAfter(position element);
int freeMemory(position head);
int printPoly(position first);


int main()
{
	polinom head1 = { .koef = 0,.expo = 0,.next = NULL };
	polinom head2 = { .koef = 0,.expo = 0,.next = NULL };
	polinom headSum = { .koef = 0,.expo = 0,.next = NULL };
	polinom headMultiply = { .koef = 0,.expo = 0,.next = NULL };
	char datoteka[MAX_SIZE] = { 0 };

	printf("Unesite ime datoteke: ");
	scanf(" %s", datoteka);

	readFile(datoteka, &head1, &head2);
	add(&head1,&head2,&headSum);
	multiply(&head1, &head2, &headMultiply);

	/*printf("\nKonacna suma:\n");
	printList(headSum.next);
	
	printf("\nKonacan umnozak:\n");
	printList(headMultiply.next);*/

	printf("\nKonacna suma:\n");
	printPoly(headSum.next);

	printf("\nKonacan umnozak:\n");
	printPoly(headMultiply.next);

	freeMemory(&head1);
	freeMemory(&head2);
	freeMemory(&headSum);
	freeMemory(&headMultiply);

	return 0;
}

int nameEntry(char* ime)
{
	printf("Unesite ime datoteke: ");
	scanf(" %s", ime);

	return 0;
}
int readFile(char* file, position head1, position head2)
{
	FILE* pfile = fopen(file, "r");
	char buffer[MAX_LINE] = { 0 };
	int status = 0;

	if (!pfile)
	{
		perror("Neuspijelo otvaranje datoteke!\n");
		return -1;
	}

	fgets(buffer, MAX_LINE, pfile);
	status = findPairs(buffer, head1);
	if (status != 0)
	{
		return -1;
	}

	fgets(buffer, MAX_LINE, pfile);

	status = findPairs(buffer, head2);
	if (status)
	{
		return -1;
	}

	fclose(pfile);
	return 0;
}
int findPairs(char* buffer, position head)
{
	int status = 0, expo = 0, koef = 0, n = 0;
	position new = NULL;
	char* currentBuffer = buffer;
	while (strlen(currentBuffer) > 0)
	{
		status = sscanf(currentBuffer, " %d %d %n", &koef, &expo, &n);
		if (status != 2)
		{
			return -1;
		}

		new = createNew(expo, koef, head);
		if (!new)
		{
			return -1;
		}

		currentBuffer += n; 
	}
	return 0;
}
position createNew(int expo, int koef, position head)
{
	position newElement = NULL; 
	newElement = (position)malloc(sizeof(polinom));

	if (!newElement)
	{
		perror("Alokacija memorije nije uspijela!\n");
		return NULL;
	}

	newElement->expo = expo; 
	newElement->koef = koef;

	sortedInput(head, newElement);
	return newElement;
}
int sortedInput(position head, position new)
{
	position temp = head;
	while (temp->next != NULL && (temp->next->expo) > (new->expo))
	{
		temp = temp->next;
	}

	merge(temp, new);

	return 0;
}
int insertAfter(position after, position new)
{
	new->next = after->next;
	after->next = new;

	return 0;
}
int add(position head1, position head2, position headSum)
{
	position first = head1->next;
	position second = head2->next;
	position result = headSum;
	position temp = NULL;
	position new = NULL;

	while (first != NULL && second != NULL)
	{
		if (first->expo == second->expo)
		{
			new=createNew(first->expo, first->koef + second->koef, result);
			if (!new)
			{
				return -1;
			}
		
			first = first->next;
			second = second->next;
		}
		else if (first->expo > second->expo)
		{
			new=createNew(second->expo, second->koef, result);
		
			second = second->next;
		}
		else
		{
			new=createNew(first->expo, first->koef, result);
	
			first = first->next;
		}
	}

	if (first == NULL)
	{
		temp = second;
	}

	if (second == NULL)
	{
		temp = first;
	}

	while (temp != NULL)
	{
		createNew(temp->expo, temp->koef, result);
		temp = temp->next;
	}

	return 0;
}
int printList(position first)
{
	
	position temp = first;
	while (temp)
	{
		printf("koef: %d expo:%d\n", temp->koef, temp->expo);
		temp = temp->next;
	}
	return 0;
}

int multiply(position head1,position head2,position headMultiply)
{
	position first = NULL;
	position second = NULL;
	position result = headMultiply;

	for (first = head1->next; first != NULL; first = first->next)
	{
		for (second = head2->next; second != NULL; second = second->next)
		{
			position new = createNew(first->expo + second->expo, first->koef * second->koef, result);
			if (!new)
			{
				return -1;
			}
		}
	}
	return 0;
}

int merge(position after,position new)
{
	if(after->next==NULL || after->next->expo!= new->expo)
	{
		insertAfter(after, new);
	}
	else
	{
		int koef = after->next->koef + new->koef;
		if (koef == 0)
		{
			DeleteAfter(after);
		}
		else
		{
			after->next->koef = koef;
		}
		free(new);
	}
	return 0;
}

int DeleteAfter(position element)
{
	position toDelete=element->next;
	element->next = toDelete->next;
	free(toDelete);

	return 0;
}
int freeMemory(position head)
{
	position temp = NULL;
	while (head->next != NULL)
	{
		temp = head->next;
		head->next = temp->next;
		free(temp);
	}
	return 0;
}

int printPoly(position first)
{
	position temp = first;
	printf("f(x) = ");
	while (temp!=NULL)
	{
		if (temp->next== NULL)
		{
			printf("%dx^(%d)", temp->koef, temp->expo);
		}
		else
		{
			printf("%dx^(%d)", temp->koef, temp->expo);
			printf("+");
		}
		temp = temp->next;
	}
	return 0;
}
