#pragma once
#include <list>
#include <string>

using namespace std;

struct LogEntry
{
    int turn;
    string username;
    string aksi;
    string details;
};

class GameLogger
{
private:
    list<LogEntry> logs; 
public:
    void addLog(int turn, string username, string aksi, string details);
    void printLogs(int count);
};
