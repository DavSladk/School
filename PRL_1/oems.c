/*
    Author: David Sladký
    Description: Implementation of Odd-Even Merge Sort algorithm for 8 numbers
*/
#include "oems.h"

/*
    numbers - array of char to send

    description: Takes array of uchars and sends them to initial processors for sorting.
*/
void sendToInitialProcessors(unsigned char* numbers)
{
    for (int i = 0; i < NUMBER_COUNT/2; i++)
    {
        MPI_Send(&numbers[2*i], 1, MPI_UNSIGNED_CHAR, i+1, 0, MPI_COMM_WORLD);
        MPI_Send(&numbers[2*i+1], 1, MPI_UNSIGNED_CHAR, i+1, 0, MPI_COMM_WORLD);
    }

    return;
}

/*
    numbers - buffer for accepting sorted numbers

    description: Receives sorted numbers from processor net.
*/
void recvFromFinalProcessors(unsigned char* numbers)
{
    MPI_Recv(&numbers[0], 1, MPI_UNSIGNED_CHAR, 11,  0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&numbers[1], 1, MPI_UNSIGNED_CHAR, 17, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&numbers[2], 1, MPI_UNSIGNED_CHAR, 17, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&numbers[3], 1, MPI_UNSIGNED_CHAR, 18, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&numbers[4], 1, MPI_UNSIGNED_CHAR, 18, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&numbers[5], 1, MPI_UNSIGNED_CHAR, 19, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&numbers[6], 1, MPI_UNSIGNED_CHAR, 19, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&numbers[7], 1, MPI_UNSIGNED_CHAR, 14, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    return;
}

/*
    numbers - buffer of numbers to print

    description: Prints numbers with spaces in between them.
*/
void printInitialSequence(unsigned char* numbers)
{
    printf("%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu\n", \
                numbers[0], \
                numbers[1], \
                numbers[2], \
                numbers[3], \
                numbers[4], \
                numbers[5], \
                numbers[6], \
                numbers[7]);
    return;
}

/*
    numbers - buffer of numbers to print

    description: Prints numbers with line breaks in between them.
*/
void printFinalSequence(unsigned char* numbers)
{
    printf("%hhu\n%hhu\n%hhu\n%hhu\n%hhu\n%hhu\n%hhu\n%hhu\n",
                numbers[0], \
                numbers[1], \
                numbers[2], \
                numbers[3], \
                numbers[4], \
                numbers[5], \
                numbers[6], \
                numbers[7]);
    return;
}

/*
    description: Function controlling the flow of control process.
*/
void runControlProcess()
{
    FILE *inputFile;
    unsigned char numbers[NUMBER_COUNT];

    inputFile = fopen(FILE_NAME, FILE_MODE);
    fread(numbers, NUMBER_SIZE, NUMBER_COUNT, inputFile);
    fclose(inputFile);

    printInitialSequence(numbers);

    sendToInitialProcessors(numbers);
    recvFromFinalProcessors(numbers);

    printFinalSequence(numbers);

    return;
}

/*
    inputA - process to load first number from
    inputB - process to load second number from
    outputMin - process to send smaller number to
    outputMax - process to send larger number to

    description: Function controlling the flow of sorting processes.
*/
void runSortingProcess(int inputA, int inputB, int outputMin, int outputMax)
{
    unsigned char numberA;
    unsigned char numberB;

    MPI_Recv(&numberA, 1, MPI_UNSIGNED_CHAR, inputA, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&numberB, 1, MPI_UNSIGNED_CHAR, inputB, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    if(numberA < numberB)
    {
        MPI_Send(&numberA, 1, MPI_UNSIGNED_CHAR, outputMin, 0, MPI_COMM_WORLD);
        MPI_Send(&numberB, 1, MPI_UNSIGNED_CHAR, outputMax, 0, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Send(&numberB, 1, MPI_UNSIGNED_CHAR, outputMin, 0, MPI_COMM_WORLD);
        MPI_Send(&numberA, 1, MPI_UNSIGNED_CHAR, outputMax, 0, MPI_COMM_WORLD);
    }

    return;
}

/*
    rank - rank of procces

    description: Runs function for processes based on their rank.
*/
void runProcess(int rank)
{
    switch (rank)
    {
    case 0:
        runControlProcess();
        break;
    
    case 1:
        runSortingProcess(0,0,5,6);
        break;

    case 2:
        runSortingProcess(0,0,5,6);
        break;

    case 3:
        runSortingProcess(0,0,7,8);
        break;

    case 4:
        runSortingProcess(0,0,7,8);
        break;

    case 5:
        runSortingProcess(1,2,11,9);
        break;
    
    case 6:
        runSortingProcess(1,2,9,14);
        break;

    case 7:
        runSortingProcess(3,4,11,10);
        break;

    case 8:
        runSortingProcess(3,4,10,14);
        break;

    case 9:
        runSortingProcess(5,6,13,12);
        break;

    case 10:
        runSortingProcess(7,8,13,12);
        break;

    case 11:
        runSortingProcess(5,7,0,15);
        break;

    case 12:
        runSortingProcess(9,10,15,19);
        break;

    case 13:
        runSortingProcess(9,10,17,16);
        break;

    case 14:
        runSortingProcess(6,8,16,0);
        break;

    case 15:
        runSortingProcess(11,12,17,18);
        break;

    case 16:
        runSortingProcess(13,14,18,19);
        break;

    case 17:
        runSortingProcess(15,13,0,0);
        break;

    case 18:
        runSortingProcess(15,16,0,0);
        break;

    case 19:
        runSortingProcess(12,16,0,0);
        break;

    default:
        break;
    }
}

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    int myRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    runProcess(myRank);

    MPI_Finalize();

    return 0;
}
