// Lab_10_1.cpp
// Єфіменко Софія
// Лабораторна робота № 10.1
// Послідовний пошук в масиві структур. Рівень В.
// Варіант 8

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <string>
#if defined(_WIN32)
#include <Windows.h>
#endif

using namespace std;

// Перелік спеціальностей (Рівень В)
enum Spec { KN, IN, ME, FI, TN };

const string specStr[] = {
    "Комп'ютерні науки",
    "Інформатика",
    "Математика та ек.",
    "Фізика та інф.",
    "Трудове навчання"
};

const string profSubjectName[] = {
    "програмування",
    "чисельних методів",
    "педагогіки",
    "педагогіки",
    "педагогіки"
};

const string subjectTitles[5] = {
    "Фізика            ",
    "Математика        ",
    "Програмування (KN)",
    "Чис. методи (IN)  ",
    "Педагогіка        "
};

// Структура студента з анонімним об'єднанням для третьої оцінки.
// Усі поля union мають тип int і займають одну й ту саму область пам'яті.
struct Student {
    string prizv;
    int kurs;
    Spec spec;
    int fizyka;
    int matematika;
    union {
        int programuvannya;   // для спеціальності KN
        int chyselni_metody;  // для спеціальності IN
        int pedagogika;       // для спеціальностей ME, FI, TN
    };
};

// Прототипи функцій
void Create(Student* p, const int N);
void Print(const Student* p, const int N);
void CalcAverages(const Student* p, const int N, double avg[5], int cnt[5]);
int CountPhysics5or4(const Student* p, const int N);

// Введення даних про студентів
void Create(Student* p, const int N) {
    int ispec;
    for (int i = 0; i < N; i++) {
        cout << "\nСтудент № " << i + 1 << ":" << endl;

        cout << "  Прізвище: ";
        getline(cin >> ws, p[i].prizv);

        cout << "  Курс: ";
        cin >> p[i].kurs;

        cout << "  Спеціальність (0 - Комп'ютерні науки, 1 - Інформатика, 2 - Математика та ек., 3 - Фізика та інф., 4 - Трудове навчання): ";
        cin >> ispec;
        if (ispec < 0 || ispec > 4) ispec = 0;
        p[i].spec = (Spec)ispec;

        cout << "  Оцінка з фізики: ";
        cin >> p[i].fizyka;

        cout << "  Оцінка з математики: ";
        cin >> p[i].matematika;

        // Усі поля union (int) ділять спільну область пам'яті, тому записуємо напряму
        cout << "  Оцінка з " << profSubjectName[p[i].spec] << ": ";
        cin >> p[i].programuvannya;
    }
}

// Вивід масиву у вигляді таблиці (9 стовпців)
void Print(const Student* p, const int N) {
    cout << "\n========================================================================================================\n";
    cout << "| " << setw(3)  << right << "№"
         << " | " << setw(14) << left  << "Прізвище"
         << " | " << setw(4)  << right << "Курс"
         << " | " << setw(18) << left  << "Спеціальність"
         << " | " << setw(6)  << right << "Фізика"
         << " | " << setw(6)  << right << "Матем."
         << " | " << setw(6)  << right << "Прогр."
         << " | " << setw(8)  << right << "Чис.мет."
         << " | " << setw(6)  << right << "Педаг."
         << " |\n";
    cout << "--------------------------------------------------------------------------------------------------------\n";

    for (int i = 0; i < N; i++) {
        // Зчитуємо третю оцінку один раз із спільної області пам'яті union
        int thirdMark = p[i].programuvannya;
        cout << "| " << setw(3)  << right << i + 1
             << " | " << setw(14) << left  << p[i].prizv
             << " | " << setw(4)  << right << p[i].kurs
             << " | " << setw(18) << left  << specStr[p[i].spec]
             << " | " << setw(6)  << right << p[i].fizyka
             << " | " << setw(6)  << right << p[i].matematika
             << " | " << setw(6)  << right << (p[i].spec == KN ? to_string(thirdMark) : "-")
             << " | " << setw(8)  << right << (p[i].spec == IN ? to_string(thirdMark) : "-")
             << " | " << setw(6)  << right << (p[i].spec > IN  ? to_string(thirdMark) : "-")
             << " |\n";
    }
    cout << "========================================================================================================\n";
}

// Обчислення середнього балу для кожного з 5 предметів за один прохід по масиву:
// індекси: 0 - Фізика, 1 - Математика, 2 - Програмування, 3 - Чис. методи, 4 - Педагогіка
void CalcAverages(const Student* p, const int N, double avg[5], int cnt[5]) {
    double sum[5] = { 0.0, 0.0, 0.0, 0.0, 0.0 };
    for (int j = 0; j < 5; j++) {
        avg[j] = 0.0;
        cnt[j] = 0;
    }

    for (int i = 0; i < N; i++) {
        sum[0] += p[i].fizyka;
        cnt[0]++;

        sum[1] += p[i].matematika;
        cnt[1]++;

        // Індекс профільного предмету залежить від спеціальності,
        // а значення оцінки береться напряму зі спільної пам'яті union
        int profIdx = (p[i].spec == KN) ? 2 : (p[i].spec == IN ? 3 : 4);
        sum[profIdx] += p[i].programuvannya;
        cnt[profIdx]++;
    }

    for (int j = 0; j < 5; j++) {
        if (cnt[j] > 0)
            avg[j] = sum[j] / cnt[j];
    }
}

// Кількість студентів, які отримали з фізики оцінки "5" або "4"
int CountPhysics5or4(const Student* p, const int N) {
    int k = 0;
    for (int i = 0; i < N; i++) {
        if (p[i].fizyka == 5 || p[i].fizyka == 4) {
            k++;
        }
    }
    return k;
}

int main() {
#if defined(_WIN32)
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
#endif

    int N;
    cout << "Введіть кількість студентів N: ";
    cin >> N;

    if (N <= 0) {
        cout << "Кількість студентів має бути більшою за 0!" << endl;
        return 1;
    }

    Student* p = new Student[N];

    Create(p, N);
    Print(p, N);

    double avg[5];
    int cnt[5];
    CalcAverages(p, N, avg, cnt);

    cout << fixed << setprecision(2);
    cout << "\nСередній бал для кожного предмету:" << endl;
    for (int j = 0; j < 5; j++) {
        cout << "  " << j + 1 << ". " << subjectTitles[j] << ": ";
        if (cnt[j] > 0)
            cout << avg[j] << " (студентів: " << cnt[j] << ")" << endl;
        else
            cout << "немає студентів" << endl;
    }

    int countPhys = CountPhysics5or4(p, N);
    cout << "\nКількість студентів, які отримали з фізики оцінки '5' або '4': " << countPhys << endl;

    delete[] p;
    return 0;
}
