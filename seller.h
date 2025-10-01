#ifndef SELLER_H
#define SELLER_H

#include "buyer.h"
#include "item.h"
#include <vector>

class seller : public Buyer {
private:
    string sellerID;
    string storeName;
    vector<Item> items; 
    
public:
    
    seller(string id, string name, string password, BankCustomer *acc, string store) 
        : Buyer(name, password, acc), sellerID(id), storeName(store) {
        
        this->id = id;
    }
    
    
    seller() : Buyer() {}


    void addNewItem(int itemID, string name, int quantity, double price) {
        items.emplace_back(itemID, name, quantity, price);
        cout << "[SUKSES] Item " << name << " ditambahkan ke inventori.\n";
    }

    void viewInventory() const {
        cout << "\n--- Inventori Toko " << storeName << " ---\n";
        if (items.empty()) {
            cout << "Inventori kosong.\n";
            return;
        }
        for (const auto& item : items) {
            item.printInfo();
        }
    }
    

    string getStoreName() const { return storeName; }
};

#endif 
