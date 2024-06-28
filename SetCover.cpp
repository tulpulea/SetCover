#include "SetCover.hpp"
#include <iostream>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <numeric>

//default constructor
SetCover::SetCover() : best(0) {} 

// copy constructor
SetCover::SetCover(const SetCover& s) : people(s.people),skills(s.skills),
                                        best(s.best),ticket(s.ticket) {}

//copy-assignment operator overload
SetCover& SetCover::operator=(const SetCover& s){
    people = s.people;
    skills = s.skills;
    best = s.best;
    ticket = s.ticket;
    return *this;
}

// destructor
SetCover::~SetCover(){}

// read data function
void SetCover::read(const std::string &f_name){
    int N; //number of people (possible sets)
    int K; //number of skills (number of elements in universe)
    std::ifstream f_in(f_name);
    f_in >> N >> K;
    for (int i = 0; i < K; i++)
    {
        // insert the skills into the skills set
        std::string s;
        f_in >> s;
        skills.insert(s); 
    }
    int num_skills = 0;
    std::string i_skill;
    // build vector of people's skills
    std::set<std::string> i_skills; //init set of each ith person
    for (int i = 0; i < N; i++) 
    {
        f_in >> num_skills;
        if (num_skills != 0){
            //add given person's skills
            for (int j = 0; j < num_skills; j++){
                f_in >> i_skill;
                i_skills.insert(i_skill); //add skill to set
            }
            people.push_back(i_skills); //push back ith person skills to vector
            i_skills.clear(); //clear set for next person
        }
    }
    best = static_cast<int>(people.size());
}

// remove redundant subsets from consideration
void SetCover::rm_redun_subsets() { //init set of iterators to vec to remove
    std::set<std::vector<std::set<std::string>>::iterator> subsets_to_remove;
    for (auto i = people.begin(); i != people.end();i++){ //iterate through people vec
        for(auto j = people.begin(); j != people.end(); j++){ //create another iterator loop
            if (i != j && //if both people aren't the same
            std::includes(j->begin(),j->end(),
                        i->begin(),i->end())) //and if i is subset of j
            {
                subsets_to_remove.insert(i);
            }
            //or if there are duplicates
            else if(i != j && *i == *j)
            {
                if (i < j) {
                    subsets_to_remove.insert(j);
                }
                else {
                    subsets_to_remove.insert(i);
                }
            }
        }
    }
    //remove the redundant subsets
    int cnt = 0; //variable to ensure correct element is removed from vec
    for (auto p = subsets_to_remove.begin(); p != subsets_to_remove.end();p++){
            people.erase((*p)-cnt);
            cnt++;
        }
}

// preprocess pipeline step
void SetCover::preprocess(){
    rm_redun_subsets();
    //sort the people map in descending order of length of skills for each person
    std::sort(people.begin(),people.end(), 
    [](const std::set<std::string>& a,
    const std::set<std::string>& b)
    { //if size of one set is greater than another sort it first
        return static_cast<int>(a.size()) > static_cast<int>(b.size());
    });
    best = static_cast<int>(people.size());
    }

// evaluate ticket - check how many skills are met
int SetCover::eval_ticket() {
    std::set<std::string> check;
    for(auto person_it = ticket.begin(); person_it != ticket.end(); person_it++){
        // ticket is a set of iterators that point to the people vec
        // therefor dereference person_it to get to individual iterator that points to people
        // deref+access begin member to get begin iterator of that person's skill set
        // iterate through those skills
        for(auto skill_it = (*person_it)->begin(); 
        skill_it != (*person_it)->end();skill_it++){
            check.insert(*skill_it); //add skill to check set
        }
    }
    return static_cast<int>(check.size()); //returns how many skills are met
}

// solve which implements branch and bound approach with DFS
void SetCover::solve(std::vector<std::set<std::string>>::iterator curr) {
    //if current ticket meets all skills in universe
    if (eval_ticket() == static_cast<int>(skills.size()))
    { //set min to smallest of current best and ticket size
        best = std::min(best,static_cast<int>(ticket.size()));
        return; //break recursion
    }
    //end and branching condition
    //if reached end of people vec OR number of skills met + total remaining
    // possible skills doesn't reach size of universe (branching)
    if (curr == people.end() || (eval_ticket() + 
    std::accumulate(curr, people.end(), 0, 
    [](const int &acc, const std::set<std::string> &s)
    {return acc + static_cast<int>(s.size());}) 
    < static_cast<int>(skills.size()))){
        return; //end recursion
    }
    //bounding condition
    //if number of skills met already exceeds current best
    if (static_cast<int>(ticket.size()) >= best)
    {
        return; //end recursion
    }

    ticket.insert(curr); //add current person to ticket
    solve(++curr); //go to next level of tree
    ticket.erase(--curr); //erase that person from 
    solve(++curr); //go to next level of tree
}

int SetCover::find_min() {
    preprocess(); //preprocess step
    //set begin of dfs search
    std::vector<std::set<std::string>>::iterator start = people.begin();
    solve(start); //call dfs search algorithm
    return best; 
}









