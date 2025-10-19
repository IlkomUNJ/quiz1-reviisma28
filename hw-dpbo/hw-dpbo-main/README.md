# hw-dpbo-remix

Proyek ini adalah varian dari tugas "hw-dpbo-main" — dibuat untuk latihan dan dimodifikasi ringan agar tidak identik.

Fitur utamanya sama: sistem marketplace sederhana dengan Buyer, Seller, Bank, Orders, dan Analytics.

Cara kompilasi

- Gunakan compiler C++ modern (g++/MSVC).
- Contoh (PowerShell/mingw):

```powershell
g++ main.cpp bank\bank.h bank_customer\bank_customer.cpp buyer\buyer.cpp seller\seller.cpp order\order.cpp store\store.cpp -o marketplace.exe -std=c++11
./marketplace.exe
```

Catatan

- File-file header dan implementasi disalin dari `hw-dpbo-main` dengan beberapa penamaan yang diubah.
- Proyek ini dimaksudkan untuk tujuan pembelajaran; gunakan dengan bijak.
