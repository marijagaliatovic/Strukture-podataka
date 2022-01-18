#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_LINE 1024
#define MAX_SIZE 128
#define HASH_SIZE 11
#pragma warning(disable:4996)

struct _stablo;
typedef struct _stablo* position;
typedef struct _stablo
{
	char name[MAX_SIZE];
	int population;
	position left;
	position right;
}stablo;

struct _cvor;
typedef struct _cvor* pozicija;
typedef struct _cvor
{
	char ime[MAX_SIZE];
	position root;
	pozicija next;
}cvor;

int readFile(char* name, pozicija head,cvor HashTable[]);
int insertSorted(pozicija new, pozicija head);
int insertAfter(pozicija before, pozicija new);
int printList(pozicija head);
position readFileForTree(char* name, position root);
position insertToTree(position new, position current);
position createNewTreeElement(char* name, int number);
int printTree(position current);
position findCities(position current, int number);
int findCountry(cvor HashTablica[], char* country, int number);
int HashTable(cvor HashTablica[], position new);
int HashNumber(char* country);
int initializeList(pozicija head);

int main()
{
	cvor head = { .ime = { 0 },.root = NULL,.next = NULL };
	pozicija p = &head;
	char ime[MAX_SIZE] = { 0 };
	int broj;
	
	cvor HashTablica[HASH_SIZE];

	for (int i = 0; i < HASH_SIZE; i ++)
	{
		initializeList(&HashTablica[i]);
	}

	readFile("Drzave.txt",p,&HashTablica);
	
	/*for (int i = 0; i < HASH_SIZE; i++)
	{
		if (HashTablica[i].next != NULL)
		{
			printf("\n%d. lista:\n", i);
			printList(&HashTablica[i]);
		}
	}*/

	printf("Unesite ime drzave koju zelite pretraziti: ");
	scanf("%s", ime);

	printf("Unesite broj stanovnika: ");
	scanf("%d", &broj);

	findCountry(HashTablica,ime, broj);

	return 0;
}
int initializeList(pozicija head)
{
	memset(head->ime, 0, MAX_SIZE);
	head->root = NULL;
	head->next = NULL;

	return 0;
}
int findCountry(cvor HashTablica[], char* country, int number)
{
	int num = HashNumber(country);

	pozicija temp = &HashTablica[num];
	while (temp != NULL && strcmp(country, temp->ime) != 0)
		temp = temp->next;

	if (temp)
	{
		printf("Cities in %s that have population larger than %d: \n", country, number);
		temp->root = findCities(temp->root, number);
	}

	else
		printf("There is no such country in the file!\n");

	return 0;
}
position findCities(position current, int number)
{
	if (current == NULL)
	{
		return NULL;
	}

	else if (current->population < number)
	{
		current->right = findCities(current->right, number);
	}
	else if (current->population >= number)
	{
		current->left = findCities(current->left, number);
		printf("Name:%s  Population: %d\n", current->name, current->population);
		current->right = findCities(current->right, number);
	}

	return current;
}
int readFile(char* name, pozicija head,cvor HashTablica[])
{
	char buffer[MAX_LINE] = { 0 };
	char ime[MAX_LINE] = { 0 };
	int num;
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

		if (sscanf(buffer, " %s %s", new->ime, ime) == 2)
		{
			new->root = NULL;
			new->root = readFileForTree(ime, new->root);
			HashTable(HashTablica, new);
		}
	}
	fclose(pfile);
	return 0;
}

position readFileForTree(char* name, position root)
{
	char buffer[MAX_LINE] = { 0 };
	FILE* pfile = NULL;
	pfile = fopen(name, "r");
	if (!pfile)
	{
		printf("%s\n",name);
		printf("Failed opening of the file!\n");
		return -1;
	}

	while (!feof(pfile))
	{
		int number;
		char country[MAX_SIZE] = { 0 };
		position new = NULL;

		fgets(buffer, MAX_SIZE, pfile);
		if (sscanf(buffer, " %s %d", country, &number) == 2)
		{
			new = createNewTreeElement(country, number);
			root = insertToTree(new, root);
		}
	}
	fclose(pfile);
	return root;
}
int insertSorted(pozicija new, pozicija head)
{
	
	pozicija temp = head;
	while (temp->next != NULL && strcmp(temp->next->ime, new->ime) < 0)
		temp = temp->next;
	
	insertAfter(temp, new);

	return 0;
}
int insertAfter(pozicija before, pozicija new)
{
	new->next = before->next;
	before->next = new;

	return 0;
}
int printList(pozicija head)
{
	pozicija first = head->next;
	while (first)
	{
		printf("\nIme:%s\nGradovi:\n", first->ime);
		if (first->root)
			printTree(first->root);

		else
			printf("There is no cities in tree!\n");

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
	else if (current->population > new->population)
	{
		current->left = insertToTree(new, current->left);
	}
	else if (current->population < new->population)
	{
		current->right = insertToTree(new, current->right);
	}
	else//ako imaju jednako stanovnika
	{
		if (strcmp(current->name, new->name) > 0)
		{
			current->left = insertToTree(new, current->left);
		}

		else if (strcmp(current->name, new->name) < 0)
		{
			current->right = insertToTree(new, current->right);
		}
		else //ako je isti grad
			free(new);
	}
	return current;
}
position createNewTreeElement(char* name, int number)
{
	position new = NULL;

	new = (position)malloc(sizeof(stablo));
	if (!new)
	{
		perror("Failed allocation!\n");
		return NULL;
	}

	strcpy(new->name, name);
	new->population = number;
	new->left = NULL;
	new->right = NULL;

	return new;
}
int printTree(position current)
{
	if (NULL == current)
		return 0;

	printTree(current->left);
	printf("%s %d\n", current->name, current->population);
	printTree(current->right);

	return 0;
}
int HashNumber(char*country)
{
	int num = 0;

	num = country[0] + country[1] + country[2] + country[3] + country[4];
	return num%11;
}
int HashTable(cvor HashTablica[],position new)
{
	int num = HashNumber(new->name);

	insertSorted(new,&HashTablica[num]);

	return 0;
}