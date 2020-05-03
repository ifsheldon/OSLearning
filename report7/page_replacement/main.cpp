#include <iostream>
#include <memory>

using namespace std;
enum Algorithm
{
    FIFO, LRU, MIN, CLOCK, SECOND_CHANCE
};

int main()
{
    ios::sync_with_stdio(false);
    int cacheSize = 0;
    int algo = 0;
    int numOfPages = 0;
    cin >> cacheSize;
    cin >> algo;
    cin >> numOfPages;
    if (!(algo >= 0 && algo <= 4 && numOfPages > 0 && cacheSize > 0))
    {
        cout << "Invalid Input\n" << "algorithm index = " << algo << ", cache size = " << cacheSize
             << ", number of pages = " << numOfPages << endl;
        return -1;
    }
    Algorithm algorithm = Algorithm(algo);
//    unique_ptr<int[]> pageSequence(new int[numOfPages]);
    int *pageSequence = new int[numOfPages];
    for (int i = 0; i < numOfPages; i++)
        cin >> pageSequence[i];


    delete [] pageSequence;

    return 0;
}
