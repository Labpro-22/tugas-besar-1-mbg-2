#include "GameLogger.hpp"

LogEntry::LogEntry(int turn, const string& username, const string& action, const string& details)
    : turn(turn), username(username), action(action), details(details) {}

int LogEntry::getTurn() const {
    return turn;
}

const string& LogEntry::getUsername() const {
    return username;
}

const string& LogEntry::getAction() const {
    return action;
}
const string& LogEntry::getDetails() const {
    return details;
}

void GameLogger::addLog(int turn, const string& username, const string& action, const string& details){
    logs.push_back(LogEntry{turn, username, action, details});
}

void GameLogger::printLogs(int count) const{
    if (logs.empty()){
        cout << "[LOG] Belum ada aksi yang tercatat!" << endl;
        return;
    }
    int total = int(logs.size());
    int limit = (count <= 0 || count > total) ? total : count;
    cout << "=== Log Transaksi (" << limit << " Terakhir) ===" << endl;

    int start = total - limit;
    for (int i = start; i < total; i++){
        auto& entry = logs[i];
        cout << "[Turn " << entry.getTurn() << "] " << entry.getUsername() << " | " << entry.getAction() << " | ";
        if (entry.getDetails().empty()){
            cout << "-" <<endl;
        } else {
            cout << entry.getDetails() << endl;
        }
    }
}

int GameLogger::getLogCount() const{
    return int(logs.size());
}

const vector<LogEntry>& GameLogger::getLogs() const{
    return logs;
}

void GameLogger::clearLogs(){
    logs.clear();
}

void GameLogger::saveToStream(ostream& out) const {
    out << logs.size() << '\n';
    for (const auto& entry : logs) {
        out << entry.getTurn() << ' ' << quoted(entry.getUsername()) << ' ' << quoted(entry.getAction()) << ' ' << quoted(entry.getDetails()) << '\n';
    }
}

void GameLogger::loadFromStream(istream& in) {
    int count = 0;
    if (!(in >> count)) {
        return;
    }

    logs.clear();
    logs.reserve(count); 

    for (int i = 0; i < count; ++i) {
        LogEntry entry;
        int turn;
        string username, action, details;
        if (!(in >> turn >> quoted(username) >> quoted(action) >> quoted(details))) {
            break;
        }
        logs.push_back(LogEntry(turn, username, action, details));
    }
}