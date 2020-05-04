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

class FIFOQueue
{
private:
    unordered_set<int> items;
    int *integers;
    int maxCapacity;
    int head;
    int tail;
public:
    explicit FIFOQueue(int size)
    {
        this->maxCapacity = size;
        this->integers = new int[size];
        this->head = 0;
        this->tail = 0;
    }

    ~FIFOQueue()
    {
        delete[] integers;
    }

    int append(int i)
    {
        if (items.size() < maxCapacity)
        {
            items.insert(i);
            integers[tail] = i;
            if (items.size() < maxCapacity)
                tail++;
            return INT_MIN;
        } else
        {
            int previous = integers[head];
            integers[head] = i;
            head++;
            head %= maxCapacity;
            items.erase(previous);
            items.insert(i);
            return previous;
        }
    }

    bool contains(int i)
    {
        return items.find(i) != items.end();
    }

    int size()
    {
        return items.size();
    }
};

struct Node
{
    int val;
    Node *prev;
    Node *next;
};

class LRUQueue
{
private:
    unordered_map<int, Node *> map;
    int capacity;
    Node *nodes;
    Node *head;
    Node *tail;

    inline void initLinkedList()
    {
        nodes[0].prev = nullptr;
        nodes[0].next = nodes + 1;
        nodes[capacity - 1].next = nullptr;
        nodes[capacity - 1].prev = nodes + (capacity - 2);
        for (int i = 1; i < capacity - 1; i++)
        {
            nodes[i].prev = nodes + (i - 1);
            nodes[i].next = nodes + (i + 1);
        }
    }

public:
    explicit LRUQueue(int size)
    {
        capacity = size;
        nodes = new Node[size];
        head = nodes;
        tail = nodes;
        initLinkedList();
    }

    int addNode(int i)
    {
        if (map.size() < capacity)
        {
            tail->val = i;
            map[i] = tail;
            if (map.size() < capacity) tail++;
            return INT_MIN;
        } else
        {
            int previous = head->val;
            map.erase(previous);
            Node *newNode = head;
            map[i] = newNode;
            head = head->next;
            newNode->prev = tail;
            newNode->next = nullptr;
            tail->next = newNode;
            tail = newNode;
            return previous;
        }
    }

    void hit(int i)
    {
        auto found = map.find(i);
        Node *node = found->second;
        if (node != tail)
        {
            if (node == head)
            {
                head = head->next;
                head->prev = nullptr;
            } else
            {
                node->prev->next = node->next;
                node->next->prev = node->prev;
            }
            tail->next = node;
            node->prev = tail;
            node->next = nullptr;
            tail = node;
        }
    }

    bool replace(int origin, int newOne)
    {
        if (contains(origin))
        {
            auto found = map.find(origin);
            Node *node = found->second;
            node->val = newOne;
            map.erase(origin);
            map[newOne] = node;
            if (node != tail)
            {
                if (node == head)
                {
                    head = head->next;
                    head->prev = nullptr;
                } else
                {
                    node->prev->next = node->next;
                    node->next->prev = node->prev;
                }
                node->prev = tail;
                node->next = nullptr;
                tail->next = node;
                tail = node;
            }
            return true;
        } else
        {
            return false;
        }
    }

    bool contains(int i)
    {
        return map.find(i) != map.end();
    }

    int size()
    {
        return map.size();
    }

    ~LRUQueue()
    {
        delete[] nodes;
    }
};

#ifdef DEBUG
float result = 0.0f;
#endif

//TODO: fix the output format. Sample1Output: 05.56%, my output 5.56%
inline void printResult(int pageNum, int cacheMiss)
{
    int hit = pageNum - cacheMiss;
    float hitRatio = (float) (hit * 100) / (float) pageNum;
    hit = ceil(hitRatio * 100.0f);
    hitRatio = (float) hit / 100.0f;
#ifdef DEBUG
    result = hitRatio;
#endif
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
    FIFOQueue fifoQueue(cacheSize);
    int missCount = 0;
    for (int i = 0; i < length; i++)
    {
        int page = pageSequence[i];
        if (!fifoQueue.contains(page))
        {
            fifoQueue.append(page);
            missCount++;
        }
    }
    printResult(length, missCount);
}


inline void lru(const int *pageSequence, int length, int cacheSize)
{
    LRUQueue lruQueue(cacheSize);
    int missCount = 0;
    for (int i = 0; i < length; i++)
    {
        int page = pageSequence[i];
        if (lruQueue.contains(page))
        {
            lruQueue.hit(page);
        } else
        {
            missCount++;
            lruQueue.addNode(page);
        }
    }
    printResult(length, missCount);
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
    unordered_map<int, int> cachedPages;
    int *pages = new int[cacheSize];
    bool *validBits = new bool[cacheSize];
    int clockArm = 0;
    int missCount = 0;
    for (int i = 0; i < length; i++)
    {
        int page = pageSequence[i];
        auto found = cachedPages.find(page);
        if (found != cachedPages.end())//hit
        {
            validBits[found->second] = true;
        } else
        {
            missCount++;
            if (cachedPages.size() < cacheSize)
            {
                validBits[cachedPages.size()] = true;
                cachedPages[page] = cachedPages.size();
            } else
            {
                for (; validBits[clockArm]; validBits[clockArm] = false, clockArm++, clockArm %= cacheSize);
                validBits[clockArm] = true;
                int previousPage = pages[clockArm];
                cachedPages.erase(previousPage);
                cachedPages[page] = clockArm;
            }
        }
    }
    printResult(length, missCount);
    delete[] pages;
    delete[] validBits;
}

//inline void clock(const int *pageSequence, int length, int cacheSize)
//{
//    unordered_map<int, Node *> cachedPages;
//    Node *nodes = new Node[cacheSize];
//    nodes[0].prev = nullptr;
//    nodes[0].next = nodes + 1;
//    nodes[0].valid = false;
//    nodes[cacheSize - 1].next = nullptr;
//    nodes[cacheSize - 1].prev = nodes + cacheSize - 2;
//    nodes[cacheSize - 1].valid = false;
//    int missCount = 0;
//    for (int i = 1; i < cacheSize - 1; i++)
//    {
//        nodes[i].prev = nodes + (i - 1);
//        nodes[i].next = nodes + (i + 1);
//        nodes[i].valid = false;
//    }
//    Node *currentPointing = nodes;
//    for (int i = 0; i < length; i++)
//    {
//        int page = pageSequence[i];
//        auto found = cachedPages.find(page);
//        if (found != cachedPages.end())//hit
//        {
//            found->second->valid = true;
//        } else
//        {
//            missCount++;
//            if (cachedPages.size() < cacheSize)
//            {
//                (nodes + cachedPages.size())->valid = true;
//                cachedPages[page] = nodes + cachedPages.size();
//            } else
//            {
//                for (; currentPointing->valid; currentPointing->valid = false, currentPointing = currentPointing->next);
//                currentPointing->valid = true;
//                int previousPage = currentPointing->val;
//                cachedPages.erase(previousPage);
//                cachedPages[page] = currentPointing;
//            }
//        }
//    }
//    printResult(length, missCount);
//    delete[] nodes;
//}

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
    //cache size >= 4
    int fifoSize = cacheSize / 2;
    int lruSize = cacheSize - fifoSize;
    LRUQueue lruQueue(lruSize);
    FIFOQueue fifoQueue(fifoSize);
    int missCount = 0;
    for (int i = 0; i < length; i++)
    {
        int page = pageSequence[i];
        if (lruQueue.contains(page) || fifoQueue.contains(page)) // if cached
        {
            if (lruQueue.contains(page))
            {
                //move to FIFO
                int poppedPageInFIFO = fifoQueue.append(page);
                lruQueue.replace(page, poppedPageInFIFO);
            }
        } else
        {
            missCount++;
            if (fifoQueue.size() + lruQueue.size() < cacheSize)
            {
                if (fifoQueue.size() < fifoSize)
                {
                    fifoQueue.append(page);
                } else //FIFO full
                {
                    int poppedPageInFIFO = fifoQueue.append(page);
                    lruQueue.addNode(poppedPageInFIFO);
                }
            } else //cache full
            {
                int poppedPageInFIFO = fifoQueue.append(page);
                lruQueue.addNode(poppedPageInFIFO);
            }
        }
    }
    printResult(length, missCount);
}


#ifdef DEBUG

#include <random>

const int LENGTH = 10000;

inline void printSample(int *integers, int length, int cacheSize, int method, float ratio)
{
    cout << cacheSize << endl;
    cout << method << endl;
    cout << length << endl;
    for (int i = 0; i < length; i++)
        cout << integers[i] << " ";
    cout << endl;
    cout << "Result = " << ratio << endl;
    cout << endl;
}

void another_main()
{
    int integers[LENGTH];
    normal_distribution<float> normal(1000.0, 5.0);
    for (int i = 1; i <= 1; i++)
    {
        default_random_engine e(i);
        for (int &integer : integers)
        {
            integer += ceil(normal(e));
            integer %= 1000;
            integer++;
        }
        fifo(integers, LENGTH, 4);
        printSample(integers, LENGTH, 4, FIFO, result);
        lru(integers, LENGTH, 4);
        printSample(integers, LENGTH, 4, LRU, result);
        clock(integers, LENGTH, 4);
        printSample(integers, LENGTH, 4, CLOCK, result);
        second_chance(integers, LENGTH, 4);
        printSample(integers, LENGTH, 4, SECOND_CHANCE, result);
    }
}

#endif

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
