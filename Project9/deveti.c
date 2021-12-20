#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#pragma warning(disable:4996)
#define MAX_SIZE 128
#define MAX_LINE 1024
#define lower 10
#define upper 90

struct _cvor;
typedef struct _cvor* position;
typedef struct _cvor
{
	int number;
	position left;
	position right;
}cvor;

struct _stog;
typedef struct _stog* StogPosition;
typedef struct _stog
{
	int element;
	StogPosition next;
}stog;

position insert(position current, position new);
position createNew(int number,position current);
int printInOrder(position current);
int replace(position current);
int DeleteAll(position current);
int PopStog(StogPosition head);
int PushStog(StogPosition head, int element);
int addToFile(StogPosition head,position root,char*name);
int addToStog(StogPosition head, position current);

int main()
	{
		position root = NULL;
		srand((unsigned)time(NULL));
		int i = 0,n;
		stog head;
		head.element = 0;
		head.next = NULL;
		char name[MAX_SIZE] = { 0 };

		printf("Enter file name : ");
		scanf(" %s", name);

		//a i b
		int niz[10] = { 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 };
	
		for(i=0;i<10;i++)
		{ 
			root = createNew(niz[i],root);
		}

		printInOrder(root);
		printf("\n");

		addToStog(&head, root);
		addToFile(&head, root, name);

		replace(root);

		printInOrder(root);
		printf("\n");

		addToStog(&head, root);
		addToFile(&head, root, name);

		DeleteAll(root);
		
		//c
		root = NULL;
		printf("\nEnter the number of nodes: ");
		scanf(" %d", &n);

		for (i = 0; i < n; i++)
		{
			root = createNew((rand() % (upper - lower - 1) + lower + 1), root);
		}
   
		printInOrder(root);

		return 0;
}
position insert(position current, position new)
{
	if (NULL == current)
	{
		return new;
	}
	else if (new->number < current->number)
	{
		current->right = insert(current->right, new);
	}
	else //ako je veci ili jednak
	{
		current->left = insert(current->left, new);
	}

	return current;
}

position createNew(int number,position current)
{
	position new = NULL;

	new = (position)malloc(sizeof(cvor));
	if (!new)
	{
		perror("Failed allocation!");
		return NULL;
	}

	new->number = number;
	new->left = NULL;
	new->right = NULL;

	new=insert(current, new);

	return new;
}
int printInOrder(position current)
{
	if (!current)
	{
		return 0;
	}

	printInOrder(current->left);
	printf("%d ", current->number);
	printInOrder(current->right);

	return 0;
}
int replace(position current)
{
	int sum = 0;
	int temp = 0;

	if (NULL != current)
	{
		sum = replace(current->left);
		sum += replace(current->right);
		
		temp = current->number;
		current->number = sum;
		sum += temp;
	}
	return sum;

}
int DeleteAll(position current)
{
	if (current == NULL)
	{
		return 0;
	}

	DeleteAll(current->left);
	DeleteAll(current->right);
	//printf("Brise: %d\n",current->number);

	free(current);

	return 0;
}
int PopStog(StogPosition head)
{
	if (!head->next)
	{
		return 0;
	}

	StogPosition toDelete = head->next;
	head->next = toDelete->next;
	free(toDelete);

	return 0;
}
int PushStog(StogPosition head, int element)
{
	StogPosition new = NULL;
	new = (StogPosition)malloc(sizeof(stog));
	if (!new)
	{
		perror("Failed allocation!\n");
		return -1;
	}

	new->element = element;
	new->next = head->next;
	head->next = new;

	return 0;
}
int addToStog(StogPosition head,position current)
{
	if (NULL == current)
	{
		return 0;
	}

	addToStog(head, current->right);

	PushStog(head, current->number);

	addToStog(head, current->left);


	return 0;
}
int addToFile(StogPosition head,position root,char*name)
{
	FILE* pfile = NULL;

	pfile = fopen(name, "a");
	if (!pfile)
	{
		perror("Failed opening of the file!\n");
		return -1;
	}

	while (head->next)
	{
		fprintf(pfile, "%d ", head->next->element);
		PopStog(head);
	}

	fprintf(pfile, "\n");

	fclose(pfile);

	return 0;
}