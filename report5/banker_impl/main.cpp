#include <iostream>
#include <string>
#include <sstream>
#include <hash_map>
#include <cstdlib>
#include <memory.h>

#define DEBUG
using namespace std;

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

    __gnu_cxx::hash_map<int, int *> maxRequestTable;
    __gnu_cxx::hash_map<int, int *> allocationTable;
    vector<bool> oks;
    int *requestQuantities = (int *) malloc(quantityArraySize);
    int *resourcesAfterRequest = (int *) malloc(quantityArraySize);
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
            for (int i = 0; i < resourceTypeNum; i++)
                ss >> maxRequestQuantities[i];
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
            if (!exceedMax)
            {
                maxRequestTable[pid] = maxRequestQuantities;
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
                int totalRequest = allocated[i] + requestQuantities[i];
                if (totalRequest > maxRequest[i])
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
            for (int i = 0; i < resourceTypeNum; i++)
                resourcesAfterRequest[i] = allocated[i] + request[i];


        } else if (type == terminate)
        {
            int *allocated = allocationTable.find(pid)->second;
            for (int i = 0; i < resourceTypeNum; i++)
                currentAvailableQuantities[i] += allocated[i];
            allocationTable.erase(pid);
            free(allocated);
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
    free(resourcesAfterRequest);
    free(currentAvailableQuantities);
    for (auto &it : maxRequestTable)
        free(it.second);

    for (auto &it : allocationTable)
        free(it.second);

    return 0;
}
