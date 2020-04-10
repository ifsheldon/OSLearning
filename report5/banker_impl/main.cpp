#include <iostream>
#include <string>
#include <cstdio>
#include <sstream>
#include <hash_map>
#include <cstdlib>
#include <memory.h>

using namespace std;

int main()
{
    static string ok = "OK";
    static string nok = "NOT OK";
    static string neww = "new";
    static string request = "request";
    static string terminate = "terminate";
    ios::sync_with_stdio(false);
    freopen("Sample.in", "r", stdin);
    int resourceTypeNum;
    cin >> resourceTypeNum;
    size_t quantityArraySize = sizeof(int) * resourceTypeNum;
    int *maxAvailableQuantities = (int *) malloc(quantityArraySize);
    for (int i = 0; i < resourceTypeNum; i++)
        cin >> maxAvailableQuantities[i];
    string line;
    __gnu_cxx::hash_map<int, int *> maxRequestTable;
    __gnu_cxx::hash_map<int, int *> allocationTable;
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
            ////////////////
            maxRequestTable[pid] = maxRequestQuantities;
            int *allocatedQuantities = (int *) malloc(quantityArraySize);
            memset(allocatedQuantities, 0, quantityArraySize);
            allocationTable[pid] = allocatedQuantities;
        } else if (type == request)
        {
            ////////////
        } else if (type == terminate)
        {
            ////////
        } else
        {
            ///error
        }


    }
    free(maxAvailableQuantities);
    maxAvailableQuantities = nullptr;
    for (auto &it : maxRequestTable)
    {
        free(it.second);
        it.second = nullptr;
    }
    for (auto &it : allocationTable)
    {
        free(it.second);
        it.second = nullptr;
    }
    return 0;
}
