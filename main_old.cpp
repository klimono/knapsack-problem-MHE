#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <random>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <memory>

int knapsack_cap = 12;

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

    item goal() {
        item goal_var;
        //    double goal_weight = 0;
        for(int i = 0; i < size() && goal_var[0] <= knapsack_cap ; i++){
            auto &p = *problem;
            auto &t = *this;
            goal_var = p[t[i]] + p[t[(i+1)%size()]];
        }
        return {goal_var};
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

solution_t random_modify(solution_t current_item) {
    std::uniform_int_distribution<int> distr(0, current_item.size()-1);
    int a = distr(rgen);
    std::swap(current_item[a], current_item[(a+1) % current_item.size()]);
    return current_item;
}

int main() {

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
    std::cout <<solution << " " << solution.goal() <<  std::endl;


    for (int i = 0; i < 100; i++) {
        auto new_solution = random_modify(solution);
        if(new_solution.goal()[1] >= solution.goal()[1]) {
            solution = new_solution;
            std:: cout << i << " " << solution << " " << solution.goal() << std::endl;
        }
    }

    std::cout << solution << std::endl;



    return 0;






}
