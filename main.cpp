#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

void selectionSort(vector<int>& a) {
    int n = static_cast<int>(a.size());
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (a[j] < a[minIdx]) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            swap(a[i], a[minIdx]);
        }
    }
}

void shellSort(vector<int>& a) {
    int n = static_cast<int>(a.size());
    int gap = 1;

    while (gap <= (n - 2) / 3) {
        gap = 3 * gap + 1;
    }

    while (gap > 0) {
        for (int i = gap; i < n; i++) {
            int key = a[i];
            int j = i;
            while (j >= gap && a[j - gap] > key) {
                a[j] = a[j - gap];
                j -= gap;
            }
            a[j] = key;
        }
        gap = (gap - 1) / 3;
    }
}

double timeSort(vector<int>& a, void (*sortFn)(vector<int>&)) {
    auto t0 = chrono::steady_clock::now();
    sortFn(a);
    auto t1 = chrono::steady_clock::now();
    return chrono::duration<double, milli>(t1 - t0).count();
}

int main() {
    const int sizes[] = {10000, 20000, 30000, 40000, 50000};
    const int sets = 5;
    const int runs = 5;
    const unsigned int seed = 2026;
    mt19937 rng(seed);
    uniform_int_distribution<int> dist(-1000000, 1000000);

    ofstream csv("rezultatai.csv");
    if (!csv) {
        cerr << "Nepavyko sukurti rezultatai.csv.\n";
        return 1;
    }
    csv << "n,aibe,testas,isrinkimas_ms,selas_ms\n";
    csv << fixed << setprecision(6);
    cout << fixed << setprecision(3);
    cout << "11 variantas. Isrinkimo ir Selo rusiavimas\n";
    cout << "Sekla: " << seed << ". Laikai pateikiami milisekundemis.\n";

    int tests = 0;
    for (int n : sizes) {
        double selSum = 0.0;
        double shSum = 0.0;
        cout << "\nn = " << n << '\n';

        for (int setId = 1; setId <= sets; setId++) {
            vector<int> data(n);
            for (int& x : data) {
                x = dist(rng);
            }

            vector<int> ref = data;
            sort(ref.begin(), ref.end());

            for (int run = 1; run <= runs; run++) {
                vector<int> sel = data;
                vector<int> sh = data;
                double selMs;
                double shMs;

                if (tests % 2 == 0) {
                    selMs = timeSort(sel, selectionSort);
                    shMs = timeSort(sh, shellSort);
                } else {
                    shMs = timeSort(sh, shellSort);
                    selMs = timeSort(sel, selectionSort);
                }

                if (sel != ref || sh != ref) {
                    cerr << "Rusiavimo klaida. n = " << n
                         << ", aibe = " << setId << ", testas = " << run << '\n';
                    return 1;
                }

                csv << n << ',' << setId << ',' << run << ','
                       << selMs << ',' << shMs << '\n';
                selSum += selMs;
                shSum += shMs;
                tests++;
            }
            cout << "  Aibe " << setId << ": patikrinti visi "
                 << runs << " testai" << endl;
        }

        int perSize = sets * runs;
        cout << "  Vidurkiai: isrinkimo " << selSum / perSize
             << " ms, Selo " << shSum / perSize << " ms\n";
    }

    csv.close();
    if (!csv) {
        cerr << "Nepavyko issaugoti rezultatu.\n";
        return 1;
    }
    cout << "\nAtlikta testu: " << tests
         << ". Patikrinta rusiavimu: " << 2 * tests << ".\n";
    cout << "Rezultatai issaugoti: rezultatai.csv\n";
    return 0;
}
