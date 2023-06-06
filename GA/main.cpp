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
using Genotype = std::vector<bool>;
using Population = std::vector<Genotype>;
using Roulette = std::vector<int>;

vec2d items;

int capacity;

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

//oblicz wartosc
int calculateValue(Genotype solution){
    int value = 0;
    for(int i = 0; i < solution.size(); i++){
        if(solution[i] == 1){
            value += items[i].value;
        }
    }
    return value;
}

//oblicz wage
int calculateWeight(Genotype solution){
    int weight = 0;
    for(int i = 0; i < solution.size(); i++){
        if(solution[i] == 1){
            weight += items[i].weight;
        }
    }
    return weight;
}

int populationFitness(Population &population){
    int fitness = 0;
    for(auto& solution: population) {
        fitness += calculateValue(solution);
    }
    return fitness;
}

Genotype generateRandomGenotype(){
    int weight = 0;
    Genotype randomSolution;
    for(int i = 0; i < items.size(); i++){
        randomSolution.push_back(0);
    }

    for(int i = 0; i < items.size();){
        int rand = random(0, items.size()-1);
        if(randomSolution[rand] == 0){
            i++;
            if(weight + items[rand].weight <= capacity){
                randomSolution[rand] = 1;
                weight += items[rand].weight;
            }
        }
    }
    return randomSolution;
}

Population generateInitialPopulation(int populationSize){
    Population initialPopulation;
    for(int i = 0; i < populationSize; i++){
        Genotype temp = generateRandomGenotype();
        initialPopulation.push_back(temp);
    }

    std::cout<<"\nPopulacja poczatkowa : ";
    for(int i = 0; i < initialPopulation.size(); i++){
        std::cout<<"\n";
        std::cout<<"#"<<i<< " Value: " << calculateValue(initialPopulation[i])<<
                 "\t Weight: " <<calculateWeight(initialPopulation[i]) << "\t";
        for(int j = 0; j < initialPopulation[i].size(); j++){
            if(initialPopulation[i][j] == 1){
                std::cout<<j<<" ";
            }
        }

    }

    return initialPopulation;
}

//mutacja korzystna wybierająca najlepszego sąsiada rozwiązania
Genotype mutationOne(Genotype &rabbit){
    Genotype result;

    return result;
}

//mutacja losowa wybierająca dowolnego sąsiada rozwiązania
Genotype mutationTwo(Genotype &rabbit){
    Genotype result;

    return result;
}

//krzyżowanie punktowe
Population crossingSolutionOne(Genotype &Adam, Genotype &Eve) {
    Population offspring;
    Genotype Kain;
    Genotype Abel;
    int locus = random(1, Adam.size() - 2);
    for (int i = 0; i < Adam.size(); i++) {
        if (i <= locus) {
            if (Adam[i] == 1 && calculateWeight(Abel) + items[i].weight <= capacity) {
                Abel.push_back(Adam[i]);
            }else{
                Abel.push_back(0);
            }

            if (Eve[i] == 1 && calculateWeight(Kain) + items[i].weight <= capacity) {
                Kain.push_back(Eve[i]);
            }else{
                Kain.push_back(0);
            }
        }
        if (i > locus) {
            if (Adam[i] == 1 && calculateWeight(Kain) + items[i].weight <= capacity){
                Kain.push_back(Adam[i]);
            }else{
                Kain.push_back(0);
            }

            if (Eve[i] == 1 && calculateWeight(Abel) + items[i].weight <= capacity){
                Abel.push_back(Eve[i]);
            }else{
                Abel.push_back(0);
            }
        }
    }
    offspring.push_back(Abel);
    offspring.push_back(Kain);
    return offspring;
}

//krzyżowanie jednorodne
Population crossingSolutionTwo(Genotype &Adam, Genotype &Eve){
    Population offspring;
    Genotype Kain;
    Genotype Abel;

    //Abel
    for(int i = 0; i < Adam.size(); i++){
        bool fortune = random(0, 1);
        switch (fortune) {
            case 0:
                if(Adam[i] == 1 && calculateWeight(Abel)+items[i].weight <= capacity) {
                    Abel.push_back(Adam[i]);
                    break;
                }else{
                    Abel.push_back(0);
                    break;
                }
            case 1:
                if(Eve[i] == 1 && calculateWeight(Abel)+items[i].weight <= capacity) {
                    Abel.push_back(Eve[i]);
                    break;
                }else{
                    Abel.push_back(0);
                    break;
                }
        }
    }
    offspring.push_back(Abel);

    //Kain
    for(int i = 0; i < Adam.size(); i++){
        bool fortune = random(0, 1);
        switch (fortune) {
            case 0:
                if(Adam[i] == 1 && calculateWeight(Kain)+items[i].weight <= capacity) {
                    Kain.push_back(Adam[i]);
                    break;
                }else{
                    Kain.push_back(0);
                    break;
                }
            case 1:
                if(Eve[i] == 1 && calculateWeight(Kain)+items[i].weight <= capacity) {
                    Kain.push_back(Eve[i]);
                    break;
                }else{
                    Kain.push_back(0);
                    break;
                }
        }
    }
    offspring.push_back(Kain);

    return offspring;
}

//utworzenie ruletki z podanej populacji
Roulette generateRoulette(Population &population){
    Roulette populationRoulette;
    for(int i = 0; i < population.size(); i++){
        int value = calculateValue(population[i]);
        for(int j = 0; j < value; j++){
               populationRoulette.push_back(i);
        }
    }
    return populationRoulette;
}

Population reproduction(Roulette populationRoulette, Population &oldGeneration, int populationSize){
    //nastepne pokolenie
    Population nextGeneration;

    //petla dobierajaca pary z obecnego pokolenia
    for(int i = 0; i<oldGeneration.size(); i+=2){
        //osobnik bez pary nie bierze udziału w reprodukcji
        if(i == oldGeneration.size()-1) {
            break;
        }

        //wybranie Adama i Ewy przez wylosowanie osobników z ruletki
        Genotype Adam = oldGeneration[random(0, populationRoulette.size()-1)];
        Genotype Eve = oldGeneration[random(0, populationRoulette.size()-1)];

        //losowanie metody krzyżowania spośród dwóch dostępnych
        bool crossingMethod = random(0, 1);

        //potomstwo
        Population offspring;

        //zastosowanie krzyżowania
        switch(crossingMethod){
            case 0:
                offspring = crossingSolutionOne(Adam, Eve);
                break;
            case 1:
                offspring = crossingSolutionTwo(Adam, Eve);
                break;
        }

        //dodanie potomstwa do populacji nastepnego pokolenia
        for(const auto & i : offspring){
            nextGeneration.push_back(i);
        }
    }

    //mutacja z prawdopodobieństwem 1/population_size * 1/2
    int mutationChance = random(0, 3);
    int mutationGenotype = random(0, nextGeneration.size()-1);
    switch (mutationChance) {
        case 0:
            //mutacja korzystna
            nextGeneration[mutationGenotype] = mutationOne(nextGeneration[mutationGenotype]);
            break;
        case 1:
            //mutacja losowa
            nextGeneration[mutationGenotype] = mutationTwo(nextGeneration[mutationGenotype]);
            break;
        default:
            break;
    }

    return nextGeneration;
}

void geneticAlghoritm(int populationSize, int iterations){
    //deklaracja początkowej populacji
    Population currentPopulation = generateInitialPopulation(populationSize);
    //deklaracja elity
    Population elite;

    //warunek głównej pętli !!!!
    for(int i = 0; i < iterations; i++){
        //tworze ruletkę dla obecnej populacji
        Roulette roulette = generateRoulette(currentPopulation);

        //tworze nowe pokolenie populacji
        Population nextPopulation = reproduction(roulette, currentPopulation, populationSize);

        //dodaje najlepsze rozwiazania z nowej populacji do elity i aktualizuje elite

        //wypisuje nowe pokolenie

        //akceptuje nowe pokolenie jako obecna populacja


    }
}

int main() {
    srand((unsigned)time(NULL));

    //deklaracja wektora przedmiotow

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

    capacity = 50;
    int populationSize = 14;
    int iterations = 50;


    std::cout << "Wielkosc plecaka: " << capacity;
    std::cout << "\nPlecak poczatkowy \n(waga, wartosc)\n";
    for (int i = 0; i < items.size(); i++) {
        std::cout<< i <<"# (" << items[i].weight << ", " << items[i].value << ") "<< "\n";
    }





}
