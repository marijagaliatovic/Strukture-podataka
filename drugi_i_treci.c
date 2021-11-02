#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#pragma warning(disable:4996)
#define MAX_SIZE (128)
#define MAX_LINE (1024)

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
int addAfterElement(char* name, char* surname, int birthYear, char* lastname, position first);
int addBeforeElement(char* name, char* surname, int birthYear, char* lastname, position first);
int printList(position first);
int insertAfter(position last, position newPerson);
position findLast(position first);
position findPerson(position first, char* surname);
position findBefore(position first, char* surname);
int delete(position first, char* surname);
int add_to_file(char* file, position head);
int read_from_file(char* file, position first);
int sortedInput(position p, position newPerson);



int main(int argc, char** argv)
{
	int i = 0, n = 0, odabir1 = 0, odabir2 = 0;
	person head = { .name = {0},.surname = {0},.birthYear = 0,.next = NULL };
	position p = &head;
	position newPerson = NULL;
	char name[MAX_LINE] = { 0 };
	char surname[MAX_LINE] = { 0 };
	char datoteka[MAX_LINE] = { 0 };
	char temp[MAX_LINE] = { 0 };
	int birthYear = 0;

	printf("Odabaerite:\n"
		"1-za unos liste iz datoteke\n"
		"2-za samostalan unos vezane liste\n"
		"Vas izbor: ");
	scanf("%d", &odabir1);

	if (odabir1 == 1)
	{
		printf("Unesite ime datoteke:");
		scanf(" %s", datoteka);
		read_from_file(datoteka, p);
		printList(p->next);
	}
	if (odabir1 == 2)
	{
		printf("Unesite koliko osoba zelite unijeti: ");
		scanf("%d", &n);
		for (i = 0; i < n; i++)
		{
			printf("Unesite ime, prezime i godinu rodenja: ");
			scanf(" %s %s %d", name, surname, &birthYear);
			newPerson = createPerson(name, surname, birthYear);
			sortedInput(p, newPerson);
		}
		printf("\n");
		printList(p->next);
		printf("\n");
		printf("Odaberite:\n"
			"1-za dodavanje elementa na pocetak liste \n"
			"2-za dodavanje elementa na kraj liste\n"
			"3-za dodavanje nakon odredenog elementa\n"
			"4-za dodavanje prije odredenog elemnta\n"
			"5-za brisanje elementa iz liste\n"
			"6-za unos liste u datoteku\n"
			"Vas izbor: ");
		scanf("%d", &odabir2);
		printf("\n");
		switch (odabir2)
		{
		case 1:
			printf("Unesite osobu:");
			scanf(" %s %s %d", name, surname, &birthYear);
			prependList(name, surname, birthYear, p);
			printList(p->next);
			break;
		case 2:
			printf("Unesite osobu:");
			scanf(" %s %s %d", name, surname, &birthYear);
			appendList(name, surname, birthYear, p);
			printList(p->next);
			break;
		case 3:
			printf("Unesite novu osobu:");
			scanf(" %s %s %d", name, surname, &birthYear);
			printf("Unesite prezime osobe nakon koje zelite dodati novu osobu: ");
			scanf(" %s", temp);
			addAfterElement(name, surname, birthYear, temp, p->next);
			printList(p->next);
			break;
		case 4:
			printf("Unesite novu osobu:");
			scanf(" %s %s %d", name, surname, &birthYear);
			printf("Unesite prezime osobe prije koje zelite dodati novu osobu: ");
			scanf(" %s", temp);
			addBeforeElement(name, surname, birthYear, temp, p->next);
			printList(p->next);
			break;
		case 5:
			printf("Unesite prezime osobe koju zelite izbrisati iz liste: ");
			scanf(" %s", surname);
			delete(p->next, surname);
			printList(p->next);
			break;
		case 6:
			printf("Unesite ime datoteke: ");
			scanf(" %s", datoteka);
			add_to_file(datoteka, p);
			break;
		default:
			printf("Uneseni broj ne pase niti jednom ponudenom!\n");
		}
	}
	if (odabir1 != 1 && odabir1 != 2)
	{
		printf("Unijeli ste krivu vrijenost!");
		return 1;
	}
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
int addAfterElement(char* name, char* surname, int birthYear, char* lastname, position first)
{
	position newPerson = NULL;
	position element = NULL;

	newPerson = createPerson(name, surname, birthYear);
	if (!newPerson)
		return -1;

	element = findPerson(first, lastname);

	insertAfter(element, newPerson);

	return 0;
}

int addBeforeElement(char* name, char* surname, int birthYear, char* lastname, position first)
{
	position newPerson = NULL;
	position element = NULL;

	newPerson = createPerson(name, surname, birthYear);
	if (!newPerson)
		return -1;

	element = findBefore(first, lastname);

	insertAfter(element, newPerson);

	return 0;
}

int add_to_file(char* file, position head)
{
	position temp = head;
	FILE* pfile = NULL;

	pfile = fopen(file, "w");
	if (!pfile)
	{
		perror("Otvaranje datoteke nije uspijelo!\n");
		return -1;
	}
	while (temp->next != NULL)
	{
		fprintf(pfile, "%s %s %d\n", temp->next->name, temp->next->surname, temp->next->birthYear);
		temp = temp->next;
	}
	fclose(pfile);
	return 0;
}
int sortedInput(position p, position newPerson)
{
	while (p->next != NULL && strcmp(p->next->surname, newPerson->surname) <= 0)
	{
		//sortiranje po prezimenu i imenu
		if (strcmp(p->next->surname, newPerson->surname) == 0 && strcmp(p->next->name, newPerson->name) <= 0)
		{
			p = p->next;
			break;
		}
		else if (strcmp(p->next->surname, newPerson->surname)==0 && strcmp(p->next->name, newPerson->name) > 0)
		{
			break;
		}
		p = p->next;
	}

	insertAfter(p, newPerson);

	return 0;
}
int read_from_file(char* file, position head)
{
	position temp = head;
	FILE* pfile = NULL;
	char buffer[MAX_LINE] = { 0 };


	pfile = fopen(file, "r");
	if (!pfile)
	{
		perror("Otvaranje datoteke nije uspijelo!\n");
		return -1;
	}


	while (!feof(pfile))
	{
		position newPerson = NULL;
		newPerson = (position)malloc(sizeof(person));
		if (!newPerson)
		{
			perror("Can't allocate memory!\n");
			return -1;
		}

		fgets(buffer, MAX_LINE, pfile);
		if (sscanf(buffer, " %s %s %d", newPerson->name, newPerson->surname, &newPerson->birthYear) == 3)
		{
			sortedInput(temp, newPerson);
		}

	}

	fclose(pfile);

	return 0;
}
