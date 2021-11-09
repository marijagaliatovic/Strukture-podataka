//4. Napisati program za zbrajanje i množenje polinoma.Koeficijenti i eksponenti se
//čitaju iz datoteke.
//Napomena: Eksponenti u datoteci nisu nužno sortirani
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


int main()
{
	polinom head1 = { .koef = 0,.expo = 0,.next = NULL };
	polinom head2 = { .koef = 0,.expo = 0,.next = NULL };
	polinom headSum = { .koef = 0,.expo = 0,.next = NULL };
	polinom headMultiply = { .koef = 0,.expo = 0,.next = NULL };
	char datoteka[MAX_SIZE] = { 0 };

	printf("Unesite ime datoteke: ");
	scanf(" %s", datoteka);
	//char datoteka[20] = "Polinomi.txt";

	readFile(datoteka, &head1, &head2);
	add(&head1,&head2,&headSum);
	multiply(&head1, &head2, &headMultiply);

	printf("\nKonacna suma:\n");
	printList(headSum.next);
	
	printf("\nKonacan umnozak:\n");
	printList(headMultiply.next);

	freeMemory(&head1);
	freeMemory(&head2);
	freeMemory(&headSum);
	freeMemory(&headMultiply);

	return 0;
}

int nameEntry(char* ime)//unosimo ime datoteke iz koje cemo procitati 
{
	printf("Unesite ime datoteke: ");
	scanf(" %s", ime);

	return 0;
}
int readFile(char* file, position head1, position head2)
{
	FILE* pfile = fopen(file, "r");//otvaramo datoteku
	char buffer[MAX_LINE] = { 0 };
	int status = 0;//ovo je varijabla u koju cemo spremati ono sta funkcija,koja trazi eksponente i koef,vrati

	if (!pfile)
	{
		perror("Neuspijelo otvaranje datoteke!\n");
		return -1;
	}

	fgets(buffer, MAX_LINE, pfile);//u buffer spremamo prvu liniju datoteke

	status = findPairs(buffer, head1);//pozivamo funkciju u kojoj cemo traziti sve parove eksponenata i koeficijenata
	//funkcija findPairs vraca 0 u slucaju da je unos ispravan

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
	int status = 0, expo = 0, koef = 0, n = 0;//u status spremamo koliko se varijabli skreniralo iz buffera(posto imamo samo koef i expo
											//da bi bija ispravanu unos status mora biti 2)
	position new = NULL;//nova struktura u koju cemo spremiti koeficijent i njegov eksponent ako su dobro ucitani iz datoteke
	char* currentBuffer = buffer;//deklariramo novi pokazivac 
	while (strlen(currentBuffer) > 0)//petlja se vrti dok je duljina niza vec od 0
		// duljina niza nece bit veca od nule kad ga pomaknemo na kraj?
	{
		status = sscanf(currentBuffer, " %d %d %n", &koef, &expo, &n);
		//printf("Status u funkciji: %d\n", status);
		if (status != 2)// ako status nije 2 varijable nisu tipa intigera ili npr. nema nego jedna brojka
		{
			return -1;
		}

		new = createNew(expo, koef, head);// ako je unos ispravan stvara se novi element liste
		if (!new)// provjeravamo je li se svorila nova osova (ako nije bit ce NULL pa ce returnat -1)
		{
			return -1;
		}

		currentBuffer += n; // pomicemo pokazivac za onoliko mista koliko smo skenirali sa  %n iz buffer
		// npr. u datoteci imamo redak: 123 4 5 6 2 1, u prvom krugu n ce biti 6(jer 123 su tri brojke,razmak je jedna,
		//cetvorka jos jedna i razmak nakon nje zadnja)
	}
	return 0;
}
position createNew(int expo, int koef, position head)// funkcija prima vrijednosti koje triba uniti i poziciju prvog elemnta
{
	position newElement = NULL; //otvaramo novu strukturu i alociramo memoriju za nju
	newElement = (position)malloc(sizeof(polinom));

	if (!newElement)//provjera alokacije
	{
		perror("Alokacija memorije nije uspijela!\n");
		return NULL;
	}

	newElement->expo = expo; //unos vrijendosti u novu strukturu
	newElement->koef = koef;

	sortedInput(head, newElement);//unos strukture u vezanu listu (sortirani unos)
	return newElement;
}
int sortedInput(position head, position new)
{
	position temp = head;
	while (temp->next != NULL && (temp->next->expo) < (new->expo)) //petlja ide dok je temp->next manji od NULL
		//ide do temp->next!=NULL jer ne provjerava vrijednosti koje se nalaze na temp nego one na strukturi koja se nalazi iza temp
		//kad se ne ispuni uvjet petlje temp ce zapravo pokazivati na element prije onog sa kojim smo usporedivali
		//izac ce iz petlje i pozvati funkciju umetni nakon  te ce nas novi element umetniti iza prethodnog od onog s kojim smo usporedivali
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
int add(position head1, position head2, position headSum)//tribamo zbrojiti dvi liste na nacin da se zbrajaju koeficijenti s istin eksponentima a ako
// u jednoj listi postoji koeficijent koji nema u drugoj moramo uniti sam taj koeficijent takoder u novu listu od zbroja
{
	position first = head1->next;//saljemo prvi element
	position second = head2->next;
	position result = headSum;//za rezultat zbrajanja moramo head jer u tu listu samo upisujemo pa da mozemo upisat iza heada
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
		
			second = second->next;//pomicen onog koji je manji
		}
		else
		{
			printf("Uslo u else\n");
			new=createNew(first->expo, first->koef, result);
	
			first = first->next;
		}
	}

	if (first == NULL)
	{
		printf("Uslo1\n");
		temp = second;
	}

	if (second == NULL)
	{
		printf("Uslo2\n");
		temp = first;
	}

	while (temp != NULL)
	{
		printf("Uslo u drugi while u add-u\n");
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
			//printf("\n Koef: %d Ekspo: %d\n", new->koef, new->expo);
			if (!new)
			{
				return -1;
			}
		}
	}
	return 0;
}

int merge(position nakon,position new)
{
	if(nakon->next==NULL || nakon->next->expo!= new->expo)
	{
		insertAfter(nakon, new);
	}
	else//ako novi i postojeci imaju isti eksponent
	{
		int koef = nakon->next->koef + new->koef;
		if (koef == 0)
		{
			DeleteAfter(nakon);
		}
		else
		{
			nakon->next->koef = koef;
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