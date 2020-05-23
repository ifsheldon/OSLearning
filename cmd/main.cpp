#include <iostream>
#include <sstream>
#include <tuple>

using namespace std;

pair<string *, int> split_command(const string &line)
{
    int spaceCount = 0;
    for (char c:line)
    {
        if (c == ' ')
            spaceCount++;
    }
    int argCount = spaceCount + 1;
    string *args = new string[argCount];
    istringstream iss(line);
    for (int i = 0; i < argCount; i++)
        iss >> args[i];
    return make_pair(args, argCount);
}

int execute(string *args, int argLength)
{
    return 0;
}

void shell_loop()
{
    int status;
    string line;
    string *args;
    int argLength;
    do
    {
        cout << "> ";
        cin >> line;
        tie(args, argLength) = split_command(line);
        status = execute(args, argLength);
        delete[] args;
    } while (status);
}

int main()
{
    ios::sync_with_stdio(false);
    string s = "abc edf";
    int count = 0;
    for (char i : s)
    {
        if (i == ' ')
            count++;
    }
    istringstream iss(s);
    string sub;
    for (int i = 0; i < count + 1; i++)
    {
        iss >> sub;
        cout << sub << endl;
    }
    return 0;
}
