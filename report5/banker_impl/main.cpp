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
    int *maxAvailableQuantities = (int *) malloc(quantityArraySize);
    int *currentAvailableQuantities = (int *) malloc(quantityArraySize);
    for (int i = 0; i < resourceTypeNum; i++)
    {
        cin >> maxAvailableQuantities[i];
        currentAvailableQuantities[i] = maxAvailableQuantities[i];
    }
    string line;
    __gnu_cxx::hash_map<int, int *> maxRequestTable;
    __gnu_cxx::hash_map<int, int *> allocationTable;
    vector<bool> oks;
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
                if (maxAvailableQuantities[i] < maxRequestQuantities[i])
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
            ////////////
            oks.push_back(true);
        } else if (type == terminate)
        {
            /////////////////
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

    free(maxAvailableQuantities);
    maxAvailableQuantities = nullptr;
    free(currentAvailableQuantities);
    currentAvailableQuantities = nullptr;
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
