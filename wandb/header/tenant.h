#ifndef TENANT_H
#define TENANT_H

class Tenant{
private:
    char* username;
    char* password;
    char** databases;
    int databaseAmount;
    int maxDatabaseAmount;
    bool isAdmin;
    int account;

public:
    Tenant();
    ~Tenant();
    void show();
    int getSize();
    bool copy(Tenant* tenant);
    bool setUsername(char* newName);
    char* getUsername(){return username;}
    bool updatePassword(char* newPass);
    bool checkPassword(char* pass);
    char* getPassword(){return password;}
    bool grantDatabase(char* dbTitle);
    bool revokeDatabase(char* dbTitle);
    bool hasPermissionOn(char* dbTitle);
    bool addAccount(int money);
    bool setAccount(int money);
    bool upLevel();
    bool downLevel();
    bool clear();
    bool hasPermissionOnAll(){return isAdmin;}
    int getAccount(){return account;}
    int getDatabaseAmount(){return databaseAmount;}
    char* getDatabaseTitle(int i){return databases[i];}
};

#endif // TENANT_H
