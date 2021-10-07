#include <iostream>
#include <list>
#include <string>
#include <ctime>

using namespace std;

class HashTable {
private:
    static const int hashGroups = 11; // размер хеш таблицы
    list<pair<int, string>> table[hashGroups];

public:

    static int hashFunction(int key);

    static int hashFunctionS(int key);

    void insertItem(int key, const string &value);

    void removeItem(int key);

    string searchTable(int key);

    void printTable();
};

int HashTable::hashFunction(int key) {
    return key % hashGroups; // хеш = остаток от деления на резмерхеш таблицы
}

int HashTable::hashFunctionS(int key) {
    return key % (hashGroups - 1) + 1; // хеш = (остаток от деления на (резмерхеш таблицы - 1)) + 1
}

void HashTable::insertItem(int key, const string &value) {
    int hashValue = hashFunction(key); // создаем хеш
    auto &cell = table[hashValue]; // присваиваем переменной cell пару элементов таблицы
    auto bItr = begin(cell); // создаем итератор
    bool keyExists = false;
    for (; bItr != end(cell); bItr++) { // итерируем
        if (bItr->first == key) { // заходим сюда только если элемент с таким же ключем существует
            keyExists = true;
            int collisionCounter = 0;
            while (cell.empty() || bItr->first == key) {
                collisionCounter++;
                hashValue = (hashFunction(key) + collisionCounter * hashFunctionS(key)) % hashGroups;
                // избавляем от коллизи путем двойного хеширования
                cell = table[hashValue];
                bItr = begin(cell);
            }
            bItr->second = value;
            cout << "\n[WARNING] Key exists. Value replaced.\n" << endl;
            break;
        }
    }

    if (!keyExists) { // заходим сюда если ключ уникален
        cell.emplace_back(key, value);
    }
}

void HashTable::removeItem(int key) {
    int hashValue = hashFunction(key); // создаем хеш
    auto &cell = table[hashValue]; // присваиваем переменной cell пару элементов таблицы
    auto bItr = begin(cell); // создаем итератор
    bool keyExists = false;
    for (; bItr != end(cell); bItr++) { // итерируем
        if (bItr->first == key) { // заходим сюда только если элемент с таким же ключем существует
            keyExists = true;
            int collisionCounter = 0;
            while (cell.empty() || bItr->first == key) {
                collisionCounter++;
                hashValue = (hashFunction(key) + collisionCounter * hashFunctionS(key)) % hashGroups;
                // избавляем от коллизи путем двойного хеширования
                cell = table[hashValue];
                bItr = begin(cell);
            }
            cell.erase(bItr);
            cout << "\n[INFO] Item removed.\n" << endl;
            break;
        }
    }
    if (!keyExists) { // заходим сюда если ключ уникален
        cout << "\n[WARNING] Key not found. Item not removed.\n" << endl;
    }
}

string HashTable::searchTable(int key) {
    int hashValue = hashFunction(key); // создаем хеш
    auto &cell = table[hashValue]; // присваиваем переменной cell пару элементов таблицы
    auto bItr = begin(cell); // создаем итератор
    for (; bItr != end(cell); bItr++) { // итерируем
        if (bItr->first == key) { // заходим сюда только если элемент с таким же ключем существует
            int collisionCounter = 0;
            while (cell.empty() || bItr->first != key) {
                collisionCounter++;
                hashValue = (hashFunction(key) + collisionCounter * hashFunctionS(key)) % hashGroups;
                // избавляем от коллизи путем двойного хеширования
                cell = table[hashValue];
                bItr = begin(cell);
            }
            return bItr->second;
        }
    }
    // заходим сюда если ключ уникален
    return "[WARNING] No matching key";
}

void HashTable::printTable() {
    for (int i{}; i < hashGroups; i++) { // перебираем элементы таблицы
        if (table[i].empty()) continue; // если пусто идем дальше

        auto bItr = table[i].begin(); // если не пусто, то итерируем
        for (; bItr != table[i].end(); bItr++) {
            cout << "[INFO] Key: " << bItr->first << " Value: " << bItr->second << endl; //выводим инфу про читателя
        }
    }
}

int AutoID() { // генератор номера читателя
    return rand() % 100000;
}

string AutoName() { // генератор имени и адреса читателя
    string names[10] = {"Zachary Collins", "Sam Butler", "Owen Hamilton", "William Brown", "Daniel Bell",
                        "Brendan Cotton", "Augustine Rollins", "Ethan Copeland", "Camdyn Chavez", "Emerson Bryant"};
    string street[10] = {"Mohave St", "Addison St", "Old Mcintosh Rd", "Palmer Rd", "Country Club Rd",
                         "Candlecrest Cir", "W 7th Street Ct", "Gumboro Rd", "Peach Ave", "Mable Dr"};
    return names[rand() % 10] + " " + street[rand() % 10];
}

int main() {
    srand(time(nullptr));
    HashTable HT;
    cout << "Enter table size:\n";
    int N = 5;
//    cin >> N;
    for (int i{}; i < N; i++) {
        HT.insertItem(AutoID(), AutoName());
    }
    char choise = ' ';
    bool flag = true;
    int id{};
    string value{};
    while (flag) { // простой текстовый интерфейс
        cout << "\nChoose function you want to use:\nDelete=d\nFind=f\nAdd reader=a\nPrint table=p\nExit=e\n";
        cin >> choise;
        switch (choise) {
            case 'd': {
                //system("cls");
                cout << "Enter Id to remove it:\n";
                cin >> id;
                HT.removeItem(id);
                break;
            }

            case 'f': {
//                system("cls");
                cout << "Enter Id to find it:\n";
                cin >> id;
                cout << "Name, address: "+HT.searchTable(id);
                break;
            }

            case 'a': {
//                system("cls");
                cout << "Enter Id, name, address to add it:\n";
                cin >> id;
                getline(cin, value);
                HT.insertItem(id, value);
                break;
            }

            case 'p': {
//                system("cls");
                HT.printTable();
                break;
            }

            case 'e': {
                cout << ("exiting");
            }

            default: {
                flag = false;
                break;
            }
        }
    }

    return 0;
}