#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <fstream>
#include <string>


struct Item {
    int value;
    int weight;
    bool in_backpack = 0;
};

using vec2d = std::vector<Item>;
using tabuSolution = std::vector<bool>;
using tabuList = std::vector<tabuSolution>;

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
int calculateKnapsack(vec2d items, tabuSolution &solution, int capacity) {
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
int computeValue(tabuSolution &solution, vec2d &items) {
    int value = 0;
    for(int i = 0; i<solution.size(); i++){
        if(solution[i]==1){
            value += items[i].value;
        }
    }
    return value;
}

// Funkcja obliczająca wartość rozwiązania (suma wag przedmiotów)
int computeWeight(tabuSolution &solution, vec2d &items) {
    int weight = 0;
    for(int i = 0; i<solution.size(); i++){
        if(solution[i]==1){
            weight += items[i].weight;
        }
    }
    return weight;
}

//utworzenie losowego rozwiazania
tabuSolution generateRandomSolution(vec2d &items, int capacity){
    vec2d solution = items;
    tabuSolution result;

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



//generowanie sasiedztwa dla podanego rozwiazania
tabuList generateNeighbors(tabuSolution &solution, vec2d &items, int capacity){
    tabuList neighboursSolutions;
    int item_to_switch = 0;

    //wybranie losowego przedmiotu znajdujacego sie w plecaku
    while(item_to_switch == 0){
        int i = random(0, solution.size()-1);
        if (solution[i] == 1){
            item_to_switch = i;
        }
    }

    //generowanie losowych sasiadów
    for(int i = 0; i < solution.size(); i++){
        if(solution[i] == 0) {
            tabuSolution temp = solution;
            temp[i] = 1;

            //DYLEMAT!!!
            //czy losowi sasiedzi powinni uwzgledniac tez dodanie przedmiotu jezeli bedzie to korzystne?
            //inaczej liczba przedmiotów w plecaku będzie stała względem ilości w rozwiazaniu wygenerowanym losowo

            /*
            temp[item_to_switch] = 0;
             tabu.push_back(temp);
            */


            if (calculateKnapsack(items, temp, capacity) != 0) {
                neighboursSolutions.push_back(temp);
            } else {

                temp[item_to_switch] = 0;
                neighboursSolutions.push_back(temp);
            }



        }
    }

    return neighboursSolutions;
}


bool notInTabu(tabuSolution &solution, tabuList &list){
    for(auto& item: list){
        if(item == solution){
            return false;
        }
    }
    return true;
}

// glowna funkcja tabu
tabuSolution tabuSearch(vec2d &items, int capacity, int iterations, int tabuListSize) {
    //utworzenie losowego rozwiazania poczatkowego
    tabuSolution currentSolution = generateRandomSolution(items, capacity);

    //deklaracja wektora przechowujacego najlepsze rozwiazanie
    tabuSolution bestSolution = currentSolution;

    //zliczenie wartości poczatkowego plecaka losowego
    int currentFitness = calculateKnapsack(items, currentSolution, capacity);

    //deklaracja zmiennej przechowujacej wartosc najlepzego rozwiazania
    int bestFitness = currentFitness;

    //deklaracja tablicy tabu
    tabuList tabu_list;

    //glowna petla
    for (int iter = 0; iter < iterations; ++iter) {
        //wygenerowanie listy sasiedztwa do listy tabu
        tabuList neighbors = generateNeighbors(currentSolution, items, capacity);

        //deklaracja zmiennych przechowujacych nastepne rozwiazania
        tabuSolution nextSolution;
        int nextFitness = 0;

        //sprawdzenie rozwiazan sasiedztwa
        for (auto& neighbor : neighbors) {

            //oblcizanie wartosci kazdego sasiada
            int neighborFitness = calculateKnapsack(items, neighbor, capacity);

            //znajdz najlepsze rozwiązanie i sprawdz czy nie jest w tabu
            if (neighborFitness > nextFitness && notInTabu(neighbor, tabu_list)) {
                nextSolution = neighbor;
                nextFitness = neighborFitness;

            }
        }

        //przerwanie petli w sytuacji w ktorej realne najlepsze sasiedztwo nie istnieje
        if (nextFitness == 0) {
            break;
        }

        //przypisz i wypisz rozwiazanie jezeli jest ono lepsze
        currentSolution = nextSolution;
        currentFitness = nextFitness;
        if (currentFitness > bestFitness) {
            bestSolution = currentSolution;
            bestFitness = currentFitness;

            std::cout<<"Rozwiazanie w " << iter << " iteracji: ";

            for(int i = 0; i<bestSolution.size(); i++){
                if(bestSolution[i] == 1){
                    std::cout<<i<<" ";
                }
            }

            std::cout<<"\ntotal weight: " << computeWeight(bestSolution, items)
                     << " total value: " << computeValue(bestSolution, items) <<"\n\n";
        }

        //dodaj rozwiazanie do tabu
        tabu_list.push_back(currentSolution);

        //wyczysc liste tabu jezeli jest ona za duza
        if (tabu_list.size() > tabuListSize) {
            tabu_list.clear();
        }
    }
    return bestSolution;
}

int main(int argc, char* argv[]){

    srand((unsigned)time(NULL));

    //deklaracja wektora przedmiotow
    vec2d items;

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

    int capacity = 50;
    int iterations = 400;
    int tabuSize = 50;

    if(argc == 4){
        int count = 0;
        capacity = std::stoi(argv[1]);
        iterations = std::stoi(argv[2]);
        tabuSize = std::stoi(argv[3]);
        std::cout << "Wielkosc plecaka: " << capacity << " ilosc iteracji algorytmu: "
        << iterations << " rozmiar listy tabu: " << tabuSize;
        std::cout << "\nPlecak poczatkowy \n(waga, wartosc)\n";
        for (int i = 0; i < items.size(); i++) {
            std::cout<< i <<"# (" << items[i].weight << ", " << items[i].value << ") "<< "\n";
            }
        }else{
        std::cout<<"NIEPRAWIDLOWE PARAMETRY WEJSCIA";
        return 0;
    }


    //deklaracja i wywolanie wektora rozwiazania
    tabuSolution solution = tabuSearch(items, capacity, iterations, tabuSize);

    std::cout << "Wynik: ";
    for (int i = 0; i < solution.size(); ++i) {
        if (solution[i]) {
            std::cout << i + 1 << " ";
        }
    }
    std::cout<<"\ntotal weight: " << computeWeight(solution, items)
             << " total value: " << computeValue(solution, items) <<"\n\n";

    std::cout << "\n";

    return 0;
}
