#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <fstream>
#include <string>


struct Item {
    int weight;
    int value;
};

using vec2d = std::vector<Item>;
using Solution = std::vector<bool>;
using SolutionList = std::vector<Solution>;
using MovesList = std::vector<int>;

//optimum globalne
Solution globalOptimum;
//ostatni punkt z którego można wykonywać obliczenia
Solution lastPoint;
//ostatni rozwazany indeks
int lastWorkPoint = -1;

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

//oblicz wartość plecaka
int calculateValue(vec2d &items, Solution &solution){
    int value = 0;
    for(int i = 0; i < solution.size(); i++){
        if(solution[i] == 1){
            value += items[i].value;
        }
    }
    return value;
}

//oblicz wage plecaka
int calculateWeight(vec2d &items, Solution &solution){
    int weight = 0;
    for(int i = 0; i<solution.size(); i++){
        if(solution[i] == 1){
            weight += items[i].weight;
        }
    }
    return weight;
}

//generuj losowe rozwiązanie
Solution generateRandomSolution(vec2d &items, int capacity){
    int weight = 0;
    Solution randomSolution;
    for(int i = 0; i < items.size(); i++){
        randomSolution.push_back(0);
    }

    for(int i = 0; i < items.size();){
        int rand = random(0, items.size()-1);
        if(randomSolution[rand] == 0){
               i++;
               if(weight + items[rand].weight < capacity){
                   randomSolution[rand] = 1;
                   weight += items[rand].weight;
               }
        }
    }

    std::cout << "Losowe rozwiazanie poczatkowe: ";
    for(int i = 0; i<randomSolution.size(); i++){
        if(randomSolution[i] == 1) std::cout<<i<<" ";
    }

    std::cout<<"\ntotal weight: " << calculateWeight(items, randomSolution)
             << " total value: " << calculateValue(items, randomSolution) <<"";

    return randomSolution;
}

SolutionList generateNeighborhood(Solution &solution, vec2d &items, int capacity){
    SolutionList neighborhood;
    int workpoint;

    //wybieram losowy przedmiot z plecaka którego nie sprawdzałem w poprzedniej iteracji programu
    while(true) {
        int rand = random(0, solution.size() - 1);
        if(solution[rand] == 1 && rand != lastWorkPoint){
            lastWorkPoint = rand;
            workpoint = rand;
            break;
        }
    }


    //geneuje sasiedztwo
    for(int i = 0; i < solution.size(); i++){
        Solution temp = solution;
        if(i != workpoint){
            temp[i] = 1;
            if(calculateWeight(items,temp) <= capacity){
                neighborhood.push_back(temp);
            }else{

            temp[workpoint] = 0;
            if(calculateWeight(items, temp) <= capacity) {
                neighborhood.push_back(temp);
                }
            }
        }
    }

    return neighborhood;
}

bool notInTabu(Solution &solution, SolutionList &tabu){
    for(int i = 0; i<tabu.size(); i++){
        if(solution == tabu[i]){
            return false;
        }
    }
    return true;
}


Solution backToPoint(SolutionList &list, MovesList &previousMoves, MovesList &itersList){
    Solution result;
    for(int i = list.size()-1; i>=0; i--){
        if(previousMoves[i] != 0){
            std::cout << "\n\nBrak rozwiazan/Wszystkie rozwiazania w Tabu";
            std::cout << "\nCofnieto sie do ostatniego punktu z iteracji:" << itersList[i] <<"\nObecne rozwiazanie: ";
            result = list[i];
            previousMoves[i]--;
            break;
        }
    }
    return result;
}

void deleteLastPoint(SolutionList &list, MovesList &previousMoves, MovesList &itersList){
    //list.erase(list.end());
    //previousMoves.erase(previousMoves.end());
    //itersList.erase(itersList.end());
    for(int i = list.size()-1;i>=0;i--){
        if (previousMoves[i] != 0){
            previousMoves[i] = 0;
            break;
        }
    }
}

void tabuSearch(vec2d &items, int capacity, int iterations, int tabuListSize){
    //tworze poczatkowe losowe rozwiazanie
    Solution currentSolution = generateRandomSolution(items, capacity);
    globalOptimum = currentSolution;
    SolutionList tabu;
    SolutionList previousPoints;
    MovesList previousMoves;
    MovesList itersList;

    bool NoReturn = false;
    for(int iter = 0; iter<iterations; iter++) {
        //tworze sasiedztwo obecnego rozwiazania
        SolutionList neighborhood = generateNeighborhood(currentSolution, items, capacity);
        //licze ilość dostępnych ruchów
        int moves = 0;

        //przeliczenie rozwiazan sasiedztwa
        int bestValue = 0;
        Solution nextSolution = currentSolution;
        for(int i = 0; i < neighborhood.size(); i++) {
            if (notInTabu(neighborhood[i], tabu)) {
                moves++;
                int neighborValue = calculateValue(items, neighborhood[i]);
                if (neighborValue > bestValue) {
                    nextSolution = neighborhood[i];
                    bestValue = neighborValue;
                    NoReturn = false;
                }
            }
        }

        //zapisz ostatni punkt z ktorego mozna kontynuowac obliczenia
        if (moves >= 2) {
            if(notInTabu(nextSolution, previousPoints)) {
                //lastPoint = nextSolution;
                previousPoints.push_back(nextSolution);
                previousMoves.push_back(moves-1);
                itersList.push_back(iter);
            }
        }


        //jezeli nie ma wyjsc, cofnij sie do ostatniego punktu z ktorego mozna bylo kontynuowac
        if (moves == 0) {
            if(NoReturn){
                deleteLastPoint(previousPoints, previousMoves, itersList);
            }
            nextSolution = backToPoint(previousPoints, previousMoves, itersList);
            NoReturn = true;
            if(nextSolution.size() == 0){
                break;
            }


            for(int i = 0; i< nextSolution.size(); i++){
                if(nextSolution[i] == 1){
                    std::cout<<i<<" ";
                }
            }

            std::cout << "\ncurrent value: " << calculateValue(items, nextSolution)
                      <<" current weight: " << calculateWeight(items, nextSolution);

            currentSolution = nextSolution;
        }

        //jezeli jest wyjscie, weź je i wypisz
        if (moves > 0) {

            if(calculateValue(items, nextSolution) > calculateValue(items, currentSolution)){
                std::cout<<"\n\nZmierzam do optimum w " << iter << " iteracji\nObecne rozwiazanie: ";
            }

            if(calculateValue(items, nextSolution) <= calculateValue(items, currentSolution)){
                std::cout<<"\n\nWychodze z optimum w " << iter << " iteracji\nObecne rozwiazanie: ";
            }

            for(int i = 0; i< nextSolution.size(); i++){
                if(nextSolution[i] == 1){
                    std::cout<<i<<" ";
                }
            }

            std::cout << "\ncurrent value: " << calculateValue(items, nextSolution)
                  <<" current weight: " << calculateWeight(items, nextSolution);


            currentSolution = nextSolution;
            tabu.push_back(currentSolution);
        }

        if(calculateValue(items, currentSolution) > calculateValue(items, globalOptimum)){
            globalOptimum = currentSolution;
        }

        //czyszczenie listy tabu
        while(tabu.size() >= tabuListSize){
            std::vector <Solution>::iterator it;
            it = tabu.begin();
            tabu.erase(it);
        }
    }
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

    int capacity;
    int iterations;
    int tabuSize;

    if(argc == 4) {
        capacity = std::stoi(argv[1]);
        iterations = std::stoi(argv[2]);
        tabuSize = std::stoi(argv[3]);
    }else{std::cout<<"NIEPRAWIDLOWE PARAMETRY WEJSCIA";return 0;}

    std::cout << "Wielkosc plecaka: " << capacity << " ilosc iteracji algorytmu: "
              << iterations << " rozmiar listy tabu: " << tabuSize;
    std::cout << "\nPlecak poczatkowy \n(waga, wartosc)\n";
    for (int i = 0; i < items.size(); i++) {
        std::cout<< i <<"# (" << items[i].weight << ", " << items[i].value << ") "<< "\n";
    }

    tabuSearch(items, capacity, iterations, tabuSize);
    std::cout << "\n\nWynik: ";
    for (int i = 0; i < globalOptimum.size(); i++) {
        if (globalOptimum[i] == 1) {
            std::cout << i << " ";
        }
    }
    std::cout<<"\nbest weight: " << calculateWeight(items, globalOptimum)
             << " best value: " << calculateValue(items, globalOptimum) <<"\n\n";


    return 0;
}
