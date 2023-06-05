#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <string>
#include <time.h>

struct Item{
    int weight;
    int value;
    bool in_backpack = 0;
};

using vec2d = std::vector<Item>;
using Solution = std::vector<bool>;
using Neighborhood = std::vector<Solution>;
int previousItem = -1;


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

//funkcja sprawdzajaca poprawnosc plecaka i zwracajaca jego wartosc
int calculateKnapsack(vec2d items, Solution &solution, int capacity) {
    int value = 0;
    int weight = 0;

    for (int i = 0; i < solution.size(); ++i) {
        if (solution[i] == 1) {
            value += items[i].value;
            weight += items[i].weight;
        }
    }

    // blad, przedmioty nie mieszcza sie w plecaku
    if (weight > capacity) {
        return 0;
    }

    return value;
}

// Funkcja obliczająca wartość rozwiązania (suma wartości przedmiotów)
int computeValue(Solution &solution, vec2d &items) {
    int value = 0;
    for(int i = 0; i<solution.size(); i++){
        if(solution[i]==1){
            value += items[i].value;
        }
    }
    return value;
}

// Funkcja obliczająca wartość rozwiązania (suma wag przedmiotów)
int computeWeight(Solution &solution, vec2d &items) {
    int weight = 0;
    for(int i = 0; i<solution.size(); i++){
        if(solution[i]==1){
            weight += items[i].weight;
        }
    }
    return weight;
}

Solution generateRandomSolution(vec2d &items, int capacity){
    vec2d solution = items;
    Solution result;

    for (int i = 0; i < items.size(); i++){
        int j = random(0, items.size()-1);

        if(solution[j].in_backpack == 0){
            if (solution[j].weight <= capacity) {
                solution[j].in_backpack = 1;
                capacity -= items[j].weight;
            }
        }else i--;
    }

    std::cout << "Losowe rozwiazanie poczatkowe: ";
    for(int i = 0; i<solution.size(); i++){
        result.push_back(solution[i].in_backpack);
        if(solution[i].in_backpack == 1){
            std::cout<<i<<" ";
        }
    }

    std::cout<<"\ntotal weight: " << computeWeight(result, items)
             << " total value: " << computeValue(result, items) <<"\n\n";
    return result;

}

Neighborhood generateNeighborhood(Solution &currentSolution, vec2d &items, int capacity){
    Neighborhood neighborhood;
    int selected_item = 0;
    int item_to_switch = 0;




    //wybranie losowego przedmiotu znajdujacego sie w plecaku
    while(true){
        int rand = random(0, currentSolution.size()-1);
        if (currentSolution[rand] == 1 && rand != previousItem){
            item_to_switch = rand;
            break;
        }
    }

    //generowanie losowych sasiadów
    for(int i = 0; i < currentSolution.size(); i++){
        if(currentSolution[i] == 0 && i != previousItem) {
            Solution temp = currentSolution;
            temp[i] = 1;

            //DYLEMAT!!!
            //czy losowi sasiedzi powinni uwzgledniac tez dodanie przedmiotu jezeli bedzie to korzystne?
            //inaczej liczba przedmiotów w plecaku będzie stała względem ilości w rozwiazaniu wygenerowanym losowo

            /*
            temp[item_to_switch] = 0;
            if (calculateKnapsack(items, temp, capacity) != 0) {
                neighboursSolutions.push_back(temp);
            }
             */


            if (calculateKnapsack(items, temp, capacity) != 0) {
                neighborhood.push_back(temp);
            }

            else {
                temp[item_to_switch] = 0;
                if (calculateKnapsack(items, temp, capacity) != 0) {   //waznee
                    neighborhood.push_back(temp);
                }
            }
        }
    }

    return neighborhood;

}

void hillClimbingKnapsack(vec2d &items, int capaciy, int maxIterations){
    //utworzenie losowego rozwiazania poczatkowego
    Solution currentSolution = generateRandomSolution(items, capaciy);

    //glowna petla
    for(int iters = 0; iters < maxIterations; iters++){
        Neighborhood neighborhood = generateNeighborhood(currentSolution, items, capaciy);

        Solution bestSolution = currentSolution;
        for(auto& neighbor: neighborhood){
            if(computeValue(neighbor, items) > computeValue(bestSolution, items)){
                bestSolution = neighbor;
            }
        }

        if(computeValue(bestSolution, items) > computeValue(currentSolution, items)){
            currentSolution = bestSolution;
            std::cout<<"\nImproved! \nw " << iters << " iteracji programu\nAktualne rozwiazanie: ";
            for (int i = 0; i < currentSolution.size(); i++){
                if(currentSolution[i] == 1){
                    std::cout<<i<<" ";
                }
            }
            std::cout<<"\ntotal weight: " << computeWeight(currentSolution, items)
                     << " total value: " << computeValue( currentSolution, items) <<"\n\n";

        }
    }
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
        hillClimbingKnapsack(items, capacity, iterations);

    }else{
        std::cout<<"NIEPRAWIDŁOWE PARAMETRY WEJŚCIA";
    }
}