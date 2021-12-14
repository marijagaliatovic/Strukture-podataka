#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#pragma warning(disable:4996)
#define MAX_SIZE 128
#define MAX_LINE 1024

struct _cvor;
typedef struct _cvor* position;
typedef struct _cvor
{
	char name[MAX_SIZE];
	position sibling;
	position child;
}cvor;

struct _stog;
typedef struct _stog* StogPosition;
typedef struct _stog
{
	position VanjskiDirektorij;
	StogPosition next;
}stog;

position MakeDirectory(char* name, position current);
position InsertDirectory(position current, position new);
int PushToStog(StogPosition head, position VanjskiDirektorij);
int PopFromStog(StogPosition head);
StogPosition FindLast(StogPosition head);
StogPosition FindBefore(StogPosition head);
position ChangeDirectory(StogPosition head, position current, char* name);
int DisplayInformation(position current);
int Exit(position current);
int PrintCurrentDirectories(StogPosition head);
int Menu(position current, StogPosition head);

int main()
{
	cvor root;
	stog head;

	strcpy(root.name, "C:");
	root.child = NULL;
	root.sibling = NULL;

	head.VanjskiDirektorij = NULL;
	head.next = NULL;

	PushToStog(&head, &root);
	Menu(&root, &head);

	return 0;

}
//md
position MakeDirectory(char* name, position current)
{
	position new = NULL;
	new = (position)malloc(sizeof(cvor));
	if (!new)
	{
		perror("Failed allocation!\n");
		return NULL;
	}

	strcpy(new->name, name);
	new->child = NULL;
	new->sibling = NULL;

	current->child=InsertDirectory(current->child, new);

	return current;
}
position InsertDirectory(position current, position new)
{
	if (current == NULL)
		return new;

	else if (strcmp(current->name, new->name) > 0)
	{
		new->sibling = current;
		return new;
	}
	else if (strcmp(current->name, new->name) < 0)
	{
		current->sibling = InsertDirectory(current->sibling, new);
	}
	else
	{
		printf("There is already direcotry with same name!\n");
		free(new);
	}
	return current;
}
int PushToStog(StogPosition head,position VanjskiDirektorij)
{
	StogPosition new = NULL;
	new = (StogPosition)malloc(sizeof(stog));
	if (!new)
	{
		perror("Failed allocation of memmory!\n");
		return -1;
	}
	StogPosition last = FindLast(head);

	new->next = last->next;
	last->next = new;
	new->VanjskiDirektorij = VanjskiDirektorij;

	return 0;
}
StogPosition FindLast(StogPosition head)
{
	StogPosition temp = head;
	while (temp->next != NULL)
	{
		temp = temp->next;
	}

	return temp;
}
StogPosition FindBefore(StogPosition head)
{
	StogPosition temp = head;
	while (temp->next->next != NULL)
	{
		temp = temp->next;
	}

	return temp;
}
//cd..
int PopFromStog(StogPosition head)//izlazimo iz zadnjeg direktroija 
{
	if (!head->next)
		return 0;

	StogPosition before = FindBefore(head);
	StogPosition toDelete = NULL;

	toDelete = before->next;
	before->next = toDelete->next;
	free(toDelete);
	
	return 0; 
}
// cd "nameOfDiercotry"
position ChangeDirectory(StogPosition head,position current,char* name)
{
	if (NULL == current->child)//ako nema 
	{
		printf("The system cannot find the path specified.\n");
		return current;
	}
	else
	{
		position temp = current->child;
		while (temp != NULL && strcmp(temp->name,name)!=0)
			temp = temp->sibling;

		if (temp == NULL)
		{
			printf("The system cannot find the path specified.\n");
			return current;
		}
		else
		{
			PushToStog(head,current->child);
			return current->child;
		}
	}
}
//dir
int DisplayInformation(position current)
{
	if (NULL == current->child)
	{
		printf("Directory is empty.\n");
	}
	else
	{
		position temp = current->child;
		while (temp != NULL)
		{
			printf("<DIR>\t\t %s\n", temp->name);
			temp = temp->sibling;
		}
	}

	return 0;
}
int Exit(position current)
{

	if (current == NULL)
	{
		return 0;
	}

	Exit(current->sibling);
	Exit(current->child);
	Exit(current->sibling);

	free(current);

	return 0;
}
int PrintCurrentDirectories(StogPosition head)
{
	StogPosition temp = head->next;
	char line[MAX_LINE] = { 0 };

	while (temp!=NULL)
	{
		strcat(line, temp->VanjskiDirektorij->name);
		strcat(line,"/");//ne moze "\"
		temp = temp->next;
	}
	strcat(line, ">");

	printf("%s ", line);

	return 0;
}
int Menu(position current, StogPosition head)
{
	position first = current;
	char name[MAX_LINE] = { 0 };
	char command[MAX_LINE] = { 0 };
	char line[MAX_LINE] = { 0 };
	PrintCurrentDirectories(head);

	while (1)
	{
		fgets(line, MAX_LINE, stdin);
		sscanf(line, "%s %s", command, name);

		if (!strcmp(command, "md"))
		{
			current = MakeDirectory(name, current);
			//printf("Current= %s\nCurrent->child=%s\n",current->name,current->child->name);
			PrintCurrentDirectories(head);
		}
		else if(!strcmp(command, "cd"))
		{
			current=ChangeDirectory(head,current,name);
			PrintCurrentDirectories(head);
		}
		else if (!strcmp(command, "cd.."))
		{
			//ne smi izac iz C:
			PopFromStog(head);
			PrintCurrentDirectories(head);
		}
		else if (!strcmp(command, "dir"))
		{
			DisplayInformation(current);
			PrintCurrentDirectories(head);
		}
		else if (!strcmp(command, "exit"))
		{
			Exit(current);
			break;
		}
		else
		{
			printf("'%s' is not recognized as an internal or external command, operable program or batch file.\n", command);
			PrintCurrentDirectories(head);
		}
	}
	return 0;
}
