//Strukture podataka – 3 ispitni rok 28.8.2014.
//Napisati program koji za tri različita predmeta čita ime i prezime studenta te njegovu ocjenu.Podaci o
//svakom predmetu se nalaze u odvojenoj datoteci i iz svake datoteke je potrebno kreirati vezanu listu i
//ispisati je.
//a) Nakon toga potrebno je kreirati novu listu koja sadrži imena studenata koji su položili sva tri
//predmeta.
//b) Nadopuniti zadatak pod a) tako da se ispiše nova vezana lista koja osim što sadrži imena
//studenata, sadrži i njihovu prosječnu ocjenu iz ta tri predmeta.
#include<stdio.h>
#include<stdlib.h>
#define MAX_LINE 1024
#define MAX_SIZE 128
#pragma warning(disable:4996)

struct _predmet;
typedef struct _predmet* position;
typedef struct _predmet 
{
	char ime[MAX_SIZE];
	char prezime[MAX_SIZE];
	double ocjena;
	position next;
}predmet;

int readFile(char* name, position head);
position createNewElement(char* name, char* surname, double ocjena);
int insert(position new, position head);
int insertSorted(position new, position head);
int printList(position head);
double Average(double one, double two, double three);
int finalList(position head, position head1, position head2, position head3);

int main()
{
	predmet head = { .ime = {0},.prezime = {0},.ocjena = 0,.next = NULL };
	predmet head1 = { .ime = {0},.prezime = {0},.ocjena = 0,.next = NULL };
	predmet head2 = { .ime = {0},.prezime = {0},.ocjena = 0,.next = NULL };
	predmet head3 = { .ime = {0},.prezime = {0},.ocjena = 0,.next = NULL };

	readFile("matematika.txt", &head1);
	readFile("fizika.txt", &head2);
	readFile("programiranje.txt", &head3);

	printList(&head1);
	printf("\n");
	printList(&head2);
	printf("\n");
	printList(&head3);
	printf("\n");

	finalList(&head, &head1, &head2, &head3);
	printList(&head);
	printf("\n");
	return 0;
}

int readFile(char* name, position head)
{
	FILE* pfile = NULL;
	pfile = fopen(name, "r");
	if (!pfile)
	{
		printf("Failed opening of the file!\n");
		return -1;
	}

	char buffer[MAX_LINE];
	int status;
	double ocjena;
	char ime[MAX_LINE];
	char prezime[MAX_LINE];
	position new = NULL;

	while(!feof(pfile))
	{
		fgets(buffer, MAX_LINE, pfile);
		status=sscanf(buffer, " %s %s %lf ", ime, prezime, &ocjena);
		if (status == 3)
		{
			new = createNewElement(ime, prezime, ocjena);
			insertSorted(new, head);
		}
		else
			continue;
	}
	fclose(pfile);
	return 0;
}
position createNewElement(char*name,char*surname,double ocjena)
{
	position new = NULL;
	new = (position)malloc(sizeof(predmet));
	if (!new)
	{
		printf("Failed allocation of memory!\n");
		return NULL;
	}

	strcpy(new->ime, name);
	strcpy(new->prezime, surname);
	new->ocjena = ocjena;
	new->next = NULL;

	return new;
}
int insert(position new, position after)
{
	new->next = after->next;
	after->next = new;

	return 0;
}
int insertSorted(position new, position head)
{
	position temp = head;
	int result = 0;
	while (temp->next != NULL && strcmp(temp->next->ime, new->ime) <= 0)
	{
		if (strcmp(temp->next->ime, new->ime) == 0)
		{
			while( temp->next!=NULL && strcmp(temp->next->ime, new->ime) == 0 && strcmp(temp->next->prezime,new->prezime) < 0 )
			{
				temp = temp->next;
			}

			break;
		}
		
		temp = temp->next;
	}

	insert(new, temp);

	return 0;
}
int finalList(position head, position head1, position head2, position head3)
{
	position temp1 = head1;
	position temp2 = head2;
	position temp3 = head3;

	while (temp1->next != NULL)
	{
		while (strcmp(temp1->next->ime, temp2->next->ime) != 0)
			temp2 = temp2->next;

		while (strcmp(temp1->next->ime, temp3->next->ime) != 0)
			temp3 = temp3->next;

		if (temp1->next->ocjena > 1 && temp2->next->ocjena > 1 && temp3->next->ocjena > 1)
		{
			double average = Average(temp1->next->ocjena, temp2->next->ocjena, temp3->next->ocjena);
			position new = createNewElement(temp1->next->ime, temp2->next->prezime, average);
			insertSorted(new, head);

			temp1 = temp1->next;
			temp2 = head2;
			temp3 = head3;
		}

		else
		{
			temp1 = temp1->next;
			temp2 = head2;
			temp3 = head3;
		}
	}

}
double Average(double one, double two, double three)
{
	double sum = one + two + three;
	return sum / 3;
}
int printList(position head)
{
	position temp = head->next;
	while (temp)
	{
		printf("%s %s %lf\n", temp->ime, temp->prezime, temp->ocjena);
		temp = temp->next;
	}

	return 0;
}

