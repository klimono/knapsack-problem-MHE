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
int wantedValue;

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

int findBest(Population &population){
    int bestValue = 0;
    for(const auto & i : population){
        if(calculateValue(i) > bestValue){
            bestValue = calculateValue(i);
        }
    }
    return bestValue;
}

void printResult(Genotype &gen) {
    std::cout << "\nValue: " << calculateValue(gen) <<
              "\t Weight: " << calculateWeight(gen) << "\t";
    for (int i = 0; i < gen.size(); i++) {
        if (gen[i] == 1) {
            std::cout << i << " ";
        }
    }
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

    std::cout<<"\nPopulacja poczatkowa: ";
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

//generowanie sasiedztwa
Population generateNeighborhood(Genotype &genotype){
    Population neighborhood;
    int workpoint;

    //wybieram punkt roboczy dla sasiedztwa
    while(true){
        int rand = random(0, genotype.size()-1);
        if(genotype[rand] == 1){
            workpoint = rand;
            break;
        }
    }

    //generuje sasiedztwo
    for(int i = 0; i < genotype.size(); i++){
        Genotype temp = genotype;
        if(i != workpoint){
            temp[i] = 1;
            if(calculateWeight(temp) <= capacity){
                neighborhood.push_back(temp);
            }else {
                temp[workpoint] = 0;
                if(calculateWeight(temp) <= capacity){
                    neighborhood.push_back(temp);
                }
            }
        }
    }
    return neighborhood;
}

//mutacja korzystna wybierająca najlepszego sąsiada rozwiązania
Genotype mutationOne(Genotype &rabbit){
    Genotype result = rabbit;
    Population neighborhood = generateNeighborhood(rabbit);
    int bestValue = 0;
    for(int i = 0; i < neighborhood.size(); i++){
        int neighborValue = calculateValue(neighborhood[i]);
        if(neighborValue > bestValue){
            result = neighborhood[i];
            bestValue = neighborValue;
        }
    }
    return result;
}

//mutacja losowa wyrzucająca losowy przedmiot
/*
Genotype mutationTwo(Genotype &rabbit){
    Genotype result = rabbit;
    int workpoint;

    while(true){
     int rand = random(0, rabbit.size()-1);
     if(rabbit[rand] == 1){
         workpoint = rand;
         break;
     }
    }
    result[workpoint] = 0;


    return result;
}*/

//mutacja zmieniająca genotyp na nowy, losowy
Genotype mutationTwo() {
    return generateRandomGenotype();
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

int roll(Roulette &populationRoulette){
    return populationRoulette[random(0, populationRoulette.size()-1)];
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
        Genotype Adam = oldGeneration[roll(populationRoulette)];
        Genotype Eve = oldGeneration[roll(populationRoulette)];

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

    std::cout<<"\nMutacje dla rozwiazan: ";
    //mutacja z prawdopodobieństwem 1/4 dla każdego osobnika
    for (int i = 0; i < nextGeneration.size(); i++) {
        int mutationChance = random(0, 7);
        switch (mutationChance) {
            case 0:
                //mutacja wybierajaca najlepszego sasiada punktu roboczego
                std::cout<<i<<" ";
                nextGeneration[i] = mutationOne(nextGeneration[i]);
                break;
            case 1:
                //mutacja wyrzucajaca losowy przedmiot z plecaka
                //mutacja zamieniająca rozwiazanie na losowe
                std::cout<<i<<" ";
                nextGeneration[i] = mutationTwo();
                break;
            default:
                break;
        }
    }

    return nextGeneration;
}

Population selectElite(Population &population){
    Population elite;
    Genotype Adam;
    int AdamIndex;
    Genotype Eve;
    int bestValue = 0;

    for(int i = 0; i<population.size(); i++){
        if(calculateValue(population[i]) > bestValue){
            Adam = population[i];
            AdamIndex = i;
            bestValue = calculateValue(population[i]);
        }
    }
    elite.push_back(Adam);


    bestValue = 0;
    for(int i = 0; i<population.size(); i++){
        if(calculateValue(population[i]) > bestValue && i!=AdamIndex){
            Eve = population[i];
            bestValue = calculateValue(population[i]);
        }
    }
    elite.push_back(Eve);
    std::cout<<"\nELITY:";
    printResult(Adam);
    printResult(Eve);


    return elite;
}

Population insertElite(Population &population, Population &elite){
    Population nextPopulation = population;
    int st_worst;
    int nd_worst;
    int worst_value = populationFitness(nextPopulation);

    for(int i = 0; i<nextPopulation.size(); i++){
        if(calculateValue(nextPopulation[i]) <= worst_value){
            worst_value = calculateValue(nextPopulation[i]);
            st_worst = i;
        }
    }

    worst_value = populationFitness(nextPopulation);
    for(int i = 0; i<nextPopulation.size();i++){
        if(calculateValue(nextPopulation[i]) <= worst_value && i != st_worst){
            worst_value = calculateValue(nextPopulation[i]);
            nd_worst = i;
        }
    }


    if(calculateValue(elite[0]) > calculateValue(nextPopulation[st_worst]))
    nextPopulation[st_worst] = elite[0];

    if(calculateValue(elite[1]) > calculateValue(nextPopulation[nd_worst]))
    nextPopulation[nd_worst] = elite[1];

    return nextPopulation;
}

void geneticAlghoritm(int populationSize, int iterations){
    //deklaracja początkowej populacji
    Population currentPopulation = generateInitialPopulation(populationSize);

    //najlepsza wartosc
    int bestValue = 0;

    //warunek głównej pętli !!!!
    for(int i = 0; i < iterations && bestValue < wantedValue; i++){
        //tworze ruletkę dla obecnej populacji
        Roulette roulette = generateRoulette(currentPopulation);

        //deklaracja i wybranie elity obecnego rozwiazania
        Population elite = selectElite(currentPopulation);

        std::cout<<"\n\nPokolenie: "<< i + 1;
        //tworze nowe pokolenie populacji
        Population nextPopulation = reproduction(roulette, currentPopulation, populationSize);

        //zamien elite z najgorszymi osobnikami kolejnego rozwiazania
        nextPopulation = insertElite(nextPopulation, elite);

        //wypisuje nowe pokolenie
        for(int i = 0; i < nextPopulation.size(); i++){
            std::cout<<"\n";
            std::cout<<"#"<<i<< " Value: " << calculateValue(nextPopulation[i])<<
                     "\t Weight: " <<calculateWeight(nextPopulation[i]) << "\t";
            for(int j = 0; j < nextPopulation[i].size(); j++){
                if(nextPopulation[i][j] == 1){
                    std::cout<<j<<" ";
                }
            }

        }

        //akceptuje nowe pokolenie jako obecna populacja
        currentPopulation = nextPopulation;

        //przerywam obliczenia po dotarciu do oczekiwanego pułapu
        bestValue = findBest(currentPopulation);

    }
}

int main(int argc, char* argv[]){
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
    wantedValue = 105;
    int populationSize = 14;
    int iterations = 50;

    if(argc == 5){
        capacity = std::stoi(argv[1]);
        wantedValue = std::stoi(argv[2]);
        populationSize = std::stoi(argv[3]);
        iterations = std::stoi(argv[4]);
    }else{std::cout<<"NIEPRAWIDLOWE PARAMETRY WEJSCIA";return 0;}



    std::cout << "Wielkosc plecaka: " << capacity;
    std::cout << "\nPlecak poczatkowy \n(waga, wartosc)\n";
    for (int i = 0; i < items.size(); i++) {
        std::cout<< i <<"# (" << items[i].weight << ", " << items[i].value << ") "<< "\n";
    }

    geneticAlghoritm(populationSize, iterations);



}
