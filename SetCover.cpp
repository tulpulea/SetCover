#include "SetCover.hpp"
#include <iostream>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <numeric>
#include <chrono>

//default constructor
SetCover::SetCover() : best(0) {} 

// copy constructor
SetCover::SetCover(const SetCover& s) : people_skills(s.people_skills),skill_to_index(s.skill_to_index),
                                        best(s.best),ticket(s.ticket) {}

//copy-assignment operator overload
SetCover& SetCover::operator=(const SetCover& s){
    people_skills = s.people_skills;
    skill_to_index = s.skill_to_index;
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
        // skills.insert(s); 
        skill_to_index.insert({s,i}); //create mapping of skill to index
        // std::cout << "skill and index inserted: " << s << " " << i << std::endl;
    }
    // std::cout << "size of skill map " << skill_to_index.size() << std::endl;
    int num_skills = 0;
    std::string i_skill;
    // build vector of people's skills
    // std::bitset<> i_skills; //init set of each ith person
    // std::vector<std::bitset<64>> people_skills(N);
    people_skills.resize(N);
    for (int i = 0; i < N; i++) 
    {
        f_in >> num_skills;
        if (num_skills != 0){
            //add given person's skills
            for (int j = 0; j < num_skills; j++){
                f_in >> i_skill;
                if (skill_to_index[i_skill] >= 64){
                    std::cerr << "Error: Skill index out of range, bitset not big enough!" << std::endl;;
                } 
                people_skills[i].set(skill_to_index[i_skill]);
                // i_skills.insert(i_skill); //add skill to set

            }
            // people.push_back(i_skills); //push back ith person skills to vector
            // i_skills.clear(); //clear set for next person
        }
    }
    // std::cout << "people skill size " << people_skills.size() << " rows by " << people_skills[0].size() << " columns" << std::endl;
    best = static_cast<int>(people_skills.size());
}

bool is_subset(const std::bitset<64>& smaller, const std::bitset<64>& larger) {
    return (smaller & larger) == smaller;
}
//     for (size_t i = 0; i < smaller.size(); ++i) {
//         if (smaller[i] && !larger[i]) {  // If 'smaller' has a skill that 'larger' doesn't
//             return false;
//         }
//     }
//     return true;
// }

void SetCover::rm_redun_subsets() {
    // Step 1: Sort the people_skills vectors lexicographically (ascending order)
    // std::cout << "size of people " << people_skills.size() <<std::endl;
    std::sort(people_skills.begin(), people_skills.end(), 
    [](const std::bitset<64>& a, const std::bitset<64>& b) {return a.to_string() < b.to_string();});
    // std::cout << "size of people " << people_skills.size() <<std::endl;

    // Step 2: Iterate through the sorted list and remove duplicates and subsets
    std::vector<std::bitset<64>> unique_people_skills;
    
    for (const auto& person : people_skills) {
        // std::cout << "person ";
        // for (const auto& skill:person){
        //     std::cout << skill;
        // }
        // std::cout << std::endl;
         
        bool is_subset_of_existing = false;

        // Check if this person's skills are already covered by a previous person in the list
        for (const auto& existing_person : unique_people_skills) {
            if (is_subset(person, existing_person)) {
                is_subset_of_existing = true;
                break;
            }
        }

        // If this person is not a subset of any other, add them to the list
        if (!is_subset_of_existing) {
            unique_people_skills.push_back(person);
        }
    }

        // Update the people_skills with the filtered list (no subsets)
        // std::cout << "size of modified " << unique_people_skills.size() << std::endl;
        // for (const auto& person : unique_people_skills) {
        //     std::cout << "person ";
        //     for (const auto& skill:person){
        //         std::cout << skill;
        //     }
        // }
        // std::cout << std::endl;
        people_skills = unique_people_skills;
    }

// remove redundant subsets from consideration
// void SetCover::rm_redun_subsets() { //init set of iterators to vec to remove
//     std::set<std::vector<std::vector<bool>>::iterator> subsets_to_remove;
//     for (auto i = people_skills.begin(); i != people_skills.end();i++){ //iterate through people vec
//         for(auto j = people_skills.begin(); j != people_skills.end(); j++){ //create another iterator loop
//             if (i != j && //if both people aren't the same
//             std::includes(j->begin(),j->end(),
//                         i->begin(),i->end())) //and if i is subset of j
//             {
//                 subsets_to_remove.insert(i);
//             }
//             //or if there are duplicates
//             else if(i != j && *i == *j)
//             {
//                 if (i < j) {
//                     subsets_to_remove.insert(j);
//                 }
//                 else {
//                     subsets_to_remove.insert(i);
//                 }
//             }
//         }
//     }
//     //remove the redundant subsets
//     int cnt = 0; //variable to ensure correct element is removed from vec
//     for (auto p = subsets_to_remove.begin(); p != subsets_to_remove.end();p++){
//             people_skills.erase((*p)-cnt);
//             cnt++;
//         }
// }

// preprocess pipeline step
void SetCover::preprocess(){
    rm_redun_subsets();
    // std::cout << people_skills.size() << " should be bigger than 0" << std::endl;
    //sort the people map in descending order of length of skills for each person
    // std::sort(people_skills.begin(),people_skills.end(), 
    // [](const std::set<std::string>& a,
    // const std::set<std::string>& b)
    // { //if size of one set is greater than another sort it first
    //     return static_cast<int>(a.size()) > static_cast<int>(b.size());
    // });

    best = static_cast<int>(people_skills.size());
    // std::cout << "best value initial " << best << std::endl;
    }

// evaluate ticket - check how many skills are met
int SetCover::eval_ticket() {
    std::vector<bool> check(skill_to_index.size(), false);  // To track which skills are covered

    // Iterate over the people in the ticket
    for (auto person_it = ticket.begin(); person_it != ticket.end(); person_it++) {
        // person_it points to the index of the person in the people_skills vector
        int person_index = *person_it;
        
        // Iterate through the skills of the current person
        for (int skill_index = 0; skill_index < skill_to_index.size(); skill_index++) {
            // If this person has the skill, mark it as covered
            if (people_skills[person_index][skill_index]) {
                check[skill_index] = true;  // Mark this skill as covered
            }
        }
    }

    // Count how many skills are covered
    return std::count(check.begin(), check.end(), true);  // Return the number of covered skills
}

// int SetCover::eval_ticket() {
//     std::set<std::string> check;
//     for(auto person_it = ticket.begin(); person_it != ticket.end(); person_it++){
//         // ticket is a set of iterators that point to the people vec
//         // therefor dereference person_it to get to individual iterator that points to people
//         // deref+access begin member to get begin iterator of that person's skill set
//         // iterate through those skills
//         for(auto skill_it = (*person_it)->begin(); 
//         skill_it != (*person_it)->end();skill_it++){
//             check.insert(*skill_it); //add skill to check set
//         }
//     }
//     return static_cast<int>(check.size()); //returns how many skills are met
// }

// solve which implements branch and bound approach with DFS
// void SetCover::solve(std::vector<std::vector<bool>>::iterator curr, const std::vector<bool>& covered_skills, const int& current_coverage) {
//     // Base case: If all skills are covered
//     if (current_coverage == static_cast<int>(skill_to_index.size())) {
//         best = std::min(best, static_cast<int>(ticket.size()));  // Update best solution
//         return;  // End recursion
//     }

//     // Branching condition: If we've reached the end of the people vector or no potential to cover all skills
//     if (curr == people_skills.end() || current_coverage + 
//     std::accumulate(curr, people_skills.end(), 0, 
//     [](int acc, const std::vector<bool>& person_skills) {
//         return acc + std::count(person_skills.begin(), person_skills.end(), true);
//     }) < static_cast<int>(skill_to_index.size())) {
//         return;  // End recursion
//     }

//     // Bounding condition: If the current ticket size exceeds the best solution
//     if (static_cast<int>(ticket.size()) >= best) {
//         return;  // End recursion
//     }

//     // Add the current person to the ticket
//     ticket.push_back(curr - people_skills.begin());

//     // Create a copy of the covered_skills vector to update with the current person's skills
//     std::vector<bool> new_covered_skills = covered_skills;
//     int new_coverage = current_coverage;

//     // Update the coverage vector and count newly covered skills
//     for (size_t i = 0; i < skill_to_index.size(); i++) {
//         if (people_skills[curr - people_skills.begin()][i] && !covered_skills[i]) {  // Check if this skill is newly covered
//             new_covered_skills[i] = true;  // Mark the skill as covered
//             new_coverage++;  // Increment the coverage count only if it's a new skill
//         }
//     }

//     // Recursively explore adding this person to the ticket
//     solve(curr + 1, new_covered_skills, new_coverage);

//     // Remove the person from the ticket (backtrack)
//     ticket.pop_back();

//     // Recursively explore skipping this person
//     solve(curr + 1, covered_skills, current_coverage);
// }

void SetCover::solve(std::vector<std::bitset<64>>::iterator curr, const std::bitset<64>& covered_skills, const int& current_coverage) {
    // Base case: If all skills are covered
    if (current_coverage == static_cast<int>(skill_to_index.size())) {
        best = std::min(best, static_cast<int>(ticket.size()));  // Update best solution
        return;  // End recursion
    }

    // Branching condition: If we've reached the end of the people vector or no potential to cover all skills
    if (curr == people_skills.end()) {
        return;  // End recursion
    }

    // Iteratively accumulate the remaining people's skills using bitwise OR
    std::bitset<64> remaining_skills = covered_skills;
    int remaining_coverage = current_coverage;

    // Iterate through the remaining people after 'curr'
    for (auto it = curr; it != people_skills.end(); ++it) {
        remaining_skills |= *it;  // Bitwise OR with the current person's skills
    }

    // If the total potential skills (current coverage + remaining skills) is less than the total skills
    if (remaining_coverage + remaining_skills.count() < static_cast<int>(skill_to_index.size())) {
        return;  // End recursion, as it's not possible to cover all skills from here
    }

    // Bounding condition: If the current ticket size exceeds the best solution
    if (static_cast<int>(ticket.size()) >= best) {
        return;  // End recursion
    }

    // Add the current person to the ticket
    ticket.push_back(curr - people_skills.begin());

    // Create a copy of the covered_skills bitset to update with the current person's skills
    std::bitset<64> new_covered_skills = covered_skills;
    new_covered_skills |= *curr;  // Bitwise OR to update the covered skills

    // Count newly covered skills (the number of set bits in the bitset)
    int new_coverage = new_covered_skills.count();  // Count the number of covered skills

    // Recursively explore adding this person to the ticket
    solve(curr + 1, new_covered_skills, new_coverage);

    // Backtrack: Remove the person from the ticket
    ticket.pop_back();

    // Recursively explore skipping this person
    solve(curr + 1, covered_skills, current_coverage);
}




// void SetCover::solve(std::vector<std::unordered_set<std::string>>::iterator curr) {
//     //if current ticket meets all skills in universe
//     if (eval_ticket() == static_cast<int>(skills.size()))
//     { //set min to smallest of current best and ticket size
//         best = std::min(best,static_cast<int>(ticket.size()));
//         return; //break recursion
//     }
//     //end and branching condition
//     //if reached end of people vec OR number of skills met + total remaining
//     // possible skills doesn't reach size of universe (branching)
//     if (curr == people.end() || (eval_ticket() + 
//     std::accumulate(curr, people.end(), 0, 
//     [](const int &acc, const std::unordered_set<std::string> &s)
//     {return acc + static_cast<int>(s.size());}) 
//     < static_cast<int>(skills.size()))){
//         return; //end recursion
//     }
//     //bounding condition
//     //if number of skills met already exceeds current best
//     if (static_cast<int>(ticket.size()) >= best)
//     {
//         return; //end recursion
//     }

//     ticket.insert(curr); //add current person to ticket
//     solve(++curr); //go to next level of tree
//     ticket.erase(--curr); //erase that person from 
//     solve(++curr); //go to next level of tree
// }

int SetCover::find_min() {
    preprocess();  // Preprocessing step

    int current_coverage = 0;  // Initially, no skills are covered
    std::bitset<64> covered_skills;
    
    // Start DFS search with the first person (index 0) and coverage 0
    auto start_time = std::chrono::high_resolution_clock::now();
    solve(people_skills.begin(),covered_skills, current_coverage);  // Call DFS search algorithm
    auto end_time = std::chrono::high_resolution_clock::now();
    
    // Measure elapsed time
    std::chrono::duration<double> elapsed_time = end_time - start_time;
    std::cout << "Execution Time: " << elapsed_time.count() << " seconds" << std::endl;
    
    return best;
}


// int SetCover::find_min() {
//     preprocess(); //preprocess step
//     //set begin of dfs search
//     auto start = people_skills.begin();
//     auto start_time = std::chrono::high_resolution_clock::now();
//     solve(start,0); //call dfs search algorithm
//     auto end_time = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double> elapsed_time = end_time - start_time;
//     std::cout << "Execution Time: " << elapsed_time.count() << " seconds" << std::endl;
//     return best; 
// }









