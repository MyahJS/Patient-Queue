// CMSC 341 - Fall 2023 - Project 3
#include "pqueue.h"
PQueue::PQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure)
{
    // Constructor
    // preconditions: none
    // postconditions: create an empty heap

    m_heap = nullptr;
    m_size = 0;
    m_priorFunc = priFn;
    m_heapType = heapType;
    m_structure = structure;
}
PQueue::~PQueue()
{
    // Destructor
    // preconditions: object exists
    // postconditions: deallocate any memory and make object empty

    clear();
}
void PQueue::clear() {
    // Clear
    // preconditons: object exists
    // postconditons: deallocate nodes in the heap and make empty object

    clearRecursive(m_heap); // recursively delete nodes
    m_heap = nullptr;
    m_size = 0;
}
PQueue::PQueue(const PQueue& rhs)
{
    // Copy constructor
    // preconditions: rhs exists
    // postconditions: create a deep copy of the passed heap

    m_heap = nullptr;
    m_size = 0;
    m_priorFunc = rhs.m_priorFunc;
    m_heapType = rhs.m_heapType;
    m_structure = rhs.m_structure;
    if (rhs.m_heap){
        m_heap = copyRecursive(rhs.m_heap); // recursively copy nodes from rhs heap
        m_size = rhs.m_size;
    }
}

PQueue& PQueue::operator=(const PQueue& rhs) 
{
    // Overloaded assignement operator
    // preconditions: rhs and object exist
    // postconditions: copy the passed heap into the current object

    clear();
    m_priorFunc = rhs.m_priorFunc;
    m_heapType = rhs.m_heapType;
    m_structure = rhs.m_structure;
    if (rhs.m_heap){
        m_heap = copyRecursive(rhs.m_heap); // recursively copy nodes from rhs heap
        m_size = rhs.m_size;
    }
}
void PQueue::mergeWithQueue(PQueue& rhs) 
{
    // MergeWithQueue
    // preconditons: rhs exists, is not the same heap, has the same priority function, and has the same data structure
    // postconditions: merge current heap with the passed heap and update height
    //                  then empty rhs 
    
    // domain error if rhs is the same or priority functions are different
    if (this==&rhs)
        throw domain_error("Cannot merge a queue with itself. Merge failed.");

    if (m_priorFunc!=rhs.m_priorFunc)
        throw domain_error("Cannot merge queues with different priority functions. Merge failed.");

    if (m_structure!=rhs.m_structure)
        throw domain_error("Cannot merge queues with different structures. Merge failed.");

    m_heap = mergeRecursive(m_heap, rhs.m_heap);    // recursively merge two heaps
    m_size += rhs.m_size;
    rhs.clear();    // empty rhs
}

void PQueue::insertPatient(const Patient& patient) 
{
    // InsertPatient
    // preconditons: object exists
    // postconditions: insert patient into queue while maintaining heap structure

    Node* newNode = new Node(patient);
    m_heap = mergeRecursive(m_heap, newNode);   // use merge to add into right place in heap
    m_size += 1;
}

int PQueue::numPatients() const
{
    // NumPatients
    // preconditons: object exists
    // postconditions: return size of heap

    return m_size;
}

prifn_t PQueue::getPriorityFn() const 
{
    // GetPriorityFn
    // preconditons: object exists
    // postconditions: return priority function

    return m_priorFunc;
}
Patient PQueue::getNextPatient() 
{
    // GetNextPatient
    // preconditons: object exists and is not empty
    // postconditions: return and remove the top priority patient from the heap

    // out of range error if heap is empty
    if (m_size==0)
        throw out_of_range("Queue is empty. Failed to get the next patient.");

    Patient topPatient = m_heap->getPatient();  // get highest priority patient

    // remove root
    Node* root = m_heap;
    m_heap = mergeRecursive(m_heap->m_left, m_heap->m_right);   // use merge to combine left and right subtrees
    delete root;

    return topPatient;
}

void PQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) 
{
    // SetPriotityFn
    // preconditons: object exists
    // postconditions: set priority function and heap type and rebuild heap with new type

    if (m_priorFunc==priFn && m_heapType==heapType)
        return;

    m_priorFunc = priFn;
    m_heapType = heapType;

    // rebuild heap
    Node* oldHeap = m_heap;
    m_heap = nullptr;
    m_size = 0;

    // insert nodes into a new heap with new type
    while (oldHeap){
        insertPatient(oldHeap->getPatient());
        Node* temp = oldHeap;
        oldHeap = mergeRecursive(oldHeap->m_left, oldHeap->m_right);
        delete temp;    // deallocate old heap along the way
    }
}

void PQueue::setStructure(STRUCTURE structure)
{
    // SetStructure
    // preconditons: object exists 
    // postconditions: set structure and rebuild heap with the new structures

    if (m_structure==structure)
        return;

    m_structure = structure;

    // rebuild heap
    Node* oldHeap = m_heap;
    m_heap = nullptr;
    m_size = 0;

    while (oldHeap){
        insertPatient(oldHeap->getPatient());
        Node* temp = oldHeap;
        oldHeap = mergeRecursive(oldHeap->m_left, oldHeap->m_right);
        delete temp;    
    }
}

STRUCTURE PQueue::getStructure() const 
{
    // GetStructure
    // preconditons: object exists
    // postconditions: return structure

    return m_structure;
}

HEAPTYPE PQueue::getHeapType() const 
{
    // GetHeapType
    // preconditons: object exists
    // postconditions: return heap type

    return m_heapType;
}

void PQueue::printPatientQueue() const 
{
    // PrintPatientQueue
    // preconditons: object exists
    // postconditions: use preorder traversal to print out patients in the heap

    cout << "Contents of the queue:" << endl;
    printRecursive(m_heap); // recursively print nodes 
    cout << endl;
}

void PQueue::dump() const {
  if (m_size == 0) {
    cout << "Empty heap.\n" ;
  } else {
    dump(m_heap);
  }
  cout << endl;
}
void PQueue::dump(Node *pos) const {
  if ( pos != nullptr ) {
    cout << "(";
    dump(pos->m_left);
    if (m_structure == SKEW)
        cout << m_priorFunc(pos->m_patient) << ":" << pos->m_patient.getPatient();
    else
        cout << m_priorFunc(pos->m_patient) << ":" << pos->m_patient.getPatient() << ":" << pos->m_npl;
    dump(pos->m_right);
    cout << ")";
  }
}

ostream& operator<<(ostream& sout, const Patient& patient) {
  sout  << patient.getPatient() 
        << ", temperature: " << patient.getTemperature()
        << ", oxygen: " << patient.getOxygen()
        << ", respiratory: " << patient.getRR()
        << ", blood pressure: " << patient.getBP()
        << ", nurse opinion: " << patient.getOpinion();
  return sout;
}
ostream& operator<<(ostream& sout, const Node& node) {
  sout << node.getPatient();
  return sout;
}