#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <random>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <memory>

using item = std::array<double, 2>;

item operator+(item a, item b){
    return {a[0] + b[0], a[1] + b[1]};
}

using problem_t = std::vector<item>;

class solution_t: public std::vector<int> {
public:
    std::shared_ptr<problem_t> problem;
    solution_t(std::shared_ptr<problem_t> problem_): problem(problem_){
    }
};

std::ostream& operator<<(std::ostream& o, const item v)
{
    o << "[ ";
    for (auto e : v)
        o << e << " ";
    o << "]";
    return o;
}
std::ostream& operator<<(std::ostream& o, const problem_t v)
{
    o << "{ ";
    for (auto e : v)
        o << e << " ";
    o << "}";
    return o;
}
std::ostream& operator<<(std::ostream& o, const solution_t v)
{
    o << "{ ";
    for (auto e : v)
        o << e << " ";
    o << "}";
    return o;
}

std::random_device rd;

std::mt19937 rgen(rd());

void random_hillclimb(problem_t problem, solution_t solution, int knapsack_cap){
    item goal = {0, 0};
    std::cout << "Items: { ";
    for (int i = 0; i < solution.size() /*&& goal[0] + problem[solution[i]][0] <= knapsack_cap*/; i++){
        if (goal[0] + problem[solution[i]][0] <= knapsack_cap) {
            goal = goal + problem[solution[i]];
            std::cout << solution[i] + 1 << " ";
        }
    }
    std::cout << "}" << std::endl << "W/V: " << goal <<std::endl <<std::endl;
}

int main() {
    int knapsack_cap = 12;
    problem_t knapsack_problem = {
            // weight, value
            {1, 1},
            {3, 4},
            {8, 2},
            {4, 3},
            {10, 10},
            {9, 8},
    };

    solution_t solution(std::make_shared<problem_t>(knapsack_problem));

    for (int i = 0; i < knapsack_problem.size(); i++) solution.push_back(i);
    std::shuffle(solution.begin(), solution.end(), rgen);

    std::cout <<knapsack_problem << std::endl;
    for (int i = 0; i < 100; i++) {
        std::shuffle(solution.begin(), solution.end(), rgen);
        random_hillclimb(knapsack_problem, solution, knapsack_cap);
    }




    return 0;






}
