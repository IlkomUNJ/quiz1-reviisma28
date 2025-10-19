#ifndef BUYER_H
#define BUYER_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "bank_customer.h" 

using namespace std;

class Buyer {
protected:
    string id;
    string name;
    string password;
    BankCustomer *account; 
    
public:
    Buyer(string n = "", string p = "", BankCustomer *acc = nullptr) 
        : name(n), password(p), account(acc) {
        srand(time(0) + rand());
        id = "B" + to_string(rand() % 90000 + 10000); 
    }
    
    // Getter dasar
    string getId() const { return id; }
    string getName() const { return name; }
    string getPassword() const { return password; }
    BankCustomer *getAccount() const { return account; }

    
    void setAccount(BankCustomer *acc) { account = acc; }


    virtual ~Buyer() = default;
};

#endif 
