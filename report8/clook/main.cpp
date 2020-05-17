#include <iostream>
#include <algorithm>

using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    int startTrackNum = -1;
    int numOfTrackOnDisk = 0;
    int numOfRequest = 0;
    cin >> startTrackNum >> numOfTrackOnDisk >> numOfRequest;
    if (startTrackNum < 0 || numOfTrackOnDisk <= 0 || numOfRequest <= 0)
    {
        cout << "Illegal Input" << endl;
        exit(-1);
    }
    int *requests = new int[numOfRequest + 1];
    for (int i = 0; i < numOfRequest; i++)
        cin >> requests[i];
    // default direction inner->outer
    // track 0 is the outmost track
    requests[numOfRequest] = startTrackNum;
    sort(requests, requests + numOfRequest + 1, greater<int>());
    int idx = 0;
    for (; idx <= numOfRequest && requests[idx] != startTrackNum; idx++);
    int sum = 0;
    sum += requests[0] - requests[numOfRequest];
    if (idx != 0)
    {
        sum += requests[idx] - requests[numOfRequest];
        sum += requests[0] - requests[idx - 1];
    }
    rotate(requests, requests + idx, requests + numOfRequest + 1);
//    int previousTrack = startTrackNum;
//    int disSum = 0;
//    for (int i = 0; i <= numOfRequest; i++)
//    {
//        disSum += abs(requests[i] - previousTrack);
//        previousTrack = requests[i];
//    }
    for (int i = 0; i <= numOfRequest; i++)
        cout << requests[i] << " ";
    cout << endl;
//    cout << disSum << endl;
    cout << sum << endl;
    delete[] requests;
    return 0;
}
