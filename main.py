import time

class MinPeople(): 
    def __init__(self):
        self.people = None
        self.skills = None
        self.best = None
        self.ticket = set()
        self.number_nodes = 0


    def read(self):
        file = open("input15.txt","r")
        # N_K = input().split()
        N_K = file.readline().split()
        N = int(N_K[0])
        K = int(N_K[1])
        # self.skills = input().split()
        self.skills = file.readline().split()
        self.people = {}
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
                self.people[j] = set()
                for skill in i_skills:
                    self.people[j].add(skill)
                j += 1
        self.best = len(self.people)

    def remove_redundant_subsets(self):
        subsets_to_remove = set()
        # Iterate through all pairs of subsets
        for person_i, skills_i in self.people.items():
            for person_j, skills_j in self.people.items():
                # Check if skills_j is a proper subset of skills_i
                if person_i != person_j and skills_i < skills_j:
                    subsets_to_remove.add(person_i)
                elif person_i != person_j and skills_i == skills_j:
                    if person_i < person_j:
                        subsets_to_remove.add(person_j)
                    else:
                        subsets_to_remove.add(person_i)
        for person_to_remove in subsets_to_remove:
            del self.people[person_to_remove]

    def preprocess(self):
        self.remove_redundant_subsets()
        sorted_people_skills = dict(sorted(self.people.items(), key=lambda item: len(item[1]), reverse=True))
        self.people = {i: sorted_people_skills[key] for i, key in enumerate(sorted_people_skills)}
        self.best = len(self.people)

    def evaluate_ticket(self):
        check = {skill:0 for skill in self.skills}
        for person in self.ticket:
            for skill in self.people[person]:
                check[skill] = 1
        return sum(check.values())

    def solve(self,curr):
        #found solution
        self.number_nodes += 1

        if self.evaluate_ticket() == len(self.skills):
            self.best = min(self.best,len(self.ticket))
            return 

        if (curr == len(self.people)) or (self.evaluate_ticket() + sum([len(skills) for skills in list(self.people.values())[curr:]]) < len(self.skills)):
            return 
        
        if len(self.ticket) >= self.best:
            return
        
        self.ticket.add(curr)
        self.solve(curr+1)
        self.ticket.remove(curr)
        self.solve(curr+1)

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
        self.number_nodes = 0
        start_time_brute = time.time()
        self.brute_force(0)
        end_time_brute = time.time()
        brute_time = end_time_brute - start_time_brute
        print(f"Brute-force: Nodes searched = {self.number_nodes}, Time taken = {brute_time:.6f} seconds")

        self.number_nodes = 0
        start_time_bb = time.time()
        self.solve(0)
        end_time_bb = time.time()
        bb_time = end_time_bb - start_time_bb
        print(f"Branch-and-Bound: Nodes searched = {self.number_nodes}, Time taken = {bb_time:.6f} seconds")
        return self.best
    
m = MinPeople()
m.read()
sol = m.find_min()
print(sol)


