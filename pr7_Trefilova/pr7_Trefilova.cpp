// pr7_Trefilova.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <string>

#define DEBUG_MODE
using namespace std;

const int MAX = 100;

struct Booking {
    int bookingNumber;
    string clientName;
    int tableNumber;
    string time;
    int guests;
    string status;
};

Booking bookings[MAX];
int total = 0;
const string filename = "bookings.txt";

Booking parseLine(string line) {
    Booking b;

    int p1 = line.find(';');
    int p2 = line.find(';', p1 + 1);
    int p3 = line.find(';', p2 + 1);
    int p4 = line.find(';', p3 + 1);
    int p5 = line.find(';', p4 + 1);

    if (p5 == -1) {
        throw runtime_error("Недостаточно полей в строке");
    }

    b.bookingNumber = stoi(line.substr(0, p1));
    b.clientName = line.substr(p1 + 1, p2 - p1 - 1);
    b.tableNumber = stoi(line.substr(p2 + 1, p3 - p2 - 1));
    b.time = line.substr(p3 + 1, p4 - p3 - 1);
    b.guests = stoi(line.substr(p4 + 1, p5 - p4 - 1));
    b.status = line.substr(p5 + 1);

    if (b.bookingNumber <= 0 || b.tableNumber <= 0 || b.guests <= 0) {
        throw runtime_error("Некорректные числа в строке");
    }

    return b;
}

void loadFromFile() {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Файл не открывается");
    }

    string line;
    while (getline(file, line)) {
        if (line == "") continue;
        if (total >= MAX) throw runtime_error("Слишком много записей");

        bookings[total] = parseLine(line);
        total++;
    }
    file.close();

#ifdef DEBUG_MODE
    cout << "[DEBUG] Загружено: " << total << endl;
#endif
}

void saveToFile() {
    ofstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Файл не открывается для записи");
    }
    for (int i = 0; i < total; i++) {
        file << bookings[i].bookingNumber << ";"
            << bookings[i].clientName << ";"
            << bookings[i].tableNumber << ";"
            << bookings[i].time << ";"
            << bookings[i].guests << ";"
            << bookings[i].status << "\n";
    }
    file.close();

#ifdef DEBUG_MODE
    cout << "[DEBUG] Сохранено: " << total << endl;
#endif
}

int findIndex(int number) {
    for (int i = 0; i < total; i++) {
        if (bookings[i].bookingNumber == number) return i;
    }
    return -1;
}

void printBooking(Booking b) {
    cout << b.bookingNumber << "\t"
        << b.clientName << "\t"
        << b.tableNumber << "\t"
        << b.time << "\t"
        << b.guests << "\t"
        << b.status << "\n";
}

void searchBooking() {
    int number;
    cout << "Номер брони: ";
    cin >> number;
    int i = findIndex(number);
    if (i == -1) {
        cout << "Не найдено\n";
    }
    else {
        cout << "Номер\tКлиент\tСтол\tВремя\tГости\tСтатус\n";
        printBooking(bookings[i]);
    }
}

void clientBookings() {
    string name;
    cout << "Имя клиента: ";
    cin >> name;
    bool fg = false;
    cout << "Номер\tКлиент\tСтол\tВремя\tГости\tСтатус\n";
    for (int i = 0; i < total; i++) {
        if (bookings[i].clientName == name) {
            printBooking(bookings[i]);
            fg = true;
        }
    }
    if (!fg) cout << "Нет броней\n";
}

void freeTables() {
    string time;
    cout << "Время: ";
    cin >> time;
    cout << "Свободные столы: ";
    for (int t = 1; t <= 10; t++) {
        bool busy = false;
        for (int i = 0; i < total; i++) {
            if (bookings[i].tableNumber == t &&
                bookings[i].time == time &&
                bookings[i].status == "Забронирован") {
                busy = true;
            }
        }
        if (!busy) cout << t << " ";
    }
    cout << "\n";
}

void cancelBooking() {
    int number;
    cout << "Номер брони: ";
    cin >> number;
    int i = findIndex(number);
    if (i == -1) {
        cout << "Не найдено\n";
        return;
    }
    if (bookings[i].status == "Отменен") {
        throw runtime_error("Уже отменена");
    }
    bookings[i].status = "Отменен";
    cout << "Отменено\n";
    saveToFile();
}

void completeBooking() {
    int number;
    cout << "Номер брони: ";
    cin >> number;
    int i = findIndex(number);
    if (i == -1) {
        cout << "Не найдено\n";
        return;
    }
    bookings[i].status = "Завершен";
    cout << "Статус: Завершен\n";
    saveToFile();
}

void printAll() {
    cout << "Номер\tКлиент\tСтол\tВремя\tГости\tСтатус\n";
    for (int i = 0; i < total; i++) printBooking(bookings[i]);
}

int main() {
    setlocale(NULL, "Russian");
    try {
        loadFromFile();
    }
    catch (exception& e) {
        cout << "Ошибка загрузки: " << e.what() << "\n";
    }

    int choice = 0;
    while (choice != 8) {
        cout << "\n1-Поиск\n";
        cout << "2-Брони клиента\n";
        cout << "3-Свободные столы\n";
        cout << "4-Отмена\n";
        cout << "5-Посещение\n";
        cout << "6-Все\n";
        cout << "Выбор: ";
        cin >> choice;

        try {
            if (choice == 1) searchBooking();
            else if (choice == 2) clientBookings();
            else if (choice == 3) freeTables();
            else if (choice == 4) cancelBooking();
            else if (choice == 5) completeBooking();
            else if (choice == 6) printAll();
            else cout << "Нет такого пункта\n";
        }
        catch (exception& e) {
            cout << "ОШИБКА: " << e.what() << "\n";
        }
    }
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
