#pragma once
#include <string>
#include <vector>
#include <ostream>
#include <istream>
#include <iostream>
#include <iomanip>

using namespace std;

class LogEntry
{
    int turn;
    string username;
    string action;
    string details;

public:
    LogEntry() = default;
    LogEntry(int turn, const string& username, const string& action, const string& details);
    int getTurn() const;
    const string& getUsername() const;
    const string& getAction() const;
    const string& getDetails() const;
};

class GameLogger
{
private:
    vector<LogEntry> logs; 
public:
    void addLog(int turn, const string& username, const string& action, const string& details);
    void printLogs(int count) const;
    int getLogCount() const;
    const vector<LogEntry>& getLogs() const;
    void clearLogs();

    void saveToStream(ostream& out) const;
    void loadFromStream(istream& in);
};
