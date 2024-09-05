//Nicolas Jorgensen

#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

const int FIRST_PERIOD = 5, WID = 4, TIME_STEPS = 19;

class DoublyLinkedList {
private:
    struct Node {
        string name;
        Node* prev;
        Node* next;
        Node(string na, Node* p = nullptr, Node* n = nullptr) {
            name = na;//changed node to contain string name
            prev = p;
            next = n;
        }
    };

    Node* head;
    Node* tail;

public:
    DoublyLinkedList() { head = nullptr; tail = nullptr; }

    void insert_after(string value, int position) {
        if (position < 0) {
            cout << "Position must be >= 0." << endl;
            return;
        }

        Node* newNode = new Node(value);
        if (!head) {
            head = tail = newNode;
            return;
        }

        Node* temp = head;
        for (int i = 0; i < position && temp; ++i)
            temp = temp->next;

        if (!temp) {
            cout << "Position exceeds list size. Node not inserted.\n";
            delete newNode;
            return;
        }

        newNode->next = temp->next;
        newNode->prev = temp;
        if (temp->next)
            temp->next->prev = newNode;
        else
            tail = newNode;
        temp->next = newNode;
    }

    //used to remove customer element from line at random
    void delete_pos(int pos) {
        if (!head) {
            cout << "Line is empty." << endl;
            return;
        }
    
        if (pos == 1) {
            is_served(-1);
            return;
        }
    
        Node* temp = head;
    
        for (int i = 1; i < pos; i++){
            if (!temp) {
                cout << "Position doesn't exist." << endl;
                return;
            }
            else
                temp = temp->next;
        }
        if (!temp) {
            cout << "Position doesn't exist." << endl;
            return;
        }
    
        if (!temp->next) {
            leaveRear();
            return;
        }
    
        Node* tempPrev = temp->prev;
        tempPrev->next = temp->next;
        temp->next->prev = tempPrev;
        delete temp;
    }
    //For us, regular folks
    void join_line(string v) {
        Node* newNode = new Node(v);
        cout << setw(WID) << "" << v << " joins the line\n";
        if (!tail)
            head = tail = newNode;
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }
    //for VIPS
    void vipJoin(string v) {
        Node* newNode = new Node(v);
        cout << setw(WID) << "" << v << " (VIP) joins the front of the line\n";
        if (!head)
            head = tail = newNode;
        else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
    }
    //customer leaves front of line after being served
    void is_served(int mad) {

        if (!head) {
            cout << "Line is empty." << endl;
            return;
        }

        Node * temp = head;

        if (head->next) {
            head = head->next;
            head->prev = nullptr;
        }
        else
            head = tail = nullptr;
        
        if(mad != -1) { //if left line
            cout << setw(WID) << "" << temp->name;
            cout << " is served\n";
        }
        else {//if was served
            cout << setw(WID) << "" << temp->name;
            cout << " left the line\n";
        }
        delete temp;
    }
    //leaves back of line, angrily
    void leaveRear() {
        if (!tail) {
            cout << "Line is empty." << endl;
            return;
        }
        Node * temp = tail;

        if (tail->prev) {
            tail = tail->prev;
            tail->next = nullptr;
        }
        else
            head = tail = nullptr;
        cout << setw(WID) << "" << temp->name;
        cout << " (at the rear) left the line\n";
        delete temp;
    }

    ~DoublyLinkedList() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
    //modified to match desired output
    void print() {
        cout << setw(WID) <<""<< "Resulting line:\n";
        Node* current = head;
        if (!current) {
            cout << "Line is empty." << endl;
            return;
        }
        while (current) {
            cout << setw(WID*2) << "" << current->name << endl;
            current = current->next;
        }
    }
    //returns number of nodes in DLL, good for bound checking
    int countLine() {
        Node *current = head;
        int count = 0;
        while(current){
            count++;
            current= current->next;
        }
        return count;
    }
};
string pickName();//uses file and rng to return a random name from file

int main() {
    srand(time(0)); //seeding rng
    int prob;
    DoublyLinkedList line; //empty line
    cout << "Store opens:\n";
    
    for(int i = 0; i < FIRST_PERIOD;i++) 
        line.join_line(pickName());//make 5 new customers with random names
    
    line.print(); //prints line vertically
    
    for(int i = 0; i < TIME_STEPS; i ++) {//loops all scenarios for TIME_STEPS
        cout <<"Time step #" <<i+2 <<":" << endl;
        
        prob = rand() % 100 + 1; 
        if (prob <= 40)  //customer is served
            line.is_served(0);
        
        prob = rand() % 100 + 1; 
        if (prob <= 60)  //customer joins line
            line.join_line(pickName());
        
        prob = rand() % 100 + 1; 
        if (prob <= 20) //customer leaves end of line
            line.leaveRear();
        
        prob = rand() % 100 + 1; 
        if (prob <= 10){ //customer leaves at random spot of line
             prob = rand() % (line.countLine()) + 1;
            line.delete_pos(prob);
        }
        prob = rand() % 100 + 1; 
        if (prob <= 10)//obnoxious person joins the beginning of line
            line.vipJoin(pickName());
        
        line.print();
    }
    
    return 0;
}

string pickName() {
    ifstream file;
    string tempName;
    int rando = rand() % 99; 
    file.open("names.txt");
    
    if(!file.good()) {
        cerr << "Could not open file";
        file.close();
        return "empty";
    }
    for(int i = 0; i <rando; i++) //traverses list of names until reaches rando index
        getline(file,tempName);

    return tempName;
}