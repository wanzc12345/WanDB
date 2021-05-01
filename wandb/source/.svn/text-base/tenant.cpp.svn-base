#include "../header/tenant.h"
#include "string.h"

Tenant::Tenant(){
    username = new char[10];
    password = new char[10];
    maxDatabaseAmount = 1;
    databaseAmount = 0;
    databases = new char*[maxDatabaseAmount];
    for(int i=0;i<maxDatabaseAmount;i++){
        databases[i] = new char[10];
    }
    isAdmin = false;
    account = 0;
}

Tenant::~Tenant(){
    delete [] username;
    delete [] password;
    for(int i=0;i<maxDatabaseAmount;i++){
        delete [] databases[i];
    }
    delete [] databases;
}

bool Tenant::checkPassword(char *pass){
    if(strcmp(this->password, pass)){
        return false;
    }else{
        return true;
    }
}

bool Tenant::copy(Tenant *tenant){
    clear();
    setUsername(tenant->username);
    updatePassword(tenant->password);
    databaseAmount = tenant->databaseAmount;
    for(int i=0;i<databaseAmount;i++)
        grantDatabase(tenant->databases[i]);
    return true;
}

bool Tenant::clear(){
    databaseAmount = 0;
    return true;
}

int Tenant::getSize(){
    int sum=0;
    sum += maxDatabaseAmount*8;
    sum += 20;
    return sum;
}

bool Tenant::setUsername(char* newName){
    if(strlen(newName)>strlen(username))
        username = new char[strlen(newName)];
    strcpy(username, newName);
    return true;
}

bool Tenant::setAccount(int money){
    account = money;
    return true;
}

bool Tenant::addAccount(int money){
    account += money;
    return true;
}

bool Tenant::updatePassword(char *newPass){
    if(strlen(newPass)>strlen(password))
        password = new char[strlen(newPass)];
    strcpy(password, newPass);
    return true;
}

bool Tenant::upLevel(){
    isAdmin = true;
    return true;
}

bool Tenant::downLevel(){
    isAdmin = false;
    return true;
}

bool Tenant::grantDatabase(char* dbTitle){
    if(databaseAmount == maxDatabaseAmount){
        maxDatabaseAmount *= 2;
        char** tmp = new char*[databaseAmount];
        for(int i=0;i<databaseAmount;i++){
            tmp[i] = new char[10];
            strcpy(tmp[i], databases[i]);
        }
        databases = new char*[maxDatabaseAmount];
        for(int i=0;i<maxDatabaseAmount;i++)
            databases[i] = new char[10];
        for(int i=0;i<databaseAmount;i++)
            strcpy(databases[i], tmp[i]);
    }
    strcpy(databases[databaseAmount], dbTitle);
    databaseAmount++;
    return true;
}

bool Tenant::revokeDatabase(char* dbTitle){
    for(int i=0;i<databaseAmount;i++){
        if(!strcmp(databases[i], dbTitle)){
            strcpy(databases[i], databases[databaseAmount-1]);
            databaseAmount--;
            return true;
        }
    }
    return false;
}

bool Tenant::hasPermissionOn(char *dbTitle){
    for(int i=0;i<databaseAmount;i++){
        if(!strcmp(databases[i], dbTitle)){
            return true;
        }
    }
    return false;
}
