#include <bits/stdc++.h>
#include <unistd.h>

using namespace std;

//#define PROCESS_NAME_LEN 32 //进程名最大长度
//#define MIN_SLICE 10 //内碎片最大大小
#define DEFAULT_MEM_SIZE 1024  //总内存大小
#define DEFAULT_MEM_START 0  //内存开始分配时的起始地址

//typedef pair<int, string> My_algo;

int mem_size = DEFAULT_MEM_SIZE;
int totalFreeSpace = mem_size;
//bool allowResizeMem = true; //当内存已经被分配了之后，不允许更改总内存大小的flag
static int pid = 0;
//My_algo algo[123];

struct free_block
{    //空闲数据块
    int size;
    int start_addr;
    struct free_block *next;
};

struct allocated_block
{ //已分配的数据块
    int pid;
    int size;
    int start_addr;
    int *data;
    struct allocated_block *next;
};

free_block *free_block_head; //空闲数据块首指针
allocated_block *allocated_block_head = nullptr; //分配块首指针

allocated_block *find_process(int id); //寻找pid为id的分配块
free_block *init_free_block(int mem_size); //空闲块初始化
inline void display_menu(); //显示选项菜单
void set_mem_size(); //设置内存大小
int allocate_mem(allocated_block *ab); //为制定块分配内存
void rearrange(); // 对块进行重新分配
int create_new_process(); //创建新的进程
int free_mem(allocated_block *ab); //释放分配块
void swap(int *p, int *q); //交换地址
int dispose(allocated_block *ab); //释放分配块结构体
void display_mem_usage(); //显示内存情况
void kill_process(); //杀死对应进程并释放其空间与结构体
//void Usemy_algo(int id); //使用对应的分配算法

#define BEST_FIT 1
#define WORST_FIT 2
#define FIRST_FIT 3
#define BUDDY 4

int selectedAlgo = BEST_FIT;

#define RESIZE_MEM 1
#define SET_ALGO 2
#define CREATE_PROCESS 3
#define KILL_PROCESS 4
#define DISPLAY_MEM_USAGE 5
#define EXIT 233

//#define DEBUG

//主函数
int main()
{
    int op;
    pid = 0;
    free_block_head = init_free_block(mem_size); //初始化一个可以使用的内存块，类似与操作系统可用的总存储空间

    while (true)
    {
        sleep(1);
        display_menu();
        fflush(stdin);
        scanf("%d", &op);
        switch (op)
        {
            case RESIZE_MEM:
            {
                set_mem_size();
                break;
            }
            case SET_ALGO:
            {
                printf("Choose an algorithm(now = %d)\n", selectedAlgo);
                printf(" 1: Best Fit\n 2: Worst Fit\n 3: First Fit\n 4: Buddy System\n");
                int result = scanf("%d", &selectedAlgo);
                if (result != 1)
                    printf("Input Error, Try again\n");
                if (selectedAlgo < 1 || selectedAlgo > 4)
                    printf("Invaild input, Try again\n");
                break;
            }
            case CREATE_PROCESS:
            {
                create_new_process();
                break;
            }
            case KILL_PROCESS:
            {
                kill_process();
                break;
            }
            case DISPLAY_MEM_USAGE:
            {
                display_mem_usage();
                break;
            }
            case EXIT:
            {
                puts("bye....");
                sleep(1);
                return 0;
            }
            default:
                break;
        }
    }
}

inline allocated_block *find_allocated_tail()
{
    auto current = allocated_block_head;
    allocated_block *pre = nullptr;
    while (current != nullptr)
    {
        pre = current;
        current = current->next;
    }
    return pre;
}

allocated_block *find_process(int id)
{ //循环遍历分配块链表，寻找pid=id的进程所对应的块
    for (auto current = allocated_block_head; current != nullptr; current = current->next)
    {
        if (current->pid == id)
            return current;
    }
    return nullptr;
}

free_block *init_free_block(int mem_size)
{ //初始化空闲块，这里的mem_size表示允许的最大虚拟内存大小
    free_block *p;
    p = (free_block *) malloc(sizeof(free_block));
    if (p == nullptr)
    {
        puts("No memory left");
        return nullptr;
    }
    p->size = mem_size;
    p->start_addr = DEFAULT_MEM_START;
    p->next = nullptr;
    return p;
}

inline void display_menu()
{
    puts("\n\n******************menu*******************");
    printf("1) Set memory size (default = %d, now = %d)\n", DEFAULT_MEM_SIZE, mem_size);
    printf("2) Set memory allocation algorithm\n");
    printf("3) Create a new process\n");
    printf("4) Kill a process\n");
    printf("5) Display memory usage\n");
    printf("233) Exit\n");
}

void set_mem_size()
{ //更改最大内存大小
#ifdef DEBUG
    printf("total free: %d\n", totalFreeSpace);
#endif
    if (totalFreeSpace == mem_size)
    {
        while (true)
        {
            printf("Enter new mem size(positive integer)\n");
            int memNewSize;
            while (scanf("%d", &memNewSize) != 1)
                printf("Input Error, Try again");
            if (memNewSize <= 0)
                printf("Enter a positive integer plz, try again\n");
            else
            {
                //dump all free blocks
                auto node = free_block_head->next;
                while (node != nullptr)
                {
                    auto self = node;
                    node = node->next;
                    free(self);
                }
                //reset head
                free_block_head->size = memNewSize;
                free_block_head->next = nullptr;
                mem_size = memNewSize;
                totalFreeSpace = mem_size;
                break;
            }
        }
    } else
    {
        printf("Mem allocated, not allow resizing\n");
    }
}


inline free_block *bestFit(const allocated_block *ab)
{
    int absize = ab->size;
    free_block *bestFitBlock = nullptr;
    int minDiff = INT_MAX;
    for (auto current = free_block_head; current != nullptr; current = current->next)
    {
        if (current->size == absize)
        {
            bestFitBlock = current;
            break;
        } else if (current->size > absize)
        {
            int diff = current->size - absize;
            if (diff < minDiff)
            {
                minDiff = diff;
                bestFitBlock = current;
            }
        }
    }
    return bestFitBlock;
}

inline free_block *worstFit(const allocated_block *ab)
{
    int absize = ab->size;
    free_block *worstFitBlock = nullptr;
    int maxDiff = -1;
    for (auto current = free_block_head; current != nullptr; current = current->next)
    {
        if (current->size >= absize)
        {
            int diff = current->size - absize;
            if (diff > maxDiff)
            {
                maxDiff = diff;
                worstFitBlock = current;
            }
        }
    }
    return worstFitBlock;
}

inline free_block *firstFit(const allocated_block *ab)
{
    int absize = ab->size;
    free_block *firstFitBlock = nullptr;
    for (auto current = free_block_head; current != nullptr; current = current->next)
    {
        if (current->size >= absize)
        {
            firstFitBlock = current;
            break;
        }
    }
    return firstFitBlock;
}

inline free_block *buddy(const allocated_block *ab)
{
    printf("NOT IMPLEMENTED");
    return nullptr;
}

inline free_block *findFeasibleFreeBlock(const allocated_block *ab)
{
    switch (selectedAlgo)
    {
        case FIRST_FIT:
            return firstFit(ab);
        case WORST_FIT:
            return worstFit(ab);
        case BEST_FIT:
            return bestFit(ab);
        case BUDDY:
            return buddy(ab);
        default:
            return nullptr;
    }
}

int allocate_mem(allocated_block *ab)
{ //为块分配内存，真正的操作系统会在这里进行置换等操作
    if (allocated_block_head == nullptr)
    {
        allocated_block_head = ab;
        ab->start_addr = DEFAULT_MEM_START;
        free_block_head->start_addr += ab->size; //in case of non-zero DEFAULT_MEM_START
        free_block_head->size -= ab->size;
        if (free_block_head->size == 0)
        {
            free(free_block_head);
            free_block_head = nullptr;
        }
    } else
    {
        free_block *feasibleFreeBlock = findFeasibleFreeBlock(ab);
        if (feasibleFreeBlock == nullptr)
        {
            printf("cannot find a feasible free block\n");
            return -1;
        } else
        {
            if (feasibleFreeBlock->size == ab->size)//perfect match
            {
#ifdef DEBUG
                printf("perfect match\n");
#endif
                if (feasibleFreeBlock == free_block_head)
                {
#ifdef DEBUG
                    printf("free head\n");
#endif
                    free_block_head = free_block_head->next;
                } else
                {
                    // find its previous one
                    free_block *prev = nullptr;
                    for (auto current = free_block_head;
                         current->start_addr != feasibleFreeBlock->start_addr; prev = current, current = current->next);
                    //sew the link
                    prev->next = feasibleFreeBlock->next;
                }
                ab->start_addr = feasibleFreeBlock->start_addr;
                free(feasibleFreeBlock);
            } else // not perfect match
            {
                ab->start_addr = feasibleFreeBlock->start_addr;
                feasibleFreeBlock->start_addr += ab->size;
                feasibleFreeBlock->size -= ab->size;
            }
            auto tail = find_allocated_tail();
            tail->next = ab;
        }
    }
    totalFreeSpace -= ab->size;
    return 0;
}

int create_new_process()
{ //创建新进程
    int mem_sz = 0;
    printf("Please input memory size\n");
    int result = scanf("%d", &mem_sz);
    // Write your code here
    if (result != 1)
    {
        printf("Input Error, Try again\n");
        return -1;
    }
    if (mem_sz <= 0)
    {
        printf("Invalid Input, Try again\n");
        return -1;
    }
    if (mem_sz > mem_size)
    {
        printf("Exceed Mem Size(%d)", mem_size);
        return -1;
    }
    if (mem_sz > totalFreeSpace)
    {
        printf("Exceed Free Space\n Current Total Empty Space: %d\n", totalFreeSpace);
        return -1;
    }
    // input is valid, then
    pid++;
    allocated_block *ab = (allocated_block *) malloc(sizeof(allocated_block));
    ab->size = mem_sz;
    ab->pid = pid;
    ab->next = nullptr;
    ab->data = nullptr;
    return allocate_mem(ab);
}

void swap(int *p, int *q)
{
    int tmp = *p;
    *p = *q;
    *q = tmp;
}

void rearrange()
{ //将块按照地址大小进行排序
    free_block *current, *nextNode;
    puts("Rearrange begins...");
    puts("Rearrange by address...");
    current = free_block_head;
    while (current != nullptr)
    {
        nextNode = current->next;
        while (nextNode != nullptr)
        {
            if (nextNode->start_addr < current->start_addr)
            {
                swap(&current->start_addr, &nextNode->start_addr);
                swap(&current->size, &nextNode->size);
            }
            nextNode = nextNode->next;
        }
        current = current->next;
    }
    usleep(500);
    puts("Rearrange Done.");
}

//merge free block segments
void mergeFreeBlocks()
{
    auto current = free_block_head;
    auto anchor = free_block_head;
    while (current != nullptr)
    {
        auto next = current->next;
        if (next == nullptr)
            break;
        if (next->start_addr == current->start_addr + current->size)
        {
            current->size += next->size; //merge
            //release the segment
            current->next = next->next;
            free(next);
            current = anchor; //rollback to anchor
        } else //a gap between current node and the next node
        {
            anchor = next;
            current = next;
        }
    }
}

int free_mem(allocated_block *ab)
{ //释放某一块的内存
    bool foundContinuousBlock = false;
    for (auto current = free_block_head; current != nullptr; current = current->next)
    {
        int endAddress = current->start_addr + current->size;
        if (endAddress == ab->start_addr)
        {
            foundContinuousBlock = true;
            current->size += ab->size;
            break;
        }
        if (current->start_addr == ab->start_addr + ab->size)
        {
            foundContinuousBlock = true;
            current->size += ab->size;
            current->start_addr -= ab->size;
            break;
        }
    }
    if (!foundContinuousBlock)
    {
        free_block *freeBlock = (free_block *) malloc(sizeof(free_block));
        freeBlock->size = ab->size;
        freeBlock->start_addr = ab->start_addr;
        freeBlock->next = nullptr;
        if (free_block_head == nullptr)
        {
            free_block_head = freeBlock;
        } else
        {
            auto tail = free_block_head;
            for (; tail->next != nullptr; tail = tail->next);
            tail->next = freeBlock;
            rearrange();
        }
    }
    mergeFreeBlocks();
    return 0;
}

int dispose(allocated_block *fab)
{ //释放结构体所占的内存
    allocated_block *pre, *ab;
    if (fab == allocated_block_head)
    {
        allocated_block_head = allocated_block_head->next;
        totalFreeSpace += fab->size;
        free(fab);
        return 1;
    }
    pre = allocated_block_head;
    ab = allocated_block_head->next;
    while (ab != fab)
    {
        pre = ab;
        ab = ab->next;
    }
    pre->next = ab->next;
    totalFreeSpace += fab->size;
    free(fab);
    return 2;
}

void display_mem_usage()
{
    free_block *fb = free_block_head;
    allocated_block *ab = allocated_block_head;
    puts("*********************Free Memory*********************");
    printf("%20s %20s\n", "start_addr", "size");
    int cnt = 0;
    while (fb != nullptr)
    {
        cnt++;
        printf("%20d %20d\n", fb->start_addr, fb->size);
        fb = fb->next;
    }
    if (!cnt) puts("No Free Memory");
    else printf("Totaly %d free blocks\n", cnt);
    puts("");
    puts("*******************Used Memory*********************");
    printf("%10s %10s %20s\n", "PID", "start_addr", "size");
    cnt = 0;
    while (ab != nullptr)
    {
        cnt++;
        printf("%10d %10d %20d\n", ab->pid, ab->start_addr, ab->size);
        ab = ab->next;
    }
    if (!cnt) puts("No allocated block");
    else printf("Totaly %d allocated blocks\n", cnt);
}

void kill_process()
{ //杀死某个进程
    allocated_block *ab;
    int pid;
    puts("Please input the pid of Killed process");
    scanf("%d", &pid);
    ab = find_process(pid);
    if (ab == nullptr)
    {
        printf("cannot find the process with pid %d", pid);
    } else
    {
        free_mem(ab);
        dispose(ab);
    }
}



