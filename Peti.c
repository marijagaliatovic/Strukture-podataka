#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#pragma warning(disable:4996)

struct _StackElement;
typedef struct _StackElement* Position;

typedef struct _StackElement {
    double number;
    Position next;
}StackElement;


Position CreateElement(double number);
int InsertAfter(Position position, Position newElement);
int Push(Position head, double number);
int DeleteAfter(Position position);
int Pop(double* resultDestination, Position head);
int PerformOperation(Position head, char operator);
char* readPostfixFromFile(char* fileName);
int CalculatePostfix(double* resultDestination, char* fileName);
int DeleteAll(Position head);

Position CreateElement(double number) {
    Position newElement = NULL;

    newElement = (Position)malloc(sizeof(StackElement));
    if (!newElement) {
        return EXIT_FAILURE;
    }

    newElement->number = number;
    newElement->next = NULL;

    return newElement;
}

int InsertAfter(Position position, Position newElement) {
    newElement->next = position->next;
    position->next = newElement;
}

int Push(Position head, double number) {
    Position newElement = NULL;
    newElement = CreateElement(number);
    if (!newElement) {
        return EXIT_FAILURE;
    }

    InsertAfter(head, newElement);
    return EXIT_SUCCESS;
}

int DeleteAfter(Position position) {
    Position temp = position->next;
    if (!temp) {
        return EXIT_SUCCESS;
    }
    position->next = temp->next;
    free(temp);
    return EXIT_SUCCESS;
}

int Pop(double* resultDestination, Position head)
{
    Position first = head->next;

    if (!first)
    {
        printf("Postfix not valid! Please check your file ");
        return -1;
    }
    *resultDestination = first->number;
    DeleteAfter(head);

    return EXIT_SUCCESS;
}

int PerformOperation(Position head, char operator) {
    double operand1 = 0;
    double operand2 = 0;
    double result = 0;
    int status1 = EXIT_SUCCESS;
    int status2 = EXIT_SUCCESS;

    status2 = Pop(&operand2, head);
    if (status2 != EXIT_SUCCESS) {
        return -1;
    }

    status1 = Pop(&operand1, head);
    if (status1 != EXIT_SUCCESS) {
        return -2;
    }

    switch (operator)
    {
    case '+':
        result = operand1 + operand2;
        break;
    case '-':
        result = operand1 - operand2;
        break;
    case '*':
        result = operand1 * operand2;
        break;
    case '/':
        if (operand2 == 0)
        {
            perror("Can't divide with zero!\n");
            return -3;
        }
        result = operand1 / operand2;
        break;

    default:
        printf("This operation is not supported yet! Please pay and we will implement it.\n");
        return -4;
        break;
    }
    return Push(head, result);
}
char* readPostfixFromFile(char* fileName)
{
    FILE* file = NULL;
    int fileLength = 0;
    char* buffer = NULL;

    file = fopen(fileName, "rb");
    if (!file) {
        perror("Can't open file!\n");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    fileLength = ftell(file);

    buffer = (char*)calloc(fileLength + 1, sizeof(char));
    if (!buffer)
    {
        perror("Can't allocate memory");
        return NULL;
    }

    rewind(file);
    fread(buffer, sizeof(char), fileLength, file);
    printf("buffer:|%s|\n", buffer);

    fclose(file);

    return buffer;
}
int CalculatePostfix(double* resultDestination, char* fileName)
{
    char* buffer = NULL;
    char* currentbuffer = NULL;
    int numBytes = 0;
    double number = 0;
    int status = 0;
    char operation = 0;
    StackElement head = { .number = 0,.next = NULL };

    buffer = readPostfixFromFile(*fileName);
    if (!buffer)
    {
        perror("Reading of file falied!\n");
        return -1;
    }

    currentbuffer = buffer;
    while (strlen(currentbuffer) > 0)
    {
        status = sscanf(currentbuffer, " %lf %n", &number, &numBytes);
        if (status == 1)
        {
            status = Push(&head, number);
            if (status != EXIT_SUCCESS) {
                free(buffer);
                DeleteAll(&head);
                return -2;
            }
            currentbuffer += numBytes;
        }
        else
        {

            sscanf(currentbuffer, " %c %n", &operation, &numBytes);
            status = PerformOperation(&head, operation);
            if (status != EXIT_SUCCESS) {

                free(buffer);
                DeleteAll(&head);
                return -3;
            }
            currentbuffer += numBytes;
        }

    }
    free(buffer);

    status = Pop(resultDestination, &head);
    if (status != EXIT_SUCCESS)
    {
        DeleteAll(&head);
        return -4;
    }

    if (head.next)
    {
        printf("Postfix not valid! Please check your file.");
        DeleteAll(&head);
        return -5;
    }

    return EXIT_SUCCESS;
}

int DeleteAll(Position head) {
    while (head->next)
    {
        DeleteAfter(head);
    }
    return EXIT_SUCCESS;
}

int main()
{
    char fileName[128] = { 0 };
    double result;
    double* resultDestination = &result;

    printf("Enter file name: ");
    scanf(" %s", fileName);

    CalculatePostfix(resultDestination, fileName);

    printf("Result = %f\n", *resultDestination);

    return EXIT_SUCCESS;
}

