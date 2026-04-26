#include "GameLogger.hpp"


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
        cout << "[Turn " << entry.turn << "] " << entry.username << " | " << entry.action << " | ";
        if (entry.details.empty()){
            cout << "-" <<endl;
        } else {
            cout << entry.details << endl;
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
        out << entry.turn << ' ' << quoted(entry.username) << ' ' << quoted(entry.action) << ' ' << quoted(entry.details) << '\n';
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
        LogEntry entry{};
        if (!(in >> entry.turn >> quoted(entry.username) >> quoted(entry.action) >> quoted(entry.details))) {
            break;
        }
        logs.push_back(entry);
    }
}