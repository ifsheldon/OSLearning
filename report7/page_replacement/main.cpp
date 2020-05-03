#include <iostream>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <cmath>
#include <iomanip>

#define DEBUG
using namespace std;
enum Algorithm
{
    FIFO, LRU, MIN, CLOCK, SECOND_CHANCE
};

inline void printResult(int pageNum, int cacheMiss)
{
    int hit = pageNum - cacheMiss;
    float hitRatio = (float) (hit * 100) / (float) pageNum;
    hit = ceil(hitRatio * 100.0f);
    hitRatio = (float) hit / 100.0f;
    cout << "Hit ratio = " << setprecision(4) << hitRatio << "%" << endl;
}

inline void fifo(const int *pageSequence, int length, int cacheSize)
{
    if (cacheSize == 1)
    {
        int cachedPage = -1;
        int miss = 0;
        for (int i = 0; i < length; i++)
        {
            if (cachedPage != pageSequence[i])
            {
                miss++;
                cachedPage = pageSequence[i];
            }
        }
        printResult(length, miss);
        return;
    }
    unordered_set<int> cachedPages;
    int *cache = new int[cacheSize]{-1};
    int head = 0;
    int missCount = 0;
    for (int i = 0; i < length; i++)
    {
        int page = pageSequence[i];
        if (cachedPages.find(page) == cachedPages.end())// cache miss
        {
            int previous = cache[head];
            cachedPages.erase(previous);
            cache[head] = page;
            head++;
            head %= cacheSize;
            missCount++;
        }
    }
    printResult(length, missCount);
    delete[] cache;
}

struct Node
{
    int val;
    Node *prev;
    Node *next;
};

inline void lru(const int *pageSequence, int length, int cacheSize)
{
    if (cacheSize == 1)
    {
        fifo(pageSequence, length, cacheSize);
        return;
    }
    Node *nodes = new Node[cacheSize];
    Node *head = nodes;
    Node *tail = nodes + cacheSize - 1;
    unordered_map<int, Node *> map;
    nodes[0].prev = nullptr;
    nodes[cacheSize - 1].next = nullptr;
    int usedSpace = 0;
    int missCount = 0;
    for (int i = 1; i < cacheSize - 1; i++)
    {
        nodes[i].prev = nodes + (i - 1);
        nodes[i].next = nodes + (i + 1);
    }
    //TODO: need to add code for unfilled list
    for (int i = 0; i < length; i++)
    {
        int page = pageSequence[i];
        auto found = map.find(page);
        bool hit = found != map.end();
        if (hit)
        {
            Node *node = found->second;
            if (node != head)
            {
                node->prev->next = node->next;
                if (node->next != nullptr)
                    node->next->prev = node->prev;
                node->prev = nullptr;
                node->next = head;
                head = node;
            }
        } else
        {
            missCount++;
            int tailPageNum = tail->val;
            map.erase(tailPageNum);
            tail->prev->next = nullptr;
            Node *node = tail;
            tail = tail->prev;
            node->val = page;
            node->prev = nullptr;
            node->next = head;
            head = node;
            map[page] = node;
        }
    }
    printResult(length, missCount);
    delete[] nodes;
}

inline void min(const int *pageSequence, int length, int cacheSize)
{
    int *nextOccurIndices = new int[length];
    unordered_map<int, int> prevIndices;
    for (int idx = 0; idx < length; idx++)
    {
        int page = pageSequence[idx];
        auto found = prevIndices.find(page);
        if (found != prevIndices.end())
        {
            int prevOccurIndex = found->second;
            nextOccurIndices[prevOccurIndex] = idx;
        } else
        {
            nextOccurIndices[idx] = idx;
        }
        prevIndices[page] = idx;
    }
    unordered_map<int, int> cachedPages;
    int missCount = 0;
    for (int i = 0; i < length; i++)
    {
        int page = pageSequence[i];
        auto found = cachedPages.find(page);
        if (found != cachedPages.end()) //cached
            cachedPages[page] = nextOccurIndices[i];
        else// not cached
        {
            missCount++;
            if (cachedPages.size() < cacheSize) // if not full
                cachedPages[page] = nextOccurIndices[i];
            else
            {
                int maxFuture = -1;
                int pageOfMaxFuture = -1;
                for (auto &p: cachedPages)
                {
                    if (p.second > maxFuture)
                    {
                        maxFuture = p.second;
                        pageOfMaxFuture = p.first;
                    }
                }
                cachedPages.erase(pageOfMaxFuture);
                cachedPages[page] = nextOccurIndices[i];
            }
        }
    }
    printResult(length, missCount);
    delete[] nextOccurIndices;
}

inline void clock(const int *pageSequence, int length, int cacheSize)
{

}

inline void second_chance(const int *pageSequence, int length, int cacheSize)
{

}

void another_main()
{
}

int main()
{
    ios::sync_with_stdio(false);
#ifdef DEBUG
    another_main();
    return 0;
#endif
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
    switch (algorithm)
    {
        case FIFO:
            fifo(pageSequence, numOfPages, cacheSize);
            break;
        case LRU:
            lru(pageSequence, numOfPages, cacheSize);
            break;
        case MIN:
            min(pageSequence, numOfPages, cacheSize);
            break;
        case CLOCK:
            clock(pageSequence, numOfPages, cacheSize);
            break;
        case SECOND_CHANCE:
            second_chance(pageSequence, numOfPages, cacheSize);
            break;
        default:
            cout << "Error" << endl;
            return -1;
    }

    delete[] pageSequence;

    return 0;
}
