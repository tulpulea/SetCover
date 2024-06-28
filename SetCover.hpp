#ifndef SETCOVER
#define SETCOVER

#include <set>
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
    void solve(std::vector<std::set<std::string>>::iterator); //solve problem
    int find_min(); //find and return solution

private:
    std::vector<std::set<std::string>> people; //initialize vector of people to skills
    std::set<std::string> skills; //initialize set of skills in universe
    int best; //initialize solution data
    // std::set<int> ticket; //initialize ticket of solution
    std::set<std::vector<std::set<std::string>>::iterator> ticket; //initialize ticket of solution
    // set of pointers to vec

}
;

#endif