#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <string>
#include <time.h>

struct Item {
    int weight;
    int value;
    bool in_backpack = 0;
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

//generator liczb losowych na scope (X-Y)
int random(int min, int max){
    if (min > max){
        int tmp = max;
        max = min;
        min = tmp;
    }
    int range = max - min + 1;
    int num = rand() % range + min;
    return num;
}

std::vector<Item> generateRandomSolution(const std::vector<Item>& items, int capacity) {
    std::vector<Item> solution = items;

    for (int i = 0; i < items.size(); i++){
        int j = random(0, items.size()-1);
       // std::cout<<j<<" "<<solution[j].in_backpack<<" "<< capacity<< " \n";

        if (solution[j].weight <= capacity && solution[j].in_backpack == 0) {
            solution[j].in_backpack = 1;
            capacity -= items[j].weight;
        }
    }
    return solution;
}

std::vector<Item> hillClimbingKnapsack(const std::vector<Item>& items, int capacity, int max_iterations) {
    // Wygeneruj losowe rozwiązanie początkowe
    std::vector<Item> currentSolution = generateRandomSolution(items, capacity);
    std::cout << "\nPoczatkowe rozwiazanie losowe \nID# (waga, wartosc) czy spakowane \n";
    int cout = 0;
    for (const auto& item : currentSolution) {
        std::cout << cout <<"# (" << item.weight << ", " << item.value << ") " << item.in_backpack << "\n";
        cout++;
    }
    std::cout << "Waga: " << computeWeight(currentSolution) << " wartosc: " << computeValue(currentSolution) << "\n";


    bool improved = false;
    int iteration = 0;
    while (iteration!=max_iterations) {
        iteration++;
        if (improved) {

            std::cout << "\nW " << iteration << " iteracji hillClimbing \n(waga, wartosc) czy spakowane \n";
            int cout = 0;
            for (const auto &item: currentSolution) {
                std::cout << cout <<"# (" << item.weight << ", " << item.value << ") " << item.in_backpack << "\n";
                cout++;
            }
            std::cout << "Waga: " << computeWeight(currentSolution) << " wartosc: " << computeValue(currentSolution)
                      << "\n";

        }

        std::vector<Item> testingSolution = currentSolution;
        improved = false;
        // Wygeneruj sąsiednie rozwiązania przez zamianę przedmiotów i sprawdź czy jest lepsze
        bool checked = 1;
        while(checked){
            int i = random(0, currentSolution.size()-1);
            int j = random(0, testingSolution.size()-1);
            if (currentSolution[i].in_backpack == 1
                && testingSolution[j].in_backpack == 0){
                testingSolution[j].in_backpack = 1;
                if (computeWeight(testingSolution) <= capacity && computeValue(testingSolution) > computeValue(currentSolution)){
                    checked = 0;
                    currentSolution = testingSolution;
                    improved = true;
                    std::cout<<"\n IMPROVEDDODANO OBIEKT #" << j;

                } else if (computeWeight(testingSolution) > capacity){
                    testingSolution[i].in_backpack = 0;
                    if(computeWeight(testingSolution) <= capacity && computeValue(testingSolution) > computeValue(currentSolution)){
                        checked = 0;
                        currentSolution = testingSolution;
                        improved = true;
                        std::cout<<"\nIMPROVED! ZAMIENIONO OBIEKT #" << i << " Z OBIEKTEM #" << j;
                    }
                }
                checked = 0;
            }
        }

    }

    return currentSolution;
}

int main(int argc, char* argv[]){
    srand((unsigned)time(NULL));

    //deklaracja pojemnosci plecaka
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



    //deklaracja ilosci iteracji algorytmu
    int iterations = 20;


    /*
    std::cout << "Podaj pojemnosc plecaka";
    std::cin >> capacity;
    std::cout << "Podaj ilosc iteracji algorytmu wspinaczkowego: ";
    std::cin >> iterations;
*/
    if(argc == 3){
        int count = 0;
        capacity = std::stoi(argv[1]);
        iterations = std::stoi(argv[2]);
        std::cout << "Wielkosc plecaka: " << capacity << " ilosc iteracji algorytmu: " << iterations;
        std::cout << "\nPlecak poczatkowy \n(waga, wartosc) czy spakowane \n";
        for (const auto& item : items) {
            std::cout<< count <<"# (" << item.weight << ", " << item.value << ") " << item.in_backpack << "\n";
            count++;
        }

    }else{
        std::cout<<"NIEPRAWIDLOWE PARAMETRY WEJSCIA";
        return 0;
    }

    std::vector<Item> solution = hillClimbingKnapsack(items, capacity, iterations);

}