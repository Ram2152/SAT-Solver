#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using namespace std;

class DPLLSolver {
public:
    bool solve(const vector<vector<int>>& clauses, int numVars) {
        unordered_map<int, bool> assignment;
        return DPLL(clauses, assignment, numVars);
    }

private:
    bool DPLL(vector<vector<int>> clauses, unordered_map<int, bool>& assignment, int numVars) {
        // Check if all clauses are satisfied
        if (clauses.empty()) {
            return true;
        }

        // Check if any clause is unsatisfied
        for (const auto& clause : clauses) {
            if (clause.empty()) {
                return false;
            }
        }

        // Unit propagation: if a clause has only one literal, assign it
        for (const auto& clause : clauses) {
            if (clause.size() == 1) {
                int unit = clause[0];
                bool value = (unit > 0);
                assignment[abs(unit)] = value;

                return DPLL(simplify(clauses, unit), assignment, numVars);
            }
        }

        // Choose a variable to branch on (heuristic can be added here)
        int variable = chooseVariable(clauses, numVars);

        // Try assigning the variable to true
        assignment[variable] = true;
        if (DPLL(simplify(clauses, variable), assignment, numVars)) {
            return true;
        }

        // Try assigning the variable to false
        assignment[variable] = false;
        if (DPLL(simplify(clauses, -variable), assignment, numVars)) {
            return true;
        }

        // If both assignments fail, the formula is unsatisfiable
        return false;
    }

    vector<vector<int>> simplify(const vector<vector<int>>& clauses, int literal) {
        vector<vector<int>> simplified;

        for (const auto& clause : clauses) {
            // If the clause contains the literal, it's satisfied, skip it
            if (find(clause.begin(), clause.end(), literal) != clause.end()) {
                continue;
            }

            // Remove the negation of the literal from the clause
            vector<int> newClause;
            for (int lit : clause) {
                if (lit != -literal) {
                    newClause.push_back(lit);
                }
            }

            // Add the new clause if it's non-empty
            simplified.push_back(newClause);
        }

        return simplified;
    }

    int chooseVariable(const vector<vector<int>>& clauses, int numVars) {
        // Simple heuristic: choose the first unassigned variable
        unordered_set<int> seenVars;
        for (const auto& clause : clauses) {
            for (int lit : clause) {
                seenVars.insert(abs(lit));
            }
        }

        for (int i = 1; i <= numVars; ++i) {
            if (seenVars.find(i) != seenVars.end()) {
                return i;
            }
        }

        return 0;  // Should never reach here
    }
};

int main() {
    // Example input: CNF formula (x1 OR x2) AND (NOT x1 OR x3) AND (NOT x2 OR NOT x3)
    vector<vector<int>> clauses = {
        {1, 2, -3},
        {-1, -2},
        {3, 4},
        {-3, -4},
        {-1, 2, 3, 4},
        {1, -2, -3, -4}
    };
    int numVars = 4;  // 4 variables: x1, x2, x3, x4


    DPLLSolver solver;
    bool result = solver.solve(clauses, numVars);

    if (result) {
        cout << "SATISFIABLE" << endl;
    } else {
        cout << "UNSATISFIABLE" << endl;
    }

    return 0;
}
