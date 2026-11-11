/*
 * Name: Amina Ahmad
 * Registration No: 542601
 * Section: BSAI-2A
 *
 * DESIGN EXPLANATION (Task 1)
 * - I stored Student records in a Binary Search Tree (BST) keyed by roll
 *   number. Roll numbers are unique and the desk mostly looks students up,
 *   so a BST gives fast search and insert operations on average, and an in-order traversal
 *   prints students sorted by roll number for free.
 * - For additional feature i picked help requests,for that i use a linked-list Queue (FIFO):
 *   the earliest request waiting is served first, repeated requests are
 *   allowed, and there is no fixed size, so no "full" case.
 *
 * COMPLEXITY
 * - Add student:    O(h) -> worst case O(n) when the BST degenerates into a
 *                   chain (e.g. roll numbers inserted in sorted order).
 *                   Average case O(log n).
 * - Search student: O(h) -> worst case O(n), average O(log n).
 * - Enqueue/Dequeue request: O(1).
 * - Display all:    O(n).
 */
#include <iostream>
#include <string>
using namespace std;

// ---------- Student BST ----------
struct Student {
    int roll;
    string name;
    Student* left, * right;
    Student(int r, const string& n) : roll(r), name(n), left(nullptr), right(nullptr) {}
};

// Returns false if roll number already exists. Worst case O(n).
bool addStudent(Student*& root, int roll, const string& name) {
    if (!root) { root = new Student(roll, name); return true; }
    Student* cur = root;
    while (true) {
        if (roll == cur->roll) return false;
        Student*& next = (roll < cur->roll) ? cur->left : cur->right;
        if (!next) { next = new Student(roll, name); return true; }
        cur = next;
    }
}

// Returns pointer or nullptr. Worst case O(n).
Student* searchStudent(Student* root, int roll) {
    while (root) {
        if (roll == root->roll) return root;
        root = (roll < root->roll) ? root->left : root->right;
    }
    return nullptr;
}

// In-order traversal: sorted by roll number.
void displayAll(Student* root) {
    if (!root) return;
    displayAll(root->left);
    cout << "  Roll " << root->roll << " - " << root->name << "\n";
    displayAll(root->right);
}

void freeTree(Student* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

// ---------- Help request Queue (linked list, FIFO) ----------
struct Node {
    int roll;
    Node* next;
    Node(int r) : roll(r), next(nullptr) {}
};

struct Queue {
    Node* front, * rear;
    Queue() : front(nullptr), rear(nullptr) {}
    bool empty() const { return front == nullptr; }
    void enqueue(int roll) {            // O(1)
        Node* n = new Node(roll);
        if (rear) rear->next = n; else front = n;
        rear = n;
    }
    int dequeue() {                     // O(1), call only if not empty
        Node* n = front;
        int roll = n->roll;
        front = front->next;
        if (!front) rear = nullptr;
        delete n;
        return roll;
    }
    void display() const {
        if (empty()) { cout << "  (no pending requests)\n"; return; }
        int pos = 1;
        for (Node* c = front; c; c = c->next)
            cout << "  " << pos++ << ". Roll " << c->roll << "\n";
    }
    ~Queue() { while (!empty()) dequeue(); }
};

// ---------- Menu ----------
void printMenu() {
    cout << "\n===== Student Help Desk =====\n"
        << "1. Add student\n"
        << "2. Search student by roll number\n"
        << "3. Display all students\n"
        << "4. Add help request\n"
        << "5. Serve next help request\n"
        << "6. Show pending requests\n"
        << "0. Exit\n"
        << "Choice: ";
}

int main() {
    Student* root = nullptr;
    Queue requests;
    int choice;

    do {
        printMenu();
        cin >> choice;
        switch (choice) {
        case 1: {
            int roll; string name;
            cout << "Roll number: "; cin >> roll;
            cout << "Name: "; cin.ignore(); getline(cin, name);
            if (addStudent(root, roll, name)) cout << "Student added.\n";
            else cout << "Error: roll number " << roll << " already exists.\n";
            break;
        }
        case 2: {
            int roll;
            cout << "Roll number to search: "; cin >> roll;
            Student* s = searchStudent(root, roll);
            if (s) cout << "Found: Roll " << s->roll << " - " << s->name << "\n";
            else cout << "Student with roll number " << roll << " not found.\n";
            break;
        }
        case 3:
            if (!root) cout << "No students registered.\n";
            else { cout << "Students (sorted by roll number):\n"; displayAll(root); }
            break;
        case 4: {
            int roll;
            cout << "Roll number requesting help: "; cin >> roll;
            if (!searchStudent(root, roll))
                cout << "Rejected: roll number " << roll << " is not registered.\n";
            else { requests.enqueue(roll); cout << "Request added.\n"; }
            break;
        }
        case 5:
            if (requests.empty()) cout << "No pending requests.\n";
            else {
                int roll = requests.dequeue();
                Student* s = searchStudent(root, roll);
                cout << "Serving: Roll " << roll << " - " << (s ? s->name : "?") << "\n";
            }
            break;
        case 6:
            cout << "Pending requests (front = next to be served):\n";
            requests.display();
            break;
        case 0:
            cout << "Goodbye.\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    freeTree(root);   // queue is freed by its destructor
    return 0;
}