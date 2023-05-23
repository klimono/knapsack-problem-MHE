#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <string>

struct Item {
    int weight;
    int value;
    bool in_backpack = 0;
    double ratio = (double(value)/double(weight));
};

// Funkcja obliczająca wartość rozwiązania (suma wartości przedmiotów)
int computeValue(const std::vector<Item>& solution) {
    int value = 0;
    for (const auto &item: solution) {
        if(item.in_backpack == 1) value += item.value;
    }
    return value;
}

// Funkcja obliczająca wartość rozwiązania (suma wartości przedmiotów)
int computeWeight(const std::vector<Item>& solution) {
    int weight = 0;
    for (const auto &item: solution) {
        if(item.in_backpack == 1) weight += item.weight;
    }
    return weight;
}

bool compareItems(Item i1, Item i2){
    return (i1.ratio > i2.ratio);
}

std::vector<Item> sortByRatio(const std::vector<Item>& items) {
    std::vector<Item> solution = items;
    std::sort(solution.begin(), solution.end(), compareItems);
    return solution;
}

std::vector<Item> hillClimbingKnapsack(const std::vector<Item>& items, int capacity) {
    // Wygeneruj losowe rozwiązanie początkowe
    std::vector<Item> currentSolution = items;
    currentSolution = sortByRatio(currentSolution);



    std::cout << "\nPrzedmioty posortowane wzgledem wspolczynnika ratio \nID# (waga, wartosc) czy spakowane | ratio \n";
    int cout = 0;
    for (const auto& item : currentSolution) {
        std::cout << cout <<"# (" << item.weight << ", " << item.value << ") " << item.in_backpack << " | "<< item.ratio << "\n";
        cout++;
    }

    for (auto& item: currentSolution){
        if (item.weight <= capacity){
            item.in_backpack = 1;
            capacity -= item.weight;
        }
    }

    std::cout << "\nPrzedmioty spakowane wzgledem najwiekszego wspolczynnika ratio \nID# (waga, wartosc) czy spakowane | ratio \n";
    cout = 0;
    for (const auto& item : currentSolution) {
        std::cout << cout <<"# (" << item.weight << ", " << item.value << ") " << item.in_backpack << " | "<< item.ratio << "\n";
        cout++;
    }
    std::cout << "Waga: " << computeWeight(currentSolution) << " wartosc: " << computeValue(currentSolution) << "\n";

    return currentSolution;
}

int main(int argc, char* argv[]){

    //deklaracja pojemnosci plecaka aktualnie okreslana parametrem wejscia
    int capacity = 15;

    //deklaracja wektora przedmiotow
    std::vector<Item> items;

    //otworzenie plików z przedmiotami i wrzucenie ich do wektora
    std::ifstream file_w("./dataset/weights.txt");
    std::ifstream file_v("./dataset/values.txt");

    std::string line_w;
    std::string line_v;

    while (!file_w.eof()){
        std::getline(file_w,line_w);
        int w = stoi(line_w);

        std::getline(file_v,line_v);
        int v = stoi(line_v);

        items.push_back({w, v});
    }


    file_w.close();
    file_v.close();



    /*
    std::cout << "Podaj pojemnosc plecaka";
    std::cin >> capacity;
    std::cout << "Podaj ilosc iteracji algorytmu wspinaczkowego: ";
    std::cin >> iterations;
*/
    if(argc == 2){
        int count = 0;
        capacity = std::stoi(argv[1]);

        std::cout << "Wielkosc plecaka: " << capacity;
        std::cout << "\nPlecak poczatkowy \n(waga, wartosc) czy spakowane \n";
        for (const auto& item : items) {
            std::cout<< count <<"# (" << item.weight << ", " << item.value << ") " << item.in_backpack << "\n";
            count++;
        }

    }else{
        std::cout<<"NIEPRAWIDLOWE PARAMETRY WEJSCIA";
        return 0;
    }

    std::vector<Item> solution = hillClimbingKnapsack(items, capacity);

}