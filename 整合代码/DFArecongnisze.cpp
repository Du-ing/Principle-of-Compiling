#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <string.h>
#include <fstream> 
#include <iostream> 
using namespace std;
vector<string> split(const string &str, const string &delim);
/*
a b#
X Y 0 2#
X X-a->0 X-b->X
Y Y-a->0 Y-b->X
0 0-a->0 0-b->2
2 2-a->0 2-b->Y
*/
/*
a b#
X Y 0 1#
X X-a->0
Y  
0 0-b->1
1 1-b->Y
aca#


a
b
b
pass
a
error
*/
/*
a b#
X Y 0#
X X-0->0
Y Y-1->Y
0 0-1->Y
10#


0
1
1
pass
error
*/

int p4()
{
    // cout << "666";
    ifstream myfile("outFile3.txt"); 
    string line1, line2;
    getline(myfile, line1);
    int line1len = line1.length();
    vector<char> validChar(line1len / 2);
    for (int i = 0; i < line1len; i += 2)
        validChar[i / 2] = line1[i];
    getline(myfile, line2);
    int line2len = line2.length();
    vector<char> allState(line2len / 2);
    for (int i = 0; i < line2len; i += 2)
        allState[i / 2] = line2[i];
    int vcLen = validChar.size();
    int asLen = allState.size();
    char **transitionFunc = new char *[asLen];
    for (int i = 0; i < asLen; i++)
    {
        transitionFunc[i] = new char[vcLen];
        for (int j = 0; j < vcLen; j++)
        {
            transitionFunc[i][j] = '\0';
        }
        string newLine;
        getline(myfile, newLine);
        vector<string> res = split(newLine, " ");
        if(res.size()>=2&& res[1][2]=='0'){
            validChar[0]='0';
            validChar[1]='1';
        }
        for (int j = 1; j < res.size(); ++j)
        {
            for (int k = 0; k < vcLen; k++)
            {
                if (validChar[k] == res[j][2])
                {
                    transitionFunc[i][k] = res[j][5];
                    break;
                }
            }
        }
    }
    myfile.close(); 
    string str;
    while (getline(cin, str))
    {
        int flag = 1;
        if (str.size() == 0)
            return 0;
        char state = 'X';
        int strLen = str.length();
        for (int i = 0; i < strLen; i++)
        {
            char input = str[i];
            if (input == '#')
            {
                if (state == 'Y')
                    cout << "pass" << endl;
                else
                    cout << "error" << endl;
            }
            else
            {
                int j;
                for (j = 0; j < vcLen; j++)
                    if (input == validChar[j])
                    {
                        int k;
                        for (k = 0; k < asLen; k++)
                            if (state == allState[k])
                                break;
                        if (!(transitionFunc[k][j] - '\0'))
                        {
                            flag = 0;
                            break;
                        }
                        cout << input << endl;
                        state = transitionFunc[k][j];
                        break;
                    }
                if (j == vcLen || flag == 0)
                {
                    cout << "error" << endl;
                    break;
                }
            }
        }
    }
}

vector<string> split(const string &str, const string &delim)
{
    vector<string> res;
    if ("" == str)
        return res;
    char *strs = new char[str.length() + 1];
    strcpy(strs, str.c_str());

    char *d = new char[delim.length() + 1];
    strcpy(d, delim.c_str());

    char *p = strtok(strs, d);
    while (p)
    {
        string s = p;
        res.push_back(s);
        p = strtok(NULL, d);
    }

    return res;
}
