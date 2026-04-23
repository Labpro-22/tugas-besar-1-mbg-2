#pragma once
#include <string>
#include <vector>
#include <ostream>
#include <istream>
#include <iostream>

using namespace std;

struct LogEntry
{
    int turn;
    string username;
    string action;
    string details;
};

class GameLogger
{
private:
    vector<LogEntry> logs; 
public:
    void addLog(int turn, const string& username, string& action, const string& details);
    void printLogs(int count) const;
    int getLogCount() const;
    const vector<LogEntry>& getLogs() const;
    void clearLogs();

    void saveToStream(ostream& out) const;
    void loadFromStream(istream& in);
};
