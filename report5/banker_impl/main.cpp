#include <iostream>
#include <string>
#include <sstream>
#include <hash_map>
#include <cstdlib>
#include <memory.h>

#define DEBUG
using namespace std;
using namespace __gnu_cxx;

bool isSafe(const int *remainResources, const int *newNeed, int len, int pid, const hash_map<int, int *> &need)
{
    return false;
}

int main()
{
    static string ok = "OK";
    static string nok = "NOT OK";
    static string neww = "new";
    static string request = "request";
    static string terminate = "terminate";
    ios::sync_with_stdio(false);

#ifdef DEBUG
    freopen("Sample.in", "r", stdin);// redirect stdio
#endif

    int resourceTypeNum;
    cin >> resourceTypeNum;
    size_t quantityArraySize = sizeof(int) * resourceTypeNum;
    int *currentAvailableQuantities = (int *) malloc(quantityArraySize);
    for (int i = 0; i < resourceTypeNum; i++)
        cin >> currentAvailableQuantities[i];

    hash_map<int, int *> maxRequestTable;
    hash_map<int, int *> allocationTable;
    hash_map<int, int *> needTable;
    vector<bool> oks;
    int *requestQuantities = (int *) malloc(quantityArraySize);
    int *remainResourcesAfterRequest = (int *) malloc(quantityArraySize);
    int *newNeedAfterRequest = (int *) malloc(quantityArraySize);
    string line;
    while (getline(cin, line))
    {
        if (line.length() == 0)
            continue;
        istringstream ss(line);
        int pid;
        string type;
        ss >> pid >> type;
        if (type == neww)
        {
            int *maxRequestQuantities = (int *) malloc(quantityArraySize);
            int *need = (int *) malloc(quantityArraySize);
            for (int i = 0; i < resourceTypeNum; i++)
            {
                ss >> maxRequestQuantities[i];
                need[i] = maxRequestQuantities[i];
            }
            // also need to check maxAvailableQuantities before adding to hashmap
            bool exceedMax = false;
            for (int i = 0; i < resourceTypeNum; i++)
            {
                if (currentAvailableQuantities[i] < maxRequestQuantities[i])
                {
                    exceedMax = true;
                    break;
                }
            }
            if (exceedMax)
            {
                free(maxRequestQuantities);
                free(need);
            } else
            {
                maxRequestTable[pid] = maxRequestQuantities;
                needTable[pid] = need;
                int *allocatedQuantities = (int *) malloc(quantityArraySize);
                memset(allocatedQuantities, 0, quantityArraySize);
                allocationTable[pid] = allocatedQuantities;
            }
            oks.push_back(!exceedMax);
        } else if (type == request)
        {
            //get request
            for (int i = 0; i < resourceTypeNum; i++)
                ss >> requestQuantities[i];
            // check if still available
            bool stillAvailableAfterAllocation = true;
            for (int i = 0; i < resourceTypeNum; i++)
            {
                int remain = currentAvailableQuantities[i] - requestQuantities[i];
                if (remain < 0)
                {
                    stillAvailableAfterAllocation = false;
                    break;
                }
            }
            if (!stillAvailableAfterAllocation)
            {
                oks.push_back(false);
                continue;
            }
            //check if exceed maxRequest
            int *maxRequest = maxRequestTable.find(pid)->second;
            int *allocated = allocationTable.find(pid)->second;
            bool exceedMaxRequest = false;
            for (int i = 0; i < resourceTypeNum; i++)
            {
                int totalAllocated = allocated[i] + requestQuantities[i];
                if (totalAllocated > maxRequest[i])
                {
                    exceedMaxRequest = true;
                    break;
                }
            }
            if (exceedMaxRequest)
            {
                oks.push_back(false);
                continue;
            }
            // not exceed max request and resources permit, then check safety
            int *need = needTable.find(pid)->second;
            for (int i = 0; i < resourceTypeNum; i++)
            {
                remainResourcesAfterRequest[i] = currentAvailableQuantities[i] - request[i];
                newNeedAfterRequest[i] = need[i] - request[i];
            }
            bool safe = isSafe(remainResourcesAfterRequest, newNeedAfterRequest, resourceTypeNum, pid, needTable);
            if (safe)
            {
                //assign resources
                for (int i = 0; i < resourceTypeNum; i++)
                {
                    need[i] = newNeedAfterRequest[i];
                    currentAvailableQuantities[i] = remainResourcesAfterRequest[i];
                    allocated[i] = allocated[i] + requestQuantities[i];
                }
            }
            oks.push_back(safe);

        } else if (type == terminate)
        {
            int *allocated = allocationTable.find(pid)->second;
            int *need = needTable.find(pid)->second;
            for (int i = 0; i < resourceTypeNum; i++)
                currentAvailableQuantities[i] += allocated[i];
            allocationTable.erase(pid);
            needTable.erase(pid);
            free(allocated);
            free(need);
            oks.push_back(true);
        } else
        {
            ///error
            cout << "input error" << endl;
            exit(-1);
        }

    }
    for (auto &&i : oks)
        cout << (i ? ok : nok) << endl;

    // free all allocated vars
    free(requestQuantities);
    free(remainResourcesAfterRequest);
    free(currentAvailableQuantities);
    free(newNeedAfterRequest);
    for (auto &it : maxRequestTable)
        free(it.second);

    for (auto &it : allocationTable)
        free(it.second);

    for (auto &it:needTable)
        free(it.second);

    return 0;
}
