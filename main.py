import time

class MinPeople(): 
    def __init__(self):
        self.people_bit = None
        self.skills = None
        self.best = None
        self.ticket = []
        self.number_nodes = 0


    def read(self):
        file = open("input43.txt","r")
        # N_K = input().split()
        N_K = file.readline().split()
        N = int(N_K[0])
        K = int(N_K[1])
        # self.skills = input().split()
        self.skills = file.readline().split()
        self.skill_index = {self.skills[i]:i for i in range(len(self.skills))}
        self.people_bit = []
        self.people_cache = [None] * N
        j = 0
        for i in range(N):
            num_skills = int(file.readline())
            # num_skills = int(input())
            if num_skills == 0:
                file.readline().split()
                # input().split()
            else:
                # i_skills = input().split()
                i_skills = file.readline().split()
                self.people_cache[j] = set()
                self.people_bit.append(0)
                for skill in i_skills:
                    self.people_cache[j].add(skill)
                    self.people_bit[j] |= (1 << self.skill_index[skill])

                j += 1
        self.best = len(self.people_bit)

    def is_subset(smaller:int, larger:int):
        return (smaller & larger) == smaller

    def rm_redun_subsets_cache(self):
        subsets_to_remove = set()

        # Iterate through all pairs of subsets
        for person_i, skills_i in enumerate(self.people_cache):
            for person_j, skills_j in enumerate(self.people_cache):
                # Check if skills_j is a proper subset of skills_i
                if person_i != person_j and skills_i < skills_j:
                    subsets_to_remove.add(person_i)
                elif person_i != person_j and skills_i == skills_j:
                    if person_i < person_j:
                        subsets_to_remove.add(person_j)
                    else:
                        subsets_to_remove.add(person_i)
        
        subsets_to_remove = list(subsets_to_remove)
        subsets_to_remove.sort(reverse=True)
        for person_to_remove in subsets_to_remove:
            self.people_cache.pop(person_to_remove)


    def rm_redun_subsets_bit(self):
        # Step 1: Sort the people list lexicographically (based on skill sets)
        # print(self.people)
        self.people_bit.sort(key=str,reverse=True)  # Sorting by binary string representation of skill sets

        # Step 2: Iterate through the sorted list and remove subsets
        unique_people_skills = []

        for person in self.people_bit:
            is_subset_of_existing = False

            # Check if the current person is a subset of any person already added
            for existing_person in unique_people_skills:
                if (person & existing_person) == person:  # Bitwise AND to check subset
                    is_subset_of_existing = True
                    break

            # If the person is not a subset of any other, add them to the list
            if not is_subset_of_existing:
                unique_people_skills.append(person)

        # Update the people list with the filtered list (no subsets)
        self.people_bit = unique_people_skills




        # subsets_to_remove = set()

        # Iterate through all pairs of subsets
        # for person_i, skills_i in self.people.items():
        #     for person_j, skills_j in self.people.items():
        #         # Check if skills_j is a proper subset of skills_i
        #         if person_i != person_j and skills_i < skills_j:
        #             subsets_to_remove.add(person_i)
        #         elif person_i != person_j and skills_i == skills_j:
        #             if person_i < person_j:
        #                 subsets_to_remove.add(person_j)
        #             else:
        #                 subsets_to_remove.add(person_i)
        # for person_to_remove in subsets_to_remove:
        #     del self.people[person_to_remove]

    def preprocess(self):
        self.rm_redun_subsets_bit()
        print("bit people length",len(self.people_bit))
        self.rm_redun_subsets_cache()
        print("cache people length",len(self.people_cache))
        # sorted_people_skills = dict(sorted(self.people.items(), key=lambda item: len(item[1]), reverse=True))
        # self.people = {i: sorted_people_skills[key] for i, key in enumerate(sorted_people_skills)}
        # print("people skills array length:",len(self.people))
        self.best = len(self.people_bit)

    def evaluate_ticket(self):
        check = {skill:0 for skill in self.skills}
        for person in self.ticket:
            for skill in self.people[person]:
                check[skill] = 1
        return sum(check.values())
    
    def solve_bit(self, curr, covered_skills, current_coverage):
        if current_coverage == len(self.skills):
            self.best = min(self.best, len(self.ticket))
            return

        if curr == len(self.people_bit):
            return
        
        # current_coverage + bin(self.max_remaining_skills(curr)).count('1') < len(self.skills)

        remaining_skills = covered_skills

        for person in self.people_bit[curr:]:
            remaining_skills |= person
        
        remaining_coverage = bin(remaining_skills).count('1')

        if current_coverage + remaining_coverage < len(self.skills):
            return

        if len(self.ticket) >= self.best:
            return

        # Add current person to the ticket
        self.ticket.append(curr)

        # Update covered skills using bitwise OR
        new_covered_skills = covered_skills | self.people_bit[curr]
        new_coverage = bin(new_covered_skills).count('1')  # Count newly covered skills

        # Recursively explore adding this person to the ticket
        self.solve_bit(curr + 1, new_covered_skills, new_coverage)

        # Backtrack: Remove the person from the ticket
        self.ticket.pop()

        # Recursively explore skipping this person
        self.solve_bit(curr + 1, covered_skills, current_coverage)


    def solve_cache(self, curr, covered_skills, current_coverage):
        # Base case: If all skills are covered
        if current_coverage == len(self.skills):
            self.best = min(self.best, len(self.ticket))
            return

        # Branching condition: Check remaining potential coverage
        remaining_skills = sum(len(skills) for skills in self.people_cache[curr:])
        if curr == len(self.people_cache) or (current_coverage + remaining_skills < len(self.skills)):
            return

        # Bounding condition: If current ticket size exceeds the best solution
        if len(self.ticket) >= self.best:
            return

        # Add the current person to the ticket
        self.ticket.append(curr)

        # Update the covered skills
        new_covered_skills = covered_skills | self.people_cache[curr]  # Union of sets to track coverage
        new_coverage = len(new_covered_skills)

        # Recursively explore adding this person
        self.solve_cache(curr + 1, new_covered_skills, new_coverage)

        # Remove the current person from the ticket (backtrack)
        self.ticket.pop()

        # Recursively explore skipping this person
        self.solve_cache(curr + 1, covered_skills, current_coverage)




    # def solve(self,curr):
    #     #found solution
    #     eval_tic = self.evaluate_ticket()

    #     if eval_tic == len(self.skills):
    #         self.best = min(self.best,len(self.ticket))
    #         return 

    #     if (curr == len(self.people)) or (eval_tic + sum([len(skills) for skills in list(self.people.values())[curr:]]) < len(self.skills)):
    #         return 
        
    #     if len(self.ticket) >= self.best:
    #         return
        
    #     self.ticket.add(curr)
    #     self.solve(curr+1)
    #     self.ticket.remove(curr)
    #     self.solve(curr+1)

    def brute_force(self,curr):
        self.number_nodes += 1
        if self.evaluate_ticket() == len(self.skills):
            self.best = min(self.best, len(self.ticket))
            return
        
        if curr == len(self.people):
            return
        
        self.ticket.add(curr)
        self.brute_force(curr+1)
        self.ticket.remove(curr)
        self.brute_force(curr+1)

    def find_min(self):
        self.preprocess()
        # start = 0
        # self.number_nodes = 0
        # start_time_brute = time.time()
        # self.brute_force(0)
        # end_time_brute = time.time()
        # brute_time = end_time_brute - start_time_brute
        # print(f"Brute-force: Nodes searched = {self.number_nodes}, Time taken = {brute_time:.6f} seconds")

        self.number_nodes = 0
        start_time_bb = time.time()
        covered_skills = 0
        self.solve_bit(0,covered_skills,0)
        end_time_bb = time.time()
        bb_time = end_time_bb - start_time_bb
        print(self.best)
        print(f"Branch-and-Bound Bits: Nodes searched = {self.number_nodes}, Time taken = {bb_time:.6f} seconds")
        
        self.best = len(self.people_cache)
        self.number_nodes = 0
        start_time_bb = time.time()
        covered_skills = set()
        self.solve_cache(0,covered_skills,0)
        end_time_bb = time.time()
        bb_time = end_time_bb - start_time_bb
        print(self.best)
        print(f"Branch-and-Bound Caching: Nodes searched = {self.number_nodes}, Time taken = {bb_time:.6f} seconds")
        return self.best
    

        
m = MinPeople()
m.read()
sol = m.find_min()
print(sol)


