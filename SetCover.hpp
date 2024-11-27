#ifndef SETCOVER
#define SETCOVER

#include <bitset> //was <set>
#include <unordered_map>
#include <map>
#include <vector>
#include <utility>
#include <string>
#include <iterator>

class SetCover 
{
public:
    SetCover(); // default constructor
    SetCover(const SetCover&); // copy constructor 
    SetCover &operator=(const SetCover&); //copy-assignment overlaod
    ~SetCover(); //destructor
    void read(const std::string&); //read data
    void rm_redun_subsets(); //remove redundant subsets
    void preprocess(); //preprocess 
    int eval_ticket(); //evaluate ticket
    void solve(std::vector<std::bitset<64>>::iterator, const std::bitset<64>&, const int&); //solve problem
    int find_min(); //find and return solution

private:
    // std::unordered_map<std::string, int> skill_to_index;
    std::vector<std::bitset<64>> people_skills;
    // std::vector<std::vector<bool>> people_skills; //initialize vector of people to skills
    std::unordered_map<std::string, int> skill_to_index;
    // std::unordered_set<std::string> skills; //initialize set of skills in universe
    int best; //initialize solution data
    std::vector<int> ticket; //initialize ticket of solution
    // std::unordered_set<std::vector<std::unordered_set<std::string>>::iterator> ticket; //initialize ticket of solution
    // set of pointers to vec

}
;

#endif