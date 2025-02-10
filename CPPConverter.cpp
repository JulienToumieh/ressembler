//undeclared variables.
#include <iostream>
#include <string.h>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
using namespace std;


struct noded {
	char data[80];
	noded* next;
};
struct nodeu {
	char data[80];
	int l;
	nodeu* next;
};

struct node {
    char label[80];
    char p[80]; 
    int n1, n; 
    noded* d1, * d; 
    nodeu* und;      

    vector<node*> children;  // Dynamic list of child nodes
    
    // Constructor to initialize the node with a label and empty children list
    node(const char* label_val = "", const char* p_val = "") {
        strncpy(label, label_val, sizeof(label));
        strncpy(p, p_val, sizeof(p));
        n1 = n = 0;
        d1 = d = nullptr;
        und = nullptr;
        children.clear();
    }
};

string prog;
string progStart = R"(#include <iostream>
using namespace std;

int memCells[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int currCell = 0;

void increment(int val) {
    memCells[currCell] += val;
    if (memCells[currCell] > 255)
        memCells[currCell] = 0;
    else if (memCells[currCell] < 0)
        memCells[currCell] = 255;
}

void move(int val) {
    currCell += val; 
    if (currCell > 15)
        currCell = 0;
    else if (currCell < 0)
        currCell = 15;
}

void output() {
    cout << static_cast<char>(memCells[currCell]);
}

void input() {
    char c;
    cin >> c;
    memCells[currCell] = static_cast<int>(c);
}

int main() {)";

char UL[128];
char mot[1024];
//char p[128];
char* car;
//void AP();
void program();
//void declarations();
int n = 6;
const char* TableKeywords[6] = { "OPR", "MOV" ,"JNZ", "JMP", "VIN", "VOUT" };
const char* AL();
//void eval(node*);
int find(char* s, noded* head);

int verify(const char*, const char*);
void statements();
void returns();
//void accept(const char*, const char*);
void copie(char*, char*);

node* root, * nd;

void printAST(node* n, int depth = 0) {
    if (n == nullptr) return;

    for (int i = 0; i < depth; ++i) cout << "  "; 
    cout << n->label << ": " << n->p << endl;
    
    // Recursively print all children
    for (auto child : n->children) {
        printAST(child, depth + 1);
    }
}

noded* insertatqueuelist1(noded* head, char* label) {
	noded* cur, * tmp;
	tmp = new noded;
	if (tmp == NULL)
		exit(1);
	strcpy(tmp->data, label);
	tmp->next = NULL;
	if (head == NULL)
		return tmp;
	cur = head;
	while (cur->next != NULL)
		cur = cur->next;
	cur->next = tmp;
	return head;
}

nodeu* insertatqueuelist2(nodeu* head, char* label/*, int value*/) {
	nodeu* cur, * tmp;
	tmp = new nodeu;
	if (tmp == NULL)
		exit(1);
	strcpy(tmp->data, label);
	//tmp->l = value;
	tmp->next = NULL;
	if (head == NULL)
		return tmp;
	cur = head;
	while (cur->next != NULL)
		cur = cur->next;
	cur->next = tmp;
	return head;
}

int find(char* s, noded* head) {
	noded* cur = head;
	while (cur != NULL) {
		if (!strcmp(cur->data, s))
			return 1;
		else
			cur = cur->next;
	}
	return 0;
}

void eval(node* nd) {
	if (nd != NULL) {
		if (!strcmp(nd->label, "P1")) {
            prog += progStart;
            prog += "\n";
            for (int i = 0; i < nd->children.size(); i++)
			    eval(nd->children[i]);
			prog += "\n    return 0;\n}";
		}
		else if (!strcmp(nd->label, "OPR")) {
			prog += "    increment(";
            eval(nd->children[0]);
            prog += ");\n";
		}
		else if (!strcmp(nd->label, "MOV")) {
			prog += "    move(";
            eval(nd->children[0]);
            prog += ");\n";
		}
		else if (!strcmp(nd->label, "VIN")) {
			prog += "    input();\n";
		}
		else if (!strcmp(nd->label, "VOUT")) {
			prog += "    output();\n";
		}
        else if (!strcmp(nd->label, "JMP")) {
			prog += "    goto ";
            eval(nd->children[0]);
            prog += ";\n";
		}
		else if (!strcmp(nd->label, "JNZ")) {
			prog += "    if (memCells[currCell] != 0) {\n        goto ";
            eval(nd->children[0]);
            prog += ";\n    }\n";
		}
        else if (!strcmp(nd->label, "LBL")) {
            prog += "\n    ";
            prog += nd->p;
            prog += ":\n";
            root->d = insertatqueuelist1(root->d, nd->p);
		}
        else if (!strcmp(nd->label, "INT")) {
			prog += nd->p;
		}
        else if (!strcmp(nd->label, "PLBL")) {
            prog += nd->p;
            root->d1 = insertatqueuelist1(root->d1, nd->p);
		}
		else
			if (!strcmp(nd->label, "p5")) {
				nd->und = NULL;
			}
	}

}

const char* AL() {
    int state = 0, i;
    char* p = NULL;
    while (1) {
        switch (state) {
        case 0:
            if (*car == ' '){
                car++;
                return "space";
            }
                
            if (*car == '$'){
                return "END";
            }
                
            else if ((*car == '<' && *(car + 1) == '<')) {
                p = car;
                car++;
                state = 2;
                break;
            }
            else if (isalpha(*car)) {
                p = car;
                state = 1;
                break;
            }
            else if (*car == '+' || *car == '-' || isdigit(*car)) {
                p = car;
                state = 3;
                break;
            }
            else {
                cout << "Lexical error\n";
                exit(1);
            }

        case 1:
            if (isalpha(*car) || isdigit(*car)) {
				copie(p, car);
				for (i = 0; i < n; i++) {
					if (!strcmp(UL, TableKeywords[i])){
                        return "KW";
                    }
				}
				break;
			}
			else {
				copie(p, car);
                for (i = 0; i < n; i++) {
					if (!strcmp(UL, TableKeywords[i])){
                        return "KW";
                    }
				}
				return "PLBL";
			}

        case 2:
            if (isalpha(*car) || isdigit(*car)) {
                copie(p, car);
                break;
            }
            else {
                if (*car == '>' && *(car + 1) == '>'){
                    car++;
                    car++;
                    copie(p+2, car-2);
                    return "LBL";
                }
                cout << "Lexical error\n";
                exit(1);
            }

        case 3: 
            if (isdigit(*car)) {
                break;
            } else {
                copie(p, car);
                return "INT";
            }
        }
        ++car;
    }
}

void accept(const char* symbol, const char* type, const string& token) {
    // Verify token type and value
    if (type == "KW") {
        if (token == "KW" && strcmp(UL, symbol) == 0) {
            return;
        } else {
            cout << "Error: The word is not in the language\n";
            exit(1);
        }
    }
    else if (type == "LBL") {
        if (token == "LBL" && strcmp(UL, symbol) == 0) {
            return;
        } else {
            cout << "Error: The label is not in the language\n";
            exit(1);
        }
    }
    else if (type == "PLBL") {
        if (token == "PLBL" && strcmp(UL, symbol) == 0) {
            return;
        } else {
            cout << "Error: The label is not in the language\n";
            exit(1);
        }
    }
    else if (type == "INT") {
        if (token == "INT" && strcmp(UL, symbol) == 0) {
            return;
        } else {
            cout << "Error: The integer is not in the language\n";
            exit(1);
        }
    }
}

void program() {
    root = new node("P1");
    node* currentNode = root;

    string token = AL();  
    while (token != "END") {
        if (token == "space") {
            token = AL();
        }
        
        // Check for the different keywords and create corresponding nodes
        cout << token << " - " << UL << endl;
        if (token == "KW") {
            if (strcmp(UL, "OPR") == 0) {
                // Process OPR
                accept("OPR", "KW", token);
                currentNode = new node("OPR", "");  // Create an OPR node
                root->children.push_back(currentNode);
                token = AL();
                if (token == "space") {
                    token = AL();
                }
                accept(UL, "INT", token);  // Accept the integer operand
                currentNode->children.push_back(new node("INT", UL));  
            }
            else if (strcmp(UL, "MOV") == 0) {
                // Process MOV
                accept("MOV", "KW", token);
                currentNode = new node("MOV", "");  // Create a MOV node
                root->children.push_back(currentNode);
                token = AL();
                if (token == "space") {
                    token = AL();
                }
                accept(UL, "INT", token); 
                currentNode->children.push_back(new node("INT", UL)); 
            }
            else if (strcmp(UL, "VOUT") == 0) {
                // Process VOUT
                accept("VOUT", "KW", token);
                currentNode = new node("VOUT", "");
                root->children.push_back(currentNode);
            }
            else if (strcmp(UL, "VIN") == 0) {
                // Process VOUT
                accept("VIN", "KW", token);
                currentNode = new node("VIN", "");
                root->children.push_back(currentNode);
            }
            else if (strcmp(UL, "JNZ") == 0) {
                // Process JNZ
                accept("JNZ", "KW", token);
                currentNode = new node("JNZ", "");
                root->children.push_back(currentNode);
                token = AL();
                if (token == "space") {
                    token = AL();
                }
                accept(UL, "PLBL", token);  // Accept the label for the jump
                currentNode->children.push_back(new node("PLBL", UL));  
            }
            else if (strcmp(UL, "JMP") == 0) {
                // Process JMP
                accept("JMP", "KW", token);
                currentNode = new node("JMP", "");
                root->children.push_back(currentNode);
                token = AL();
                if (token == "space") {
                    token = AL();
                }
                accept(UL, "PLBL", token);  // Accept the label for the jump
                currentNode->children.push_back(new node("PLBL", UL));  
            }
        }
        else if (token == "LBL") {
                currentNode = new node("LBL", UL);  // Create a label definition node
                root->children.push_back(currentNode);
        }
        else {
            cout << "Unexpected token: " << UL << endl;
            exit(1);
        }

        token = AL();  
    }

}


void returns() {
	unsigned int i;
	for (i = 0; i < strlen(UL); i++);
	car--;
}

void copie(char* p, char* car) {
	int i = 0;
	while (p < car) {
		UL[i] = *p;
		p++;
		i++;
	}
	UL[i] = '\0';
}


unordered_map<string, string> symbolTable; 
unordered_set<string> labels;  // Set of defined labels


bool isLabelDefined(const string& label) {
    return labels.find(label) != labels.end();
}

void printList(noded* head) {
    noded* current = head;
    
    cout << "List: "; 
    // Traverse the list and print each node
    while (current != NULL) {
        cout << current->data << endl;
        current = current->next;
    }
}

void printList2(nodeu* head) {
    nodeu* current = head;
    
    // Traverse the list and print each node
    while (current != NULL) {
        cout << "Data: " << current->data /*<< ", Integer: " << current->l*/ << endl;
        current = current->next;
    }
}


int checkLabels(noded* head1, noded* head) {  // Check if a label is defined
    noded* current = head1;
    
    // Traverse the list and print each node
    while (current != NULL) {
        noded* curr = head;
        bool found = false;

        while (curr != NULL) {
            if (!strcmp(curr->data, current->data)){
                found = true;
                break;
            }
            curr = curr->next;
        }

        if (!found) {
            root->und = insertatqueuelist2(root->und, current->data);
            return 0;
        }
        current = current->next;
    }
    return 1;
}

string getFile(const string& filename) { //Read word from file
    ifstream inputFile(filename);
    
    if (!inputFile.is_open()) {
        cout << "Error opening the file!" << endl;
        return "";
    }

    string s; 
    getline(inputFile, s, '\0'); 
    inputFile.close();
    unsigned int n = s.size(); 
    cout << "File content read: " << endl;

    return s;
}


int main() {
    //OPR 5 MOV -2 VOUT <<label1>> JNZ label2
	unsigned int n;

    strcpy(mot, getFile("InputCode.rs").c_str());

    for (n = 0; mot[n] != '\0'; n++) {
        if (mot[n] == '\n') {
            mot[n] = ' ';  // Replace newline with space
        }
    }
    n = strlen(mot);
	mot[n] = '$';
	mot[n + 1] = '\0';
	car = mot;

    program();
    printAST(root);
    eval(root);

    printList(root->d1);
    if (checkLabels(root->d1, root->d) == 0){
        cout << "Undefined labels found.";
        exit(1);
    }
    printList2(root->und);


    ofstream outFile("compiled.cpp");

    if (outFile.is_open()) {
        outFile << prog;
        outFile.close();
        cout << "Program generated successfully." << endl;
    } else {
        cout << "Unable to open file for writing." << endl;
    }

	return 0;
}
