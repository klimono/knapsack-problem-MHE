#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <string>
#include <time.h>
#include <functional>

struct Item {
    int weight;
    int value;
    bool in_backpack = 0;
};

using vec2d = std::vector<Item>;
std::random_device rd;
std::mt19937 rgen(rd());
int previousItem = -1;

// Funkcja obliczająca wartość rozwiązania (suma wartości przedmiotów)
int computeValue(vec2d &solution) {
    int value = 0;
    for (auto &item: solution) {
        if(item.in_backpack == 1) value += item.value;
    }
    return value;
}

// Funkcja obliczająca wartość rozwiązania (suma wag przedmiotów)
int computeWeight(vec2d &solution) {
    int weight = 0;
    for (auto &item: solution) {
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

//generator losowego rozwiazania poczatkowego
vec2d generateRandomSolution(vec2d &items, int capacity) {
    std::vector<Item> solution = items;

    std::cout << "Losowe rozwiazanie poczatkowe: ";
    for (int i = 0; i < items.size(); i++){
        int j = random(0, items.size()-1);
        if (solution[j].weight <= capacity && solution[j].in_backpack == 0) {
            solution[j].in_backpack = 1;
            capacity -= items[j].weight;
        }
    }
    for (int i = 0; i < items.size(); i++){
        if(solution[i].in_backpack == 1){
            std::cout<<i<<" ";
        }
    }
    std::cout<<"\ntotal weight: " << computeWeight(solution)
             << " total value: " << computeValue( solution) <<"\n\n";
    return solution;
}

vec2d generateNeighbour(vec2d &items, int capacity){
    vec2d neighbour = items;
    int selected_item = 0;
    int switching_item = 0;

    //wybranie przedmiotu startowego

    while(true){
        int rand = random(0, items.size()-1);
        //std::cout<<items[j].in_backpack;
        if(items[rand].in_backpack == 1 && rand != previousItem){
            selected_item = rand;
            previousItem = rand;
            break;
        }
    }
    //wybranie przedmiotu do zamiany
    while(true){
        int rand = random(0, items.size()-1);
        if(items[rand].in_backpack == 0){
            switching_item = rand;
            break;
        }
    }

    //sprawdzenie dodania przedmiotu
    neighbour[switching_item].in_backpack = 1;
    if(computeWeight(neighbour) <= capacity){
        return neighbour;
    }

    //sprawdzenie zamiany przedmiotów
    neighbour[selected_item].in_backpack = 0;
    if(computeWeight(neighbour) <= capacity){
        return neighbour;
    }
    return items;
}

vec2d sim_annealing(vec2d &items, int capacity, int max_iterations, std::function<double(int)> T){
    //wygeneruj losowe rozwiazanie poczatkowe
    vec2d currentSolution = generateRandomSolution(items, capacity);
    vec2d bestSolution = currentSolution;

    //glowna petla
    for(int iters = 1; iters <= max_iterations; iters++){
        bool annealed = false;

        vec2d nextSolution = generateNeighbour(currentSolution, capacity);


        int bestFitness = computeValue(bestSolution);
        int currentFitness = computeValue(currentSolution);
        int nextFitness = computeValue(nextSolution);

        if(nextFitness > currentFitness){
            currentSolution = nextSolution;
            if(nextFitness > bestFitness){
                bestSolution = nextSolution;
                std::cout<<"\nNowe najlepsze rozwiazanie";
            }

            std::cout<<"\nProgress! \nw " << iters << " iteracji programu";
            std::cout<<"\nAktualne rozwiazanie: ";
            for (int i = 0; i < currentSolution.size(); i++){
                if(currentSolution[i].in_backpack == 1){
                    std::cout<<i<<" ";
                }
            }
            std::cout<<"\ntotal weight: " << computeWeight(currentSolution)
                     << " total value: " << computeValue( currentSolution) <<"\n";
        }else if(nextFitness < currentFitness){
            std::uniform_real_distribution<double> u(0.0,1.0);
            if (u(rgen) < std::exp(-std::abs(nextFitness - currentFitness)/T(iters))) {
                currentSolution = nextSolution;

                std::cout << "\nRegress! \nw " << iters << " iteracji programu";
                std::cout << "\nAktualne rozwiazanie: ";
                for (int i = 0; i < currentSolution.size(); i++) {
                    if (currentSolution[i].in_backpack == 1) {
                        std::cout << i << " ";
                    }
                }
                std::cout << "\ntotal weight: " << computeWeight(currentSolution)
                          << " total value: " << computeValue(currentSolution) << "\n";

            }
        }else {
            std::cout<<"\nstatus quo w " << iters << " iteracji\n" ;
        }


    }

    return bestSolution;
}

int main(int argc, char* argv[]){
    srand((unsigned)time(NULL));
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
    if(argc == 3) {
        int capacity = std::stoi(argv[1]);
        int iterations = std::stoi(argv[2]);

        std::cout << "Wielkosc plecaka: " << capacity << " ilosc iteracji algorytmu: "
                  << iterations;
        std::cout << "\nPlecak poczatkowy \n(waga, wartosc)\n";
        for (int i = 0; i < items.size(); i++) {
            std::cout << i << "# (" << items[i].weight << ", " << items[i].value << ") " << "\n";
        }
        vec2d solution = sim_annealing(items, capacity, iterations, [](int k){return 1000/k;});

        std::cout<<"\nNajlepsze znalezione rozwiazanie: ";
        for (int i = 0; i < solution.size(); i++){
            if(solution[i].in_backpack == 1){
                std::cout<<i<<" ";
            }
        }
        std::cout<<"\ntotal weight: " << computeWeight(solution)
                 << " total value: " << computeValue( solution) <<"\n\n";

    }else{
        std::cout<<"NIEPRAWIDŁOWE PARAMETRY WEJŚCIA";
    }
}