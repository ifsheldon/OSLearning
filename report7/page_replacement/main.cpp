#include <iostream>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <cmath>
#include <iomanip>

//#define DEBUG
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
    bool valid;
};

inline void lru(const int *pageSequence, int length, int cacheSize)
{
    Node *nodes = new Node[cacheSize];
    Node *head = nodes;
    Node *tail = nodes + cacheSize - 1;
    unordered_map<int, Node *> cachedPages;
    nodes[0].prev = nullptr;
    nodes[0].next = nodes + 1;
    nodes[cacheSize - 1].next = nullptr;
    nodes[cacheSize - 1].prev = nodes + cacheSize - 2;
    int usedSpace = 0;
    int missCount = 0;
    for (int i = 1; i < cacheSize - 1; i++)
    {
        nodes[i].prev = nodes + (i - 1);
        nodes[i].next = nodes + (i + 1);
    }
    for (int i = 0; i < length; i++)
    {
        int page = pageSequence[i];
        auto found = cachedPages.find(page);
        bool hit = found != cachedPages.end();
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
            if (usedSpace < cacheSize)
            {
                Node *node = nodes + usedSpace;
                node->val = page;
                cachedPages[page] = node;
                usedSpace++;
            } else
            {
                int tailPageNum = tail->val;
                cachedPages.erase(tailPageNum);
                tail->prev->next = nullptr;
                Node *node = tail;
                tail = tail->prev;
                node->val = page;
                node->prev = nullptr;
                node->next = head;
                head = node;
                cachedPages[page] = node;
            }
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
    unordered_map<int, Node *> cachedPages;
    Node *nodes = new Node[cacheSize];
    nodes[0].prev = nullptr;
    nodes[0].next = nodes + 1;
    nodes[0].valid = false;
    nodes[cacheSize - 1].next = nullptr;
    nodes[cacheSize - 1].prev = nodes + cacheSize - 2;
    nodes[cacheSize - 1].valid = false;
    int missCount = 0;
    for (int i = 1; i < cacheSize - 1; i++)
    {
        nodes[i].prev = nodes + (i - 1);
        nodes[i].next = nodes + (i + 1);
        nodes[i].valid = false;
    }
    Node *currentPointing = nodes;
    for (int i = 0; i < length; i++)
    {
        int page = pageSequence[i];
        auto found = cachedPages.find(page);
        if (found != cachedPages.end())//hit
        {
            found->second->valid = true;
        } else
        {
            missCount++;
            if (cachedPages.size() < cacheSize)
            {
                (nodes + cachedPages.size())->valid = true;
                cachedPages[page] = nodes + cachedPages.size();
            } else
            {
                for (; currentPointing->valid; currentPointing->valid = false, currentPointing = currentPointing->next);
                currentPointing->valid = true;
                int previousPage = currentPointing->val;
                cachedPages.erase(previousPage);
                cachedPages[page] = currentPointing;
            }
        }
    }
    printResult(length, missCount);
    delete[] nodes;
}

inline void second_chance(const int *pageSequence, int length, int cacheSize)
{
    if (cacheSize == 2)
    {
        int fifoCache = -1;
        int lruCache = -1;
        int missCount = 0;
        for (int i = 0; i < length; i++)
        {
            int page = pageSequence[i];
            if (page == fifoCache)
            {
                // do nothing
            } else if (page == lruCache)
            {
                int previous = fifoCache;
                fifoCache = lruCache;
                lruCache = previous;
            } else
            {
                missCount++;
                if (fifoCache != -1)
                    lruCache = fifoCache;
                fifoCache = page;
            }
        }
        printResult(length, missCount);
        return;
    }
    if (cacheSize == 3)
    {
        int fifoCache = -1;
        int *lruCache = new int[2]{-1, -1};
        bool zeroUsed = false;
        int missCount = 0;
        for (int i = 0; i < length; i++)
        {
            int page = pageSequence[i];
            if (page == fifoCache)
            {
                //do nothing
            } else if (page == lruCache[0])
            {
                zeroUsed = true;
                int tmp = fifoCache;
                fifoCache = lruCache[0];
                lruCache[0] = tmp;
            } else if (page == lruCache[1])
            {
                zeroUsed = false;
                int tmp = fifoCache;
                fifoCache = lruCache[1];
                lruCache[1] = tmp;
            } else
            {
                missCount++;
                if (fifoCache == -1)
                    fifoCache = page;
                else//fifo full
                {
                    int previous = fifoCache;
                    fifoCache = page;
                    //load to lru cache
                    if (zeroUsed)
                    {
                        lruCache[1] = previous;
                        zeroUsed = false;
                    } else
                    {
                        lruCache[0] = previous;
                        zeroUsed = true;
                    }
                }
            }
        }
        printResult(length, missCount);
        return;
    }
    int fifoSize = cacheSize / 2;
    int lruSize = cacheSize - fifoSize;
    Node *fifoQueue = new Node[fifoSize];
    int usedFifoSpace = 0;
    int fifoHead = 0;
    Node *lruQueue = new Node[lruSize];
    int usedLRUSpace = 0;
    unordered_map<int, pair<Node *, bool>> cachedPages;
    int missCount = 0;
    for (int i = 0; i < length; i++)
    {
        int page = pageSequence[i];
        auto found = cachedPages.find(page);
        if (found != cachedPages.end())
        {

        } else
        {
            missCount++;
            if (cachedPages.size() < cacheSize)
            {
                if (usedFifoSpace < fifoSize)
                {
                    fifoQueue[usedFifoSpace].val = page;
                    cachedPages[page] = make_pair(fifoQueue + usedFifoSpace, true);
                    usedFifoSpace++;
                } else //FIFO full
                {
                    //TODO: finish this
                    int previousPage = fifoQueue[fifoHead].val;
                    fifoQueue[fifoHead].val = page;
                    fifoHead++;
                    fifoHead %= fifoSize;

                }
            }
        }
    }
    delete[] fifoQueue;
    delete[] lruQueue;
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
    if (cacheSize == 1)
    {
        fifo(pageSequence, numOfPages, cacheSize);
        return 0;
    }
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
