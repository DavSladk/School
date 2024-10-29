/*
    Author: Bc. David Sladký
    Description: Implementation preorder for binary tree
*/

#include "pro.h"

/*
    i - number to evaulate

    description: Evaluates if number is even.
*/
bool isEven(int i)
{
    if(i%2==1)
    {
        return false;
    }
    else
    {
        return true;
    }
}

/*
    edgeList - vector to fill up with oriented edges
    input - input graf to create oriented edges from

    description: Fills up edge list with oriented edges
*/
void fillEdgeList(std::vector<edge> &edgeList, const std::string &input)
{
    edge toPush;
    int parentIndex;
    int leftChildIndex;
    int rightChildIndex;
    size_t s = input.size();

    toPush.endState = -1;
    toPush.startState = -1;
    edgeList.push_back(toPush);

    for (int i = 1; i < s; i++)
    {
        // Set indexes
        if(isEven(i))
        {
            parentIndex = i/2;
        }
        else
        {
            parentIndex = (i-1)/2;
        }
        leftChildIndex = 2*i;
        rightChildIndex = 2*i+1;

        // Add edge to parent
        if(i!=1)
        {
            toPush.startState = input[i];
            toPush.endState = input[parentIndex];
            edgeList.push_back(toPush);
        }

        // Add edge to left child
        if(leftChildIndex < s)
        {
            toPush.startState = input[i];
            toPush.endState = input[leftChildIndex];
            edgeList.push_back(toPush);
        }

        // Add edge to right child
        if(rightChildIndex < s)
        {
            toPush.startState = input[i];
            toPush.endState = input[rightChildIndex];
            edgeList.push_back(toPush);
        }
    }
    
    return;
}

/*
    edgeList - edge list for print

    description: Prints out content of edge list
*/
void printEdgeList(std::vector<edge> edgeList)
{
    for(int i = 1; i < edgeList.size(); i++)
    {
        std::cout << i << " : " << edgeList[i].startState << " - " << edgeList[i].endState << std::endl;
    }
    std::cout << "-----------------" << std::endl;
    return;
}

/*
    edgeList - edge list to search in
    s - state to look for
    outIndex - int to safe index of edge

    description: Looks for index of edge starting with specified
                 state in edge list. Index of this edge is returned
                 in outIndex.
*/
bool findEdgeWithStartState(std::vector<edge> edgeList, char s, int &outIndex)
{
    for (int i = outIndex+1; i < edgeList.size(); i++)
    {
        if(edgeList[i].startState == s)
        {
            outIndex = i;
            return true;
        }
    }

    return false;
}

/*
    edgeList - edge list to search in
    s - start state to look for
    e - end state to look for
    outIndex - int to safe index of edge

    description: Looks for index of edge starting with specified
                 start/end state in edge list. Index of this
                 edge is returned in outIndex.
*/
int findEdgeWithStates(std::vector<edge> edgeList, char s, char e, int &outIndex)
{
    for (int i = outIndex; i < edgeList.size(); i++)
    {
        if(edgeList[i].startState == s && edgeList[i].endState == e)
        {
            outIndex = i;
            return true;
        }
    }

    return false;
}

/*
    adjList - pointer where to create adjency list
    s - size of that list
    edgeList - edge list to creat adjList from.
    input - input state

    description: Creates adjency list
*/
void createAdjList(adjElement **adjList, size_t s, std::vector<edge> edgeList, std::string input)
{
    int outEdgeIndex = -1;
    int revEdgeIndex = 0;
    adjElement* newElem;
    adjElement** tmp;
    
    // for every state
    for (size_t i = 1; i < s; i++)
    {
        tmp = &(adjList[i]);

        // Find outgoing edge
        while ( findEdgeWithStartState(edgeList, input[i], outEdgeIndex) )
        {
            revEdgeIndex = 0;
            newElem = new adjElement;
            newElem->next = nullptr;
            newElem->edge = outEdgeIndex;

            // To outgoing edge find reverse edge
            if ( findEdgeWithStates(edgeList, edgeList[outEdgeIndex].endState, edgeList[outEdgeIndex].startState, revEdgeIndex) )
            {
                newElem->reverse = revEdgeIndex;
            }

            *tmp = newElem;
            tmp = &(newElem->next);
        }
    }

    return;
}

/*
    adjList - adjency list to delete
    s - size of that list

    description: For every new needs to be a delete.
*/
void deleteAdjList(adjElement **adjList, size_t s)
{
    adjElement *toDelete;
    adjElement *next;

    for (int i = 1; i < s; i++)
    {
        toDelete = adjList[i];
        next = adjList[i]->next;
        delete toDelete;
        while (next != nullptr)
        {
            toDelete = next;
            next = next->next;
            delete toDelete;
        }
    }

    return;
}

/*
    array - array to print
    s - size of that array

    description: Prints out array of ints and their indexes.
*/
void printIntArray(int array[], int size)
{
    for (size_t i = 1; i < size; i++)
    {
        std::cout << i << " : " << array[i] << std::endl;
    }
    std::cout << "-----------------" << std::endl;
}

/*
    adjList - adjency list to print out
    s - size of that list
    input - input state

    description: Prints adjcency list.
*/
void printAdjList(adjElement **adjList, size_t s, std::string input)
{
    adjElement *tmp;
    for (size_t i = 1; i < s; i++)
    {
        std::cout << input[i] << " : " << adjList[i]->edge << "," << adjList[i]->reverse;
        tmp = adjList[i]->next;
        while (tmp != nullptr)
        {
            std::cout << " -> " << tmp->edge << "," << tmp->reverse;
            tmp = tmp->next;
        }
        std::cout << std::endl;
    }
    std::cout << "-----------------" << std::endl;
}

/*
    edge - edge to look for
    adjList - adjency list to look in
    s - size of adjList

    description: Looks for a edge in adjency list and returns it.
*/
adjElement* findAdjElem(int edge, adjElement *adjList[], size_t s)
{
    adjElement* tmp;

    for (size_t i = 1; i < s; i++)
    {
        tmp = adjList[i];
        while (tmp != nullptr)
        {
            if (tmp->edge == edge)
            {
                return tmp;
            }
            tmp = tmp->next;
        }
    }

    return nullptr;
}

/*
    edgeList - edge list to calculate Euler tour from
    adjList - adjency list to calculate Euler tour from
    s - size of that list
    input - input state

    description: Calculates Euler Tour.
*/
void calculateEtour(const std::vector<edge> &edgeList, adjElement *adjList[], size_t s, std::string input)
{
    int reverseIndex = 0;
    adjElement* t = nullptr;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Blinding the end of eTour
    int tmp1 = input.find(edgeList[rank].startState, 0);
    int tmp2 = input.find(edgeList[rank].endState, 0);
    if (tmp1 == 3 && tmp2 == 1)
    {
        MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        return;
    }

    // Find reverse edge index
    findEdgeWithStates(edgeList, edgeList[rank].endState, edgeList[rank].startState, reverseIndex);
    // Find reverse edge in adjList
    t = findAdjElem(reverseIndex, adjList, s);

    if(t->next != nullptr)
    {
        MPI_Send(&(t->next->edge), 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    else
    {
        char endState = edgeList[rank].endState;
        int endStateIndex = input.find(endState, 0);
        int toSend = adjList[endStateIndex]->edge;
        MPI_Send(&toSend, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    return;
}

/*
    edgeList - edge list
    input - input state

    description: Calculates if edge is forward or not.
*/
void calculateWeight(const std::vector<edge> &edgeList, std::string input)
{
    int weight;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    char startState = edgeList[rank].startState;
    char endState = edgeList[rank].endState;
    
    int startIndex = input.find(startState,0);
    int endIndex = input.find(endState,0);
    
    if(startIndex < endIndex)
    {
        weight = 1;
    }
    else
    {
        weight = 0;
    }

    MPI_Send(&weight, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
}

/*
    eTour - Euler tour
    weights - weights of edges
    size - size of above arrays

    description: Calculates suffix sum.
*/
void suffixSum(int eTour[], int weights[], int size)
{
    const int NEUTRAL = 0;
    int val[size];
    memcpy(val, weights, size * sizeof(int) );
    int newTour[size];
    memcpy(newTour, eTour, size * sizeof(int) );

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int indexLast;

    // Setting last element as neutral
    for (int i = 1; i < size; i++)
    {
        if(i == eTour[i])
        {
            val[i] = NEUTRAL;
            indexLast = i;
            break;
        }
    }

    int succ;
    for (int i = 1; i < size ; i++)
    {
        succ = newTour[rank];
        val[rank] += val[succ];
        newTour[rank] = newTour[succ];

        // Send new value
        MPI_Send(&val[rank], 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        // Send new tour
        MPI_Send(&newTour[rank], 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        // Get new values
        MPI_Bcast(val, size, MPI_INT, 0, MPI_COMM_WORLD);
        // Get new tour
        MPI_Bcast(newTour, size, MPI_INT, 0, MPI_COMM_WORLD);
    }
    
    if(weights[indexLast] != NEUTRAL)
    {
        val[rank] += weights[indexLast];
    }

    // Send new value
    MPI_Send(&val[rank], 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

    return;
}

/*
    edgeList - edge list
    weights - weights of edges
    suffix - sum of suffixes
    inputSize - number of input states

    description: Calculates order of edges for preorder.
*/
void calculatePreorder(const std::vector<edge> &edgeList, int weights[], int suffix[], int inputSize)
{
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int toReturn = -1;

    if(weights[rank] == 1)
    {
        toReturn = inputSize - suffix[rank];
    }

    MPI_Send(&toReturn, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    return;
}

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    // Make some set up.
    std::string input = "0" + std::string(argv[1]);
    std::vector<edge> edgeList;

    adjElement *adjList[input.size()];
    memset(adjList, 0, sizeof(adjElement*)*input.size());

    int myRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    fillEdgeList(edgeList, input);
    createAdjList(adjList, input.size(), edgeList, input);

    int eTour[edgeList.size()];
    int newTour[edgeList.size()];
    int weights[edgeList.size()];
    int suffix[edgeList.size()];
    int order[input.size()];
    int orderMsg;

    // Controller process
    if(myRank == 0)
    {
        DEBUG_IO(printEdgeList(edgeList))
        DEBUG_IO(printAdjList(adjList,input.size(),input))

        // Colect result of eTour
        for (size_t i = 1; i < edgeList.size(); i++)
        {
            MPI_Recv(&eTour[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
        }

        DEBUG_IO(printIntArray(eTour,edgeList.size()))

        // Broadcast results of eTour
        MPI_Bcast(eTour, edgeList.size(), MPI_INT, 0, MPI_COMM_WORLD);

        // Colect results of weight
        for (size_t i = 1; i < edgeList.size(); i++)
        {
            MPI_Recv(&weights[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
        }

        DEBUG_IO(printIntArray(weights,edgeList.size()))

        // Broadcast results of weight
        MPI_Bcast(&weights, edgeList.size(), MPI_INT, 0, MPI_COMM_WORLD);

        // Refrech suffix sum for each proces
        for (int i = 1; i < edgeList.size(); i++)
        {
            // collect new values
            for (size_t i = 1; i < edgeList.size(); i++)
            {
                MPI_Recv(&suffix[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
                MPI_Recv(&newTour[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
            }
            // Broadcast new values
            MPI_Bcast(&suffix, edgeList.size(), MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Bcast(&newTour, edgeList.size(), MPI_INT, 0, MPI_COMM_WORLD);
        }
        // last refresh of suffixes
        for (size_t i = 1; i < edgeList.size(); i++)
        {
            MPI_Recv(&suffix[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
        }
        MPI_Bcast(&suffix, edgeList.size(), MPI_INT, 0, MPI_COMM_WORLD);

        DEBUG_IO(printIntArray(suffix,edgeList.size()))

        // Collect order of states
        for (size_t i = 1; i < edgeList.size(); i++)
        {
            MPI_Recv(&orderMsg, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
            if(orderMsg!=-1)
            {
                order[orderMsg] = input.find(edgeList[i].endState,0);
            }
        }
        order[1]=1;

        DEBUG_IO(printIntArray(order,edgeList.size()))

        // Print resulting order of states
        for (int i = 1; i < input.size(); i++)
        {
            std::cout << input[order[i]];
        }
        std::cout << std::endl;
    }
    // Work processes
    else
    {
        calculateEtour(edgeList, adjList, input.size(), input);
        // Get results of eTour from other procesors
        MPI_Bcast(eTour, edgeList.size(), MPI_INT, 0, MPI_COMM_WORLD);
        calculateWeight(edgeList, input);
        // Get results of Weight from other procesors
        MPI_Bcast(weights, edgeList.size(), MPI_INT, 0, MPI_COMM_WORLD);

        suffixSum(eTour,weights,edgeList.size());
        // Get final values of suffixes
        MPI_Bcast(&suffix, edgeList.size(), MPI_INT, 0, MPI_COMM_WORLD);
        
        calculatePreorder(edgeList,weights,suffix,input.size());
    }

    deleteAdjList(adjList, input.size());    
    MPI_Finalize();
    return 0;
}
