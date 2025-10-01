#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm> 
#include "buyer.h"
#include "seller.h"
#include "bank_customer.h" 

using namespace std;


enum PrimaryPrompt { LOGIN, REGISTER, EXIT };
enum RegisterPrompt { CREATE_BUYER, CREATE_SELLER, BACK };


vector<Buyer*> userDatabase;
Buyer* loggedInUser = nullptr;


Buyer* attemptLogin(const string& name, const string& pass) {
    for (Buyer* user : userDatabase) {
        if (user->getName() == name && user->getPassword() == pass) {
            return user;
        }
    }
    return nullptr;
}


void displayLoggedInMenu(Buyer*& loggedInUser);


int main() {
    srand(time(0));
    PrimaryPrompt prompt = LOGIN;
    int choice;

    while (prompt != EXIT) {
        cout << "\n========== Menu Utama ==========\n";
        cout << "1. Login\n";
        cout << "2. Register\n";
        cout << "3. Exit\n";
        cout << "Pilih opsi: ";
        if (!(cin >> choice)) {
            cin.clear(); cin.ignore(10000, '\n'); choice = 0;
        }
        
        
        if (choice >= 1 && choice <= 3) {
            prompt = static_cast<PrimaryPrompt>(choice - 1);
        } else {
            prompt = static_cast<PrimaryPrompt>(0); 
        }

        switch (prompt) {
            case LOGIN: {
                if (loggedInUser) {
                    displayLoggedInMenu(loggedInUser);
                    break;
                }
                
                string u, p;
                cout << "Username: "; cin >> u;
                cout << "Password: "; cin >> p;

                loggedInUser = attemptLogin(u, p);

                if (loggedInUser) {
                    cout << "\n[SUKSES] Login Berhasil! Selamat datang, " << loggedInUser->getName() << ".\n";
                    displayLoggedInMenu(loggedInUser);
                } else {
                    cout << "\n[GAGAL] Username atau password salah.\n";
                }
                break;
            }

            case REGISTER: {
                RegisterPrompt regPrompt = CREATE_BUYER;
                int regChoice;
                
                cout << "\n--- Registrasi ---\n";
                cout << "1. Buat Akun Pembeli (CREATE_BUYER)\n";
                cout << "2. Buat Akun Penjual (CREATE_SELLER)\n";
                cout << "3. Kembali\n";
                cout << "Pilih: ";
                if (!(cin >> regChoice)) {
                    cin.clear(); cin.ignore(10000, '\n'); regChoice = 0;
                }
                
                if (regChoice >= 1 && regChoice <= 3) {
                    regPrompt = static_cast<RegisterPrompt>(regChoice - 1);
                }

                if (regPrompt == CREATE_BUYER || regPrompt == CREATE_SELLER) {
                    string name, pass;
                    cout << "Nama Pengguna: "; cin >> name;
                    cout << "Password: "; cin >> pass;

                    BankCustomer *newAcc = nullptr;

                    if (regPrompt == CREATE_SELLER) {
                        
                        string accName = name + " Bank";
                        string accID = "ACC" + to_string(rand() % 90000 + 10000);
                        double initialDeposit;
                        cout << "Setoran Awal (Wajib untuk Penjual): Rp"; 
                        if (!(cin >> initialDeposit)) {
                            cout << "[GAGAL] Input deposit tidak valid. Registrasi dibatalkan.\n";
                            cin.clear(); cin.ignore(10000, '\n'); 
                            break;
                        }
                        newAcc = new BankCustomer(accID, accName, initialDeposit);
                    }

                    if (regPrompt == CREATE_BUYER) {
                        Buyer* newBuyer = new Buyer(name, pass, newAcc);
                        userDatabase.push_back(newBuyer);
                        loggedInUser = newBuyer;
                        cout << "[SUKSES] Akun Pembeli berhasil dibuat dan Login.\n";
                    } else {
                        string storeName;
                        cout << "Nama Toko: "; cin >> storeName;
                        seller* newSeller = new seller("S" + to_string(rand() % 9000), name, pass, newAcc, storeName);
                        userDatabase.push_back(newSeller);
                        loggedInUser = newSeller;
                        cout << "[SUKSES] Akun Penjual berhasil dibuat dan Login.\n";
                    }
                    displayLoggedInMenu(loggedInUser);
                }
                break;
            }
            
            case EXIT:
                cout << "Keluar dari program.\n";
                break;
        }
    }
    
    
    for (Buyer* user : userDatabase) {
        delete user; 
    }
    userDatabase.clear();
    return 0;
}


void displayLoggedInMenu(Buyer*& loggedInUser) {
    int loginChoice;
    do {
        cout << "\n--- Menu Akun " << loggedInUser->getName() << " ---\n";
        cout << "Status: " << (dynamic_cast<seller*>(loggedInUser) ? "PENJUAL" : "PEMBELI") << endl;
        cout << "1. Cek Akun & Bank Info\n";
        cout << "2. Upgrade ke Penjual (Sub Prompt Peningkatan Penjual)\n";
        cout << "3. Kelola Rekening Bank (Sub Prompt Fungsi Rekening Bank)\n";
        
        
        if (dynamic_cast<seller*>(loggedInUser)) {
            cout << "5. Kelola Inventori\n";
        }
        
        cout << "4. Logout\n";
        cout << "Pilih opsi: ";
        if (!(cin >> loginChoice)) { cin.clear(); cin.ignore(10000, '\n'); loginChoice = 0; }
        
        
        if (loginChoice == 5 && !dynamic_cast<seller*>(loggedInUser)) {
            cout << "Opsi tidak valid untuk Pembeli.\n";
            continue;
        }

        switch (loginChoice) {
            case 1: { 
                cout << "\n[INFO AKUN]\n";
                cout << "Nama: " << loggedInUser->getName() << ", ID: " << loggedInUser->getId() << endl;
                if (dynamic_cast<seller*>(loggedInUser)) {
                    cout << "Toko: " << dynamic_cast<seller*>(loggedInUser)->getStoreName() << endl;
                }
                if (loggedInUser->getAccount()) {
                    loggedInUser->getAccount()->printInfo();
                } else {
                    cout << "Rekening Bank: Belum terhubung.\n";
                }
                break;
            }
            
            case 2: { 
                if (dynamic_cast<seller*>(loggedInUser) == nullptr) {
                    

                    if (loggedInUser->getAccount() == nullptr) {
                        cout << "[GAGAL] Penjual harus memiliki Rekening Bank. Silakan buat (Opsi 3) terlebih dahulu.\n";
                        break;
                    }

                    string storeName;
                    cout << "Masukkan Nama Toko: "; cin >> storeName;
                    
                    
                    string oldID = loggedInUser->getId();
                    string oldName = loggedInUser->getName();
                    string oldPass = loggedInUser->getPassword();
                    BankCustomer *oldAccount = loggedInUser->getAccount(); 

                    
                    userDatabase.erase(std::remove(userDatabase.begin(), userDatabase.end(), loggedInUser), userDatabase.end());
                    delete loggedInUser; 

                    
                    seller* newSeller = new seller(oldID, oldName, oldPass, oldAccount, storeName);
                    userDatabase.push_back(newSeller);
                    loggedInUser = newSeller; 
                    
                    cout << "\n[SUKSES] Akun diupgrade menjadi Penjual: " << storeName << endl;

                } else {
                    cout << "\n[INFO] Anda sudah berstatus Penjual.\n";
                }
                break;
            }

            case 3: { 
                BankCustomer* acc = loggedInUser->getAccount(); 
                
                if (!acc) {
                    
                    string accName = loggedInUser->getName() + " Bank";
                    string accID = "ACC" + to_string(rand() % 90000 + 10000);
                    double initialDeposit = 0.0;
                    cout << "[BANK] Masukkan Setoran Awal (Min Rp0): Rp"; 
                    if (!(cin >> initialDeposit)) {
                        cin.clear(); cin.ignore(10000, '\n'); break;
                    }
                    
                    BankCustomer *newAcc = new BankCustomer(accID, accName, initialDeposit);
                    loggedInUser->setAccount(newAcc);
                    acc = newAcc;
                    cout << "[SUKSES] Rekening Bank " << accID << " berhasil dibuat.\n";
                }

                if (acc) {
                    
                    int bankAction;
                    do {
                        cout << "\n--- Kelola Bank (Saldo: Rp" << acc->getBalance() << ") ---\n";
                        cout << "1. Cek Saldo\n";
                        cout << "2. Setor Dana\n";
                        cout << "3. Tarik Dana\n";
                        cout << "4. Kembali\n";
                        cout << "Pilih aksi: "; cin >> bankAction;
                        if (cin.fail()) { cin.clear(); cin.ignore(10000, '\n'); bankAction = 0; }

                        if (bankAction == 1) {
                            cout << "   [SALDO] Saldo Anda: Rp" << acc->getBalance() << endl;
                        } else if (bankAction == 2) {
                            double amount; cout << "Jumlah Setoran: Rp"; cin >> amount;
                            if (amount > 0) acc->addBalance(amount); else cout << "Jumlah harus positif.\n";
                        } else if (bankAction == 3) {
                            double amount; cout << "Jumlah Penarikan: Rp"; cin >> amount;
                            acc->withdrawBalance(amount); 
                        }
                    } while (bankAction != 4);
                }
                break;
            }
            
            case 5: { 
                seller* currentSeller = dynamic_cast<seller*>(loggedInUser);
                if (currentSeller) {
                    int invChoice;
                    cout << "\n--- Inventori " << currentSeller->getStoreName() << " ---\n";
                    cout << "1. Tambah Item\n";
                    cout << "2. Lihat Inventori\n";
                    cout << "3. Kembali\n";
                    cout << "Pilih: "; cin >> invChoice;
                    
                    if (invChoice == 1) {
                        string name; int id, qty; double price;
                        cout << "ID Item: "; cin >> id;
                        cout << "Nama: "; cin >> name;
                        cout << "Kuantitas: "; cin >> qty;
                        cout << "Harga: "; cin >> price;
                        currentSeller->addNewItem(id, name, qty, price);
                    } else if (invChoice == 2) {
                        currentSeller->viewInventory();
                    }
                }
                break;
            }

            case 4:
                loggedInUser = nullptr;
                cout << "Logout berhasil. Kembali ke Menu Utama.\n";
                break;
                
            default:
                cout << "Opsi tidak valid. Coba lagi.\n";
        }
    } while (loginChoice != 4);
}
