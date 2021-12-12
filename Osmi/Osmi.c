#include<stdio.h>
#include<stdlib.h>
#define MAX_LINE 1024
#define MAX_SIZE 1024
#pragma warning(disable:4996)
//stablo
struct _cvor;
typedef struct _cvor* position;
typedef struct _cvor
{
	int number;
	position left;
	position right;
}cvor;
//red
struct _queue;
typedef struct _queue* QueuePosition;
typedef struct _queue
{
	position Element;
	QueuePosition next;

}queue;
//funkcije
int menu(position root,QueuePosition head);
position insert(position current, position q);
position CreateNew(int number,position root);
int printInOrder(position current);
int printPostOrder(position current);
int printPreOrder(position current);
int printLevelOrder(position root,QueuePosition head);
position DeleteElement(int number, position root);
position FindElement(int number, position current);
position FindMin(position current);
int PushToQueue(position element, QueuePosition head);
int PopFromQueue(QueuePosition head);
QueuePosition CreateNewQueueElement(position element);
QueuePosition FindLast(QueuePosition head);
int DeleteAll(position current);

int main(int argc, char* argv[])
{
	position root = NULL;
	queue head;
	head.Element = 0;
	head.next = NULL;
	
	menu(root,&head);

	return 0;
}
int menu(position root,QueuePosition head)
{
	int choice, NumOfElements, i, number;

	printf("\nOdaberite:\n"
		"1-za dodavanje elementa/elemenata u stablo \n"
		"2-za brisanje trazenog\n"
		"3-za trazenje elementa\n"
		"4-za IN ORDER ispis\n"
		"5-za POST ORDER ispis\n"
		"6-za PRE ORDER ispis\n"
		"7-za LEVEL ORDER ispis\n"
		"8-za kraj programa\n"
		"\nVas izbor: ");
	scanf("%d", &choice);

	switch (choice)
	{
	case 1:
		printf("Unesite koliko zelite elemenata unijeti u binarno stablo: ");
		scanf("%d", &NumOfElements);
		for (i = 0; i < NumOfElements; i++)
		{
			printf("Unesite %d. element: ", i + 1);
			scanf("%d",&number);
			root=CreateNew(number,root);
		}
		menu(root,head);
		break;
	case 2:
		printf("Unesite koji element zelite izbrisati: ");
		scanf("%d",&number);
		printf("\n");
		root = DeleteElement(number, root);
		menu(root, head);
		break;
	case 3:
		printf("Unesite element koji zelite pronaci: ");
		scanf("%d", &number);
		
		FindElement(number, root);
		break;
	case 4:
		printInOrder(root);
		menu(root, head);
		break;
	case 5:
		printPostOrder(root);
		menu(root, head);
		break;
	case 6:
		printPreOrder(root);
		menu(root, head);
		break;
	case 7:
		printLevelOrder(root,head);
		menu(root, head);
		break;
	case 8:
		DeleteAll(root);
		break;
	default:
		menu(root,head);
		break;
	}
		return 0;
}
position CreateNew(int number,position root)
{
	position new = NULL;

	new = (position)malloc(sizeof(cvor));
	if (!new)
	{
		perror("Failed allocation!\n");
		return NULL;
	}

	new->number = number;
	//printf("new->number=%d\n", new->number);
	new->left = NULL;
	new->right = NULL;

	new = insert(root, new);
	return new;
}
position insert(position current, position q)
{
	if (current == NULL)
		return q;//jer je ce root postat q ako je root prazan 

	else if (current->number < q->number)
		current->right = insert(current->right, q);

	else if (current->number > q->number)
		current->left = insert(current->left, q);

	else//ako su jednaki,binarno stablo je skup nema ponavljajuce podatke
		free(q);

	return current;
}
//inorder
int printInOrder(position current)
{
	if (current == NULL) 
	{ 
		return 0; 
	}

	printInOrder(current->left);
	printf("%d ", current->number);
	printInOrder(current->right);

	return 0;
}
//postorder
int printPostOrder(position current)
{
	if (current == NULL)
	{
		return 0;
	}

	printPostOrder(current->left);
	printPostOrder(current->right);
	printf("%d ", current->number);

	return 0;
}
//preorder
int printPreOrder(position current)
{
	if (current == NULL)
	{
		return 0;
	}

	printf("%d ", current->number);
	printPreOrder(current->left);
	printPreOrder(current->right);

	return 0;
}
//level order
int printLevelOrder(position root,QueuePosition head)
{
	if (NULL==root)
	{
		printf("Binary tree is empty!\n");
		return 0;
	}
	position current =NULL;
	PushToQueue(root,head);

	while (head->next!=NULL) 
	{
		current = head->next->Element;
		printf("%d ", current->number);
		if (current->left != NULL)
		{
			PushToQueue(current->left, head);
		}
		if (current->right != NULL)
		{
			PushToQueue(current->right, head);
		}
		PopFromQueue(head);
		
	}
	return 0;
}
QueuePosition CreateNewQueueElement(position element)
{
	QueuePosition new = NULL;
	new = (QueuePosition)malloc(sizeof(queue));
	if (!new)
	{
		perror("Failed allocation!\n");
		return NULL;
	}

	new->Element = element;
	new->next = NULL;

	return new;
}
int PushToQueue(position element,QueuePosition head)
{
	QueuePosition last = NULL;
	last=FindLast(head);

	last->next = CreateNewQueueElement(element);
	if (!head->next)
	{
		printf("There is no new element!");
		return -1;
	}
	
	last->next->next = NULL;

	return 0;
}
int PopFromQueue(QueuePosition head)
{
	QueuePosition toDelete = head->next;
	if(!toDelete)
	{
		printf("Queue is empty!\n");
		return 0;
	}
	
	head->next = toDelete->next;
	free(toDelete);

	return 0;
}
QueuePosition FindLast(QueuePosition head)
{
	QueuePosition temp = head;
	while (temp->next != NULL)
		temp = temp->next;

	return temp;//ako je lista prazna temp ce ostat na headu i stavljamo nakon heada
}
position DeleteElement(int number, position current)
{
	if (NULL == current)
	{
		printf("There is no such element in binary tree!\n");
	}
	
	else if (number < current->number)
	{
			current->left = DeleteElement(number, current->left);
	}

	else if (number > current->number)
	{
			current->right = DeleteElement(number, current->right);
	}
	else
	{ 
		if (current->left != NULL && current->right != NULL)//ako ima dvoje dice
		{
			position temp = FindMin(current->right);

			current->number = temp->number;

			current->right = DeleteElement(current->number, current->right);
		}
		else
		{
			position temp = current;

			if (NULL == current->left)
			{
				current = current->right;
			}
			else
			{
				current = current->left;
			}
			free(temp);
		}
			
	}
	return current;
}
position FindElement(int number, position current)
{
	if (NULL == current)
	{
		printf("There is no such element in binary tree!\n");
	}
	else
	{
		if (number < current->number)
		{
			return FindElement(number, current->left);
		}
		else if (number > current->number)
		{
			return  FindElement(number, current->right);
		}
		else
			return current;
	}
	return current;
}
position FindMin(position current)
{
	if (NULL == current) 
	{
		return NULL;
	}
	else 
	{
		if (NULL == current->left)
		{
			return current;
		}
		else //ako postoji livo dite
		{
			return FindMin(current->left);
		}
	}
}
int DeleteAll(position current)
{
	if (current == NULL)
	{
		return 0;
	}
	DeleteAll(current->left);
	DeleteAll(current->right);

	printf("\n Deleting element: %d",current->number);
	free(current);

	return 0;
}