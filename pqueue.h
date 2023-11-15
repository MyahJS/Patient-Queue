// CMSC 341 - Fall 2023 - Project 3
#ifndef PQUEUE_H
#define PQUEUE_H

#include <stdexcept>
#include <iostream>
#include <string>
using namespace std;

class Grader; // forward declaration (for grading purposes)
class Tester; // forward declaration (for test functions)
class PQueue; // forward declaration
class Patient;// forward declaration
#define EMPTY Patient() // This is an empty object (invalid patient)
enum HEAPTYPE {MINHEAP, MAXHEAP};
enum STRUCTURE {SKEW, LEFTIST};
// Priority function pointer type
typedef int (*prifn_t)(const Patient&);

// Triage parameters, min and max values
const int MINTEMP = 35; // Body temperature, celsius
const int MAXTEMP = 42;
const int MINOX = 70;   // Level of oxygen saturation (SpO2)
const int MAXOX = 101;
const int MINRR = 10;   // Respiratory Rate, per minute
const int MAXRR = 40;
const int MINBP = 70;   // Blood Pressure
const int MAXBP = 160;
const int MINOPINION = 1;   // Nurse opinion, between 1 - 10
const int MAXOPINION = 10;  // 1 is highest priotity
//
// patient class
//
class Patient {
    public:
    friend class Grader; // for grading purposes
    friend class Tester; // contains test functions
    friend class PQueue;
    Patient() {
        // This is an empty object since name is empty
        m_patient = ""; m_temperature = 37; m_oxygen = 100;
        m_RR = 20;m_BP = 100;m_opinion=10;
    }
    Patient(string name, int temp, int ox, int rr, int bp, int op) {
        if ( (temp < MINTEMP || temp > MAXTEMP) || 
        (ox < MINOX || ox > MAXOX) || (rr < MINRR || rr > MAXRR) || 
        (bp < MINBP || bp > MAXBP) || (op < 1 || op > 10)){
            // create an empty object
            m_patient = ""; m_temperature = 37; m_oxygen = 100;
            m_RR = 20; m_BP = 100;m_opinion=10;
        }
        else{
            m_patient = name; m_temperature = temp; m_oxygen = ox;
            m_RR = rr; m_BP = bp;m_opinion=op;
        }
    }
    string getPatient() const {return m_patient;}
    int getTemperature() const {return m_temperature;}
    int getOxygen() const {return m_oxygen;}
    int getRR() const {return m_RR;}
    int getBP() const {return m_BP;}
    int getOpinion() const {return m_opinion;}
    void setPatient(string name) {m_patient=name;}
    void setTemperature(int val) {m_temperature=val;}
    void setOxygen(int val) {m_oxygen=val;}
    void setRR(int val) {m_RR=val;}
    void setBP(int val) {m_BP=val;}
    void setOpinion(int val) {m_opinion=val;}
    // Overloaded assignment operator
    const Patient & operator=(const Patient& rhs){
        if (this != &rhs){
            m_patient = rhs.m_patient;
            m_temperature = rhs.m_temperature;
            m_oxygen = rhs.m_oxygen;
            m_RR = rhs.m_RR;
            m_BP = rhs.m_BP;
            m_opinion = rhs.m_opinion;
        }
        return *this;
    }
    // Overloaded equality operator
    bool operator==(const Patient & rhs) const {
        return ((m_patient == rhs.m_patient) &&
                (m_temperature == rhs.m_temperature) &&
                (m_oxygen == rhs.m_oxygen) &&
                (m_RR == rhs.m_RR) &&
                (m_BP == rhs.m_BP) &&
                (m_opinion == rhs.m_opinion));
    }
    // Overloaded insertion operator
    friend ostream& operator<<(ostream& sout, const Patient& patient);

    private:
    string m_patient;  // Patient's name, no need to be unique
    int m_temperature; // Body temperature, celsius
    int m_oxygen;      // Level of oxygen saturation (SpO2), percentage
    int m_RR;          // Respiratory Rate, per minute
    int m_BP;          // Blood Pressure
    int m_opinion;     // Nurse opinion, 1 - 10
};

class Node {
    // this is a node in the skew/leftist heap
    public:
    friend class Grader; // for grading purposes
    friend class Tester; // contains test functions
    friend class PQueue;
    Node(Patient patient) {  
        m_patient = patient;
        m_right = nullptr;
        m_left = nullptr;
        m_npl = 0;
    }
    Patient getPatient() const {return m_patient;}
    void setNPL(int npl) {m_npl = npl;}
    int getNPL() const {return m_npl;}

    // Overloaded insertion operator
    friend ostream& operator<<(ostream& sout, const Node& node);

    private:
    Patient m_patient;   // Patient information
    Node *m_right;       // Right child
    Node *m_left;        // Left child
    int m_npl;           // null path length for leftist heap
};

class PQueue {
    // stores the skew/leftist heap, minheap/maxheap
public:
    friend class Grader; // for grading purposes
    friend class Tester; // contains test functions
    PQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure);
    ~PQueue();
    PQueue(const PQueue& rhs);
    PQueue& operator=(const PQueue& rhs);
    void insertPatient(const Patient& input);
    Patient getNextPatient();
    void mergeWithQueue(PQueue& rhs);
    void clear();
    int numPatients() const;
    // Print the queue using preorder traversal.  Although the first patient
    // printed should have the highest priority, the remaining patients will
    // not necessarily be in priority order.
    void printPatientQueue() const;
    prifn_t getPriorityFn() const;
    // Set a new priority function.  Must rebuild the heap!!!
    void setPriorityFn(prifn_t priFn, HEAPTYPE heapType);
    HEAPTYPE getHeapType() const;
    STRUCTURE getStructure() const;
    // Set a new data structure (skew/leftist). Must rebuild the heap!!!
    void setStructure(STRUCTURE structure);
    void dump() const;  // For debugging purposes.

private:
    Node * m_heap;          // Pointer to root of skew heap
    int m_size;             // Current size of the heap
    prifn_t m_priorFunc;    // Function to compute priority
    HEAPTYPE m_heapType;    // either a MINHEAP or a MAXHEAP
    STRUCTURE m_structure;  // skew heap or leftist heap

    void dump(Node *pos) const; // helper function for dump

    /******************************************
    * Private function declarations go here! *
    ******************************************/

    // helper for recursively deleting nodes
    void clearRecursive(Node *root){
        if (root){
            clearRecursive(root->m_left);
            clearRecursive(root->m_right);
            delete root;
        }
    }
    // helper for recursively copying nodes into a new heap
    Node* copyRecursive(Node *rhsNode){
        if (rhsNode){
            Node *newNode = new Node(rhsNode->getPatient());
            newNode->m_left = copyRecursive(rhsNode->m_left);
            newNode->m_right = copyRecursive(rhsNode->m_right);
            newNode->setNPL(rhsNode->getNPL());

            return newNode;
        }
        return nullptr;
    }
    // helper for recursively merging two compatible heaps
    Node* mergeRecursive(Node *left, Node* right){
        if (!left)
            return right;
        
        if (!right)
            return left;

        if (m_heapType==MINHEAP){
            // for a min heap, the node with higher priority goes on left
            if (m_priorFunc(left->getPatient())<=m_priorFunc(right->getPatient()))
                swap(left, right);  // switch pointers to opposite nodes
        } else {
            // for a max heap, the node with higher priority goes on right
            if (m_priorFunc(left->getPatient())>=m_priorFunc(right->getPatient()))
                swap(left, right);
        }

        if (m_structure==SKEW){
            left->m_right = mergeRecursive(left->m_right, right);
            swap(left->m_left, left->m_right);
        } else {
            left->m_right = mergeRecursive(left->m_right, right);

            // check and fix null path length if leftist
            if (!left->m_left || (left->m_left->getNPL()<left->m_right->getNPL()))
                swap(left->m_left, left->m_right);

            // update length
            if (left->m_right){
                left->setNPL(left->m_right->getNPL()+1);
            } else {
                left->setNPL(0);
            }
        }

        return left;
    }
    // helper to recursively print nodes using preorder traversal
    void printRecursive(Node* root) const{
        if (root){
            cout << "[" << m_priorFunc(root->getPatient()) << "]" << root->getPatient() << endl;
            printRecursive(root->m_left);
            printRecursive(root->m_right);
        }
    }
    // helper for checking min heap property
    bool isMinHeap(Node* root, prifn_t priorFn) const{
        if (!root)
            return true;    // empty tree is valid
        
        if (root->m_left){
            if (priorFn(root->m_left->getPatient())<priorFn(root->getPatient()))
                return false;
        }
        if (root->m_right){
            if (priorFn(root->m_right->getPatient())<priorFn(root->getPatient()))
                return false;
        }

        return isMinHeap(root->m_left, priorFn)&&isMinHeap(root->m_right, priorFn);
    }
    // helper for checking max heap property
    bool isMaxHeap(Node* root, prifn_t priorFn) const{
        if (!root)
            return true;
        
        if (root->m_left){
            if (priorFn(root->m_left->getPatient())>priorFn(root->getPatient()))
                return false;
        }
        if (root->m_right){
            if (priorFn(root->m_right->getPatient())>priorFn(root->getPatient()))
                return false;
        }

        return isMinHeap(root->m_left, priorFn)&&isMinHeap(root->m_right, priorFn);
    }
    // helper for checking npl values of nodes in a heap 
    bool accurateNPL(Node* root) const{
        if (!root)
            return true;

        // get npl of left and right child
        int leftNPL = (root->m_left) ? root->m_left->getNPL() : 0;
        int rightNPL = (root->m_right) ? root->m_right->getNPL() : 0;

        // correct npl is minimum npl of children plus 1
        int correctNPL = min(leftNPL, rightNPL) + 1;

        if (root->getNPL()!=correctNPL)
            return false;

        return accurateNPL(root->m_left)&&accurateNPL(root->m_right);
    }
    // helper for checking if a heap is leftist
    bool isLeftist(Node* root) const{
        if (!root)
            return true;

        int leftNPL = (root->m_left) ? root->m_left->getNPL() : 0;
        int rightNPL = (root->m_right) ? root->m_right->getNPL() : 0;

        if (leftNPL<rightNPL)
            return false;

        return isLeftist(root->m_left)&&isLeftist(root->m_right);
    }
    // helper for checking equivalence of two heaps
    bool isEqual(Node* root1, Node* root2){
        if (!root1 && !root2)
            return true;
        
        if (root1&&root2){
            return (root1->getPatient()==root2->getPatient())&&isEqual(root1->m_left, root2->m_left)&&isEqual(root1->m_right, root2->m_right);
        }

        return false;
    }
};

#endif