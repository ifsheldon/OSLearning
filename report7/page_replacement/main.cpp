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

public:
    explicit LRUQueue(int size)
    {
        capacity = size;
        nodes = new Node[size];
        for (int i = 0; i < size; i++)
        {
            nodes[i].prev = nullptr;
            nodes[i].next = nullptr;
        }
        head = nullptr;
        tail = nullptr;
    }

    int addNode(int i)
    {
        if (map.size() < capacity)
        {
            Node *newNode = nodes + map.size();
            newNode->next = nullptr;
            newNode->val = i;
            if (head == nullptr && tail == nullptr)
            {
                head = newNode;
                tail = newNode;
            }
//            else if (head == nullptr && tail == nullptr)
//            {
//                cout << "THIS SHOULD NOT HAPPEN!------------------------" << endl;
//                return INT_MAX;
//            } else if (head != nullptr && tail == nullptr)
//            {
//                cout << "THIS SHOULD NOT HAPPEN!########################" << endl;
//                return INT_MAX;
//            }
            else
            {
                tail->next = newNode;
                newNode->prev = tail;
                newNode->next = nullptr;
                tail = newNode;
            }
            map[i] = newNode;
            return INT_MIN;
        } else
        {
            int previous = head->val;
            map.erase(previous);
            Node *newNode = head;
            head = head->next;
            newNode->val = i;
            newNode->prev = tail;
            newNode->next = nullptr;
            map[i] = newNode;
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

#ifdef DEBUG

    bool error()
    {
        Node *current = head;
        while (current->next != nullptr)
        {
            current = current->next;
        }
        return current != tail;
    }

#endif

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


//TODO:Sample1Output: 05.56%, my output 5.56%; format fixed, check round up
inline void printResult(int pageNum, int cacheMiss)
{
    int hit = pageNum - cacheMiss;
    float hitRatio = (float) (hit * 100) / (float) pageNum;
    hit = ceil(hitRatio * 100.0f);
    hitRatio = (float) hit / 100.0f;
    cout << "Hit ratio = " << std::fixed << setfill('0') << setw(5) << setprecision(2) << hitRatio << "%" << endl;
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
#ifdef DEBUG
        bool error = lruQueue.error();
        int j = 1;
#endif
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
        }
        nextOccurIndices[idx] = length;
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
                pages[cachedPages.size()] = page;
                cachedPages[page] = cachedPages.size();
            } else
            {
                for (; validBits[clockArm]; validBits[clockArm] = false, clockArm++, clockArm %= cacheSize);
                validBits[clockArm] = true;
                int previousPage = pages[clockArm];
                cachedPages.erase(previousPage);
                cachedPages[page] = clockArm;
                pages[clockArm] = page;
                clockArm++;
                clockArm %= cacheSize;
            }
        }
    }
    printResult(length, missCount);
    delete[] pages;
    delete[] validBits;
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

inline void printSample(int *integers, int length, int cacheSize, int method)
{
    cout << endl;
    cout << cacheSize << endl;
    cout << method << endl;
    cout << length << endl;
    for (int i = 0; i < length; i++)
        cout << integers[i] << " ";
    cout << endl;
}

void random_sample()
{
    int integers[LENGTH];
    normal_distribution<float> normal(1000.0, 5.0);
    for (int i = 1; i <= 30; i++)
    {
        default_random_engine e(i);
        for (int &integer : integers)
        {
            integer += ceil(normal(e));
            integer %= 1000;
            integer++;
        }
        printSample(integers, LENGTH, i, FIFO);
//        cout << "------------------ " << i << endl;
//        cout << "fifo" << endl;
        fifo(integers, LENGTH, i);
        printSample(integers, LENGTH, i, LRU);
//        cout << "lru" << endl;
        if (i == 1)
            fifo(integers, LENGTH, i);
        else
            lru(integers, LENGTH, i);
        printSample(integers, LENGTH, i, MIN);
//        cout << "min" << endl;
        if (i == 1)
            fifo(integers, LENGTH, i);
        else
            min(integers, LENGTH, i);
        printSample(integers, LENGTH, i, CLOCK);
//        cout << "clock" << endl;
        if (i == 1)
            fifo(integers, LENGTH, i);
        else
            clock(integers, LENGTH, i);
        printSample(integers, LENGTH, i, SECOND_CHANCE);
//        cout << "second_chance" << endl;
        if (i == 1)
            fifo(integers, LENGTH, i);
        else
            second_chance(integers, LENGTH, i);
    }
}

#endif

int main()
{
    ios::sync_with_stdio(false);
#ifdef DEBUG
    random_sample();
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
