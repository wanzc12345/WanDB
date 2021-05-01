#ifndef LOG_H
#define LOG_H

class Log{
private:
    int logAmount;
public:
    Log();
    ~Log();
    bool logCommand(char* command, int time=0);
    char* readCommand(int time);
    bool deleteBefore(int time);
    bool deleteAfter(int time);
    void show(char* resultString);
    void showAfter(int time);
    bool clear();
};

#endif // LOG_H
