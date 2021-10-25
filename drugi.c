#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_SIZE (128)

struct _person;
typedef struct _person* position;
typedef struct _person
{
	char name[MAX_SIZE];
	char surname[MAX_SIZE];
	int birthYear;
	position next;
}person;

position createPerson(char* name, char* surname, int birthYear);
int prependList(char* ime, char* prezime, int godina_rodenja, position head);
int appendList(char* ime, char* prezime, int godina_rodenja, position head);
int printList(position first);
int insertAfter(position last, position newPerson);
position findLast(position first);
position findPerson(position first, char* surname);
position findBefore(position first, char* surname);
int delete(position first, char* surname);

int main(int argc, char** argv)
{
	person head = { .name = {0},.surname = {0},.birthYear = 0,.next = NULL };
	position p = &head;
	position temp = NULL;
	char firstName[MAX_SIZE] = "First_name";
	char firstSurname[MAX_SIZE] = "First_surname";
	int firstBirthYear = 1980;
	char secondName[MAX_SIZE] = "Second_name";
	char secondSurname[MAX_SIZE] = "Second_surname";
	int secondBirthYear = 1990;

	prependList(firstName, firstSurname, firstBirthYear, p);
	appendList(secondName, secondSurname, secondBirthYear, p);
	printList(p->next);
	printf("\n");

	temp = findPerson(p->next, secondSurname);
	printList(temp);
	printf("\n");

	delete(p->next, secondSurname);
	printList(p->next);

	return 0;
}
position createPerson(char* name, char* surname, int birthYear)
{
	position newPerson = NULL;

	newPerson = (position)malloc(sizeof(person));
	if (!newPerson)
	{
		perror("Can't allocate memory!\n");
		return NULL;
	}
	strcpy(newPerson->name, name);
	strcpy(newPerson->surname, surname);
	newPerson->birthYear = birthYear;
	newPerson->next = NULL;

	return newPerson;
}
int prependList(char* name, char* surname, int birthYear, position head)
{
	position newPerson = NULL;

	newPerson = createPerson(name, surname, birthYear);
	if (!newPerson)
		return -1;

	insertAfter(head, newPerson);

	return 0;
}
int appendList(char* name, char* surname, int birthYear, position head)
{
	position newPerson = NULL;
	position last = NULL;

	newPerson = createPerson(name, surname, birthYear);
	if (!newPerson)
		return -1;

	last = findLast(head);
	insertAfter(last, newPerson);

	return 0;
}
int printList(position first)
{
	position temp = first;
	while (temp)
	{
		printf("Name: %s, surname: %s, "
			"birth year: %d\n", temp->name, temp->surname, temp->birthYear);
		temp = temp->next;
	}
	return 0;
}
int insertAfter(position last, position newPerson)
{
	newPerson->next = last->next;
	last->next = newPerson;

	return 0;
}
position findLast(position first)
{
	position temp = first;

	while (temp->next != NULL)
		temp = temp->next;

	return temp;
}

position findPerson(position first, char* surname)
{
	position temp = first;

	while (temp)
	{
		if (strcmp(temp->surname, surname) == 0)
			return temp;
		temp = temp->next;
	}
	return NULL;
}

position findBefore(position first, char* surname)
{
	position before = first;
	while (before)
	{
		if (strcmp(before->next->surname, surname) == 0)
			return before;
		before = before->next;
	}
	return NULL;
}

int delete(position first, char* surname)
{
	position toDelete = first;
	position before = findBefore(first, surname);

	if (!before)
	{
		perror("That surname is not on the list!");
		return-1;
	}

	toDelete = before->next;
	before->next = toDelete->next;
	free(toDelete);

	return 0;
}
