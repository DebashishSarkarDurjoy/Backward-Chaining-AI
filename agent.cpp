// This program takes Facts first then the rules. Automatically promts for the facts and rules.
// This program is case sensitive for: "nil" and "v"
// Please use lowercase 'nil' to terminate the input stream.
// Please use lowercase 'v' to represent AND connectives. 

#include <iostream>
#include <vector>
#include <set>

using namespace std;

vector<string> all_alphabets = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s",
                                 "t", "u",    "w", "x", "y", "z"    , "A", "B", "C", "D", "E", "F", "G", "H", "I", "J",
                                 "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U",    "W", "X", "Y", "Z" };

bool inFacts(vector<string> facts, string letter) {
    for (string e: facts) {
        if (letter == e) return true;
    }
    return false;
}

bool checkFacts(vector<string> &facts, string input) {
    // cout << "Here" << endl;
    for (string e: facts) {
            if (e == input) {
                return true;
            }
        }
    return false;
}

class Rules {
public:
    vector<string> heads;
    vector<vector<string>> bodies;
    set<string> alphabets;
    Rules () {
        for (auto s: all_alphabets) {
            this->alphabets.insert(s);
        }
    }

    // head: input[-1];
    // junction: input[1];
    // body: input[0:len(input)-1 -2];
    void insertRule(string head, string body, string junction) {
        //cout << "body: " << body << " head: " << head << " junction: " << junction << endl;
        this->heads.push_back(head);
        vector<string> body_vector;
        body_vector.push_back(junction);
        for (int i = 0; i < body.size(); i+=2) {
            string s;
            s = body[i];
            body_vector.push_back(s);
        }
        this->bodies.push_back(body_vector);
        // cout << "body: " << this->bodies[] << " head: " << this->heads[0] << " junction: " << this->bodies[0][0] << endl;

    }

    bool infer(string input, vector<string> &facts, int next_idx) {
        // base case
        if (!this->inHeads(input) && !inFacts(facts, input)) return false;
        // recursive case
        if (checkFacts(facts, input)) return true; 

        for (int i = 0; i < this->heads.size(); i++) {
            if (input == this->heads[i]) {
                vector<string> body_vector = this->bodies[i];
                // perform AND operation
                if (body_vector[0] == "^") {
                    bool result = true;
                    for (int i = 1; (result == true) && (i < body_vector.size()); i++) {
                        result = inFacts(facts, body_vector[i]) || this->inHeads(body_vector[i]);
                    }
                    return result;
                }
                
                // perform OR operation
                bool result = false;
                while (result == false && next_idx < body_vector.size()) {
                    next_idx++;
                    result = this->infer(body_vector[next_idx], facts, next_idx);
                }
                return result;
            }
        }
        return false;
        
    }

    bool inHeads(string letter) {
        for (string e: this->heads) {
            if (e == letter) return true;
        }
        return false;
    }


    void showRules() {
        cout << endl;
        cout << "Rules Table: " << endl;
        int k = 0;
        for (auto row: this->bodies) {
                for (int i = 1; i < row.size(); i++) {
                    cout << row[i] << " ";
                    if (i == row.size() - 1) continue;
                    else cout << row[0] << " ";
            }

            cout << " => ";

            cout << this->heads[k] << endl;

            k++;
        }
        cout << endl;
    }

};

void showFacts(vector<string> &facts) {
    cout << "\nFacts: " << endl;
    for (auto f: facts) {
        cout << f << endl;
    }
    cout << endl;
}



int main(void) {
    vector<string> facts;
    Rules rules;

    // get the facts
    cout << "Enter facts: " << endl;
    while (true) {
        string input = "";
        cin >> input;
        if (input == "nil") break;
        facts.push_back(input);
    }

    // get the rules
    cout << "Enter rules: " << endl;
    while (true) {
        string input = "";
        cin >> input;
        if (input == "nil") break;
        string head;
        head = input[input.size()-1];
        string body = "";
        for (int i = 0; i <= input.size()-1 -3; i++) {
            body += input[i];
        }
        string junction;
        junction = input[1];

        // cout << "body: " << body << " head: " << head << " junction: " << junction << endl;
        rules.insertRule(head, body, junction);
    }

    rules.showRules();
    showFacts(facts);

    cout << "Prove: " << endl;
    // checkFacts(facts, "a");
    while (true) {
        string input = "";
        cin >> input;
        if (input == "nil") break;

        // look for facts in the facts vector
        // checkFacts(facts, input);

        bool result = rules.infer(input, facts, 0);

        if (result) cout << " (yes) " << endl;
        else cout << " (no) " << endl;

    }
    


    return 0;
}
