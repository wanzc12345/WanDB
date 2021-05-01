#ifndef METADATA_H
#define METADATA_H

class Metadata{
private:
    char** keys;
    char** addresses;
    int currentKeyAmount;
    int maxKeyAmount;
public:
    Metadata();
    ~Metadata();
    void show();
    bool save(char* key, char* ip);
    bool drop(char* key);
    char* getIpByName(char* key);
    int getSize();
    int getKeyAmount(){return currentKeyAmount;}
    char* getKey(int i){return keys[i];}
    char* getIp(int i){return addresses[i];}
};

#endif // METADATA_H
