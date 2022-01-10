#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_LINE 1024
#define MAX_SIZE 128
#pragma warning(disable:4996)

struct _cvor;
typedef struct _cvor* pozicija;
typedef struct _cvor
{
	char ime[MAX_SIZE];
	int population;
	pozicija next;
}cvor;

struct _stablo;
typedef struct _stablo* position;
typedef struct _stablo
{
	char name[MAX_SIZE];
	cvor head;
	position left;
	position right;
}stablo;

int initializeList(pozicija headElement);
int readFile(char* name, pozicija head);
int insertSorted(pozicija new, pozicija head);
int insertAfter(pozicija before, pozicija new);
int printList(pozicija head);
position readFileForTree(char* name, position root);
position insertToTree(position new, position current);
position createNewTreeElement(char* name);
int printTree(position current);
int compareCities(pozicija new, pozicija old);
int findCity(pozicija head, int number);
position findCountry(position current, char* name, int number);

int main()
{
	position root = NULL;
	char ime[MAX_SIZE] = { 0 };
	int broj;

	root = readFileForTree("Drzave.txt", root);

	printTree(root);

	printf("\nUnesite ime drzave koju zelite pretraziti: ");
	scanf("%s", ime);

	printf("Unesite broj stanovnika: ");
	scanf("%d", &broj);

	findCountry(root, ime, broj);

	return 0;
}
int findCity(pozicija head, int number)
{
	pozicija temp = head->next;

	while (temp)
	{
		if (temp->population >= number)
		{
			printf("Name: %s  Population: %d\n", temp->ime, temp->population);
		}
		temp = temp->next;
	}
	
	return 0;
}
position findCountry(position current, char* name,int number)
{
	if (current == NULL)
	{
		printf("There is no such country!\n");
		return NULL;
	}

	else if (strcmp(current->name, name) < 0)
	{
		current->right = findCountry(current->right, name,number);
	}
	else if (strcmp(current->name, name) > 0)
	{
		current->left = findCountry(current->left, name,number);
	}
	else//ako su iste
	{
		printf("Cities in % s that have larger population than % d:\n", name, number);
		findCity(&current->head,number);
	}
	return current;
}
int initializeList(pozicija headElement)
{
	memset(headElement->ime, 0, MAX_SIZE);
	headElement->population = 0;
	headElement->next = NULL;

	return 0;
}
position readFileForTree(char* name, position root)
{
	char buffer[MAX_LINE] = { 0 };
	FILE* pfile = NULL;
	pfile = fopen(name, "r");
	if (!pfile)
	{
		printf("Failed opening of the file!\n");
		return NULL;
	}

	while (!feof(pfile))
	{
		char country[MAX_SIZE] = { 0 };
		char ime[MAX_SIZE] = { 0 };
		position new = NULL;

		fgets(buffer, MAX_SIZE, pfile);
		if (sscanf(buffer, " %s %s", country, ime) == 2)
		{
			new = createNewTreeElement(country);
			readFile(ime, &new->head);
			root = insertToTree(new, root);
		}
	}
	fclose(pfile);
	return root;
}
int readFile(char* name, pozicija head)
{
	char buffer[MAX_LINE] = { 0 };
	FILE* pfile = NULL;

	pfile = fopen(name, "r");
	if (!pfile)
	{
		printf("Failed opening of the file!\n");
		return -1;
	}

	while (!feof(pfile))
	{
		pozicija new = NULL;
		new = (pozicija)malloc(sizeof(cvor));
		if (!new)
		{
			perror("Failed allocation");
			return -1;
		}

		fgets(buffer, MAX_SIZE, pfile);
		if (sscanf(buffer, " %s %d", new->ime, &new->population) == 2)
		{
			insertSorted(new, head);
		}
	}
	fclose(pfile);
	return 0;
}
int insertSorted(pozicija new, pozicija head)
{
	pozicija temp = head;
	while (temp->next != NULL && compareCities(new,temp->next)<0)
	{
		temp = temp->next;
	}
	insertAfter(temp, new);

	return 0;
}
int compareCities(pozicija new, pozicija old)
{
	int result=new->population-old->population;

	if (new->population == old->population)
	{
		result = (strcmp(old->ime, new->ime) < 0);
	}
		return  result;
}
int insertAfter(pozicija before, pozicija new)
{
	new->next = before->next;
	before->next = new;

	return 0;
}
int printList(pozicija first)
{
	while (first)
	{
		printf("%s %d\n", first->ime, first->population);
		first = first->next;
	}
	return 0;
}
position insertToTree(position new, position current)
{
	if (current == NULL)
	{
		return new;
	}
	else if (strcmp(current->name, new->name) > 0)
	{
		current->left = insertToTree(new, current->left);
	}
	else if (strcmp(current->name, new->name) < 0)
	{
		current->right = insertToTree(new, current->right);
	}
	else
		free(new);

	return current;
}
position createNewTreeElement(char* name)
{
	position new = NULL;

	new = (position)malloc(sizeof(stablo));
	if (!new)
	{
		perror("Failed allocation!\n");
		return NULL;
	}

	strcpy(new->name, name);
	initializeList(&new->head);
	new->left = NULL;
	new->right = NULL;

	return new;
}
int printTree(position current)
{
	if (NULL == current)
		return 0;

	printTree(current->left);
	printf("\n%s\n", current->name);
	printList(current->head.next);
	printTree(current->right);

	return 0;
}