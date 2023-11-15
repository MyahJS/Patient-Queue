// CMSC 341 - Fall 2023 - Project 3

#include "pqueue.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>
using namespace std;

// Priority functions compute an integer priority for a patient.  Internal
// computations may be floating point, but must return an integer.

int priorityFn1(const Patient & patient);
int priorityFn2(const Patient & patient);

// a name database for testing purposes
const int NUMNAMES = 20;
string nameDB[NUMNAMES] = {
    "Ismail Carter", "Lorraine Peters", "Marco Shaffer", "Rebecca Moss",
    "Lachlan Solomon", "Grace Mclaughlin", "Tyrese Pruitt", "Aiza Green", 
    "Addie Greer", "Tatiana Buckley", "Tyler Dunn", "Aliyah Strong", 
    "Alastair Connolly", "Beatrix Acosta", "Camilla Mayo", "Fletcher Beck",
    "Erika Drake", "Libby Russo", "Liam Taylor", "Sofia Stewart"
};

// We can use the Random class to generate the test data randomly!
enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor 
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else if (type == UNIFORMREAL) { //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
        else { //the case of SHUFFLE to generate every number only once
            m_generator = std::mt19937(m_device());
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }

    void getShuffle(vector<int> & array){
        // the user program creates the vector param and passes here
        // here we populate the vector using m_min and m_max
        for (int i = m_min; i<=m_max; i++){
            array.push_back(i);
        }
        shuffle(array.begin(),array.end(),m_generator);
    }

    void getShuffle(int array[]){
        // the param array must be of the size (m_max-m_min+1)
        // the user program creates the array and pass it here
        vector<int> temp;
        for (int i = m_min; i<=m_max; i++){
            temp.push_back(i);
        }
        std::shuffle(temp.begin(), temp.end(), m_generator);
        vector<int>::iterator it;
        int i = 0;
        for (it=temp.begin(); it != temp.end(); it++){
            array[i] = *it;
            i++;
        }
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }
    
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};

class Tester{
    public:

    bool insertTest(){
        bool all_result = true;
        bool result;

        // case 1: min heap

        result = true;
        // create a min heap
        Random nameGen(0,NUMNAMES-1);
        Random temperatureGen(MINTEMP,MAXTEMP);
        Random oxygenGen(MINOX,MAXOX);
        Random respiratoryGen(MINRR,MAXRR);
        Random bloodPressureGen(MINBP,MAXBP);
        Random nurseOpinionGen(MINOPINION,MAXOPINION);
        PQueue aQueue(priorityFn2, MINHEAP, LEFTIST);
        for (int i=0;i<300;i++){
            Patient patient(nameDB[nameGen.getRandNum()],
                        temperatureGen.getRandNum(),
                        oxygenGen.getRandNum(),
                        respiratoryGen.getRandNum(),
                        bloodPressureGen.getRandNum(),
                        nurseOpinionGen.getRandNum());
            aQueue.insertPatient(patient);
        }

        // check if min heap property upheld
        result = aQueue.isMinHeap(aQueue.m_heap, aQueue.m_priorFunc);

        cout << endl << "Insert normal case (min heap): ";
            if (result){
                cout << "PASS" << endl;
            } else {
                cout << "FAIL" << endl;
            }

        all_result = all_result && result;

        // case 2: max heap

        result = true;
        PQueue bQueue(priorityFn1, MAXHEAP, LEFTIST);
        for (int i=0;i<300;i++){
            Patient patient(nameDB[nameGen.getRandNum()],
                        temperatureGen.getRandNum(),
                        oxygenGen.getRandNum(),
                        respiratoryGen.getRandNum(),
                        bloodPressureGen.getRandNum(),
                        nurseOpinionGen.getRandNum());
            bQueue.insertPatient(patient);
        }

        result = bQueue.isMaxHeap(bQueue.m_heap, bQueue.m_priorFunc);

        cout << endl << "Insert normal case (max heap): ";
            if (result){
                cout << "PASS" << endl;
            } else {
                cout << "FAIL" << endl;
            }

        all_result = all_result && result;

        return all_result;
    }
    bool removeTest(){
        bool all_result = true;
        bool result;

        // case 1: min heap

        result = true;
        // create a min heap
        Random nameGen(0,NUMNAMES-1);
        Random temperatureGen(MINTEMP,MAXTEMP);
        Random oxygenGen(MINOX,MAXOX);
        Random respiratoryGen(MINRR,MAXRR);
        Random bloodPressureGen(MINBP,MAXBP);
        Random nurseOpinionGen(MINOPINION,MAXOPINION);
        PQueue aQueue(priorityFn2, MINHEAP, LEFTIST);
        for (int i=0;i<300;i++){
            Patient patient(nameDB[nameGen.getRandNum()],
                        temperatureGen.getRandNum(),
                        oxygenGen.getRandNum(),
                        respiratoryGen.getRandNum(),
                        bloodPressureGen.getRandNum(),
                        nurseOpinionGen.getRandNum());
            aQueue.insertPatient(patient);
        }

        // remove nodes and check if each is greater than the last
        int prev = aQueue.m_priorFunc(aQueue.getNextPatient()); // will hold priority of the last removed node
        while (aQueue.m_heap){
            if (prev>aQueue.m_priorFunc(aQueue.m_heap->getPatient()))
                result = false;
            prev = aQueue.m_priorFunc(aQueue.getNextPatient());
        }

        cout << endl << "Remove test normal case (min heap): ";
            if (result){
                cout << "PASS" << endl;
            } else {
                cout << "FAIL" << endl;
            }

        all_result = all_result && result;

        // case 2: max heap

        result = true;
        PQueue bQueue(priorityFn1, MAXHEAP, LEFTIST);
        for (int i=0;i<300;i++){
            Patient patient(nameDB[nameGen.getRandNum()],
                        temperatureGen.getRandNum(),
                        oxygenGen.getRandNum(),
                        respiratoryGen.getRandNum(),
                        bloodPressureGen.getRandNum(),
                        nurseOpinionGen.getRandNum());
            bQueue.insertPatient(patient);
        }

        prev = bQueue.m_priorFunc(bQueue.getNextPatient()); 
        while (bQueue.m_heap){
            if (prev<bQueue.m_priorFunc(bQueue.m_heap->getPatient()))
                result = false;
            prev = bQueue.m_priorFunc(bQueue.getNextPatient());
        }

        cout << endl << "Remove test normal case (max heap): ";
            if (result){
                cout << "PASS" << endl;
            } else {
                cout << "FAIL" << endl;
            }

        all_result = all_result && result;

        // case 3: error case

        result = true;
        // create empty queue
        PQueue cQueue(priorityFn1, MAXHEAP, LEFTIST);

        try{
            cQueue.getNextPatient();
        } catch(out_of_range& e){
            result = result && (string(e.what())=="Queue is empty. Failed to get the next patient.");
        }

        cout << endl << "Remove test error case: ";
            if (result){
                cout << "PASS" << endl;
            } else {
                cout << "FAIL" << endl;
            }

        all_result = all_result && result;

        return all_result;
    }
    bool leftistTest(){
        bool all_result = true;
        bool result;

        //create leftist heap
        Random nameGen(0,NUMNAMES-1);
        Random temperatureGen(MINTEMP,MAXTEMP);
        Random oxygenGen(MINOX,MAXOX);
        Random respiratoryGen(MINRR,MAXRR);
        Random bloodPressureGen(MINBP,MAXBP);
        Random nurseOpinionGen(MINOPINION,MAXOPINION);
        PQueue aQueue(priorityFn2, MINHEAP, LEFTIST);
        for (int i=0;i<300;i++){
            Patient patient(nameDB[nameGen.getRandNum()],
                        temperatureGen.getRandNum(),
                        oxygenGen.getRandNum(),
                        respiratoryGen.getRandNum(),
                        bloodPressureGen.getRandNum(),
                        nurseOpinionGen.getRandNum());
            aQueue.insertPatient(patient);
        }

        // check that nodes have accurate npl
        result = aQueue.accurateNPL(aQueue.m_heap);

        cout << endl << "Leftist heap NPL accuracy test: ";
            if (result){
                cout << "PASS" << endl;
            } else {
                cout << "FAIL" << endl;
            }

        all_result = all_result && result;

        // check that heap is leftist
        result = aQueue.isLeftist(aQueue.m_heap);

        cout << endl << "Leaftist heap property test: ";
            if (result){
                cout << "PASS" << endl;
            } else {
                cout << "FAIL" << endl;
            }

        all_result = all_result && result;

        return all_result;
    }
    bool setPriorityTest(){
        bool all_result = true;
        bool result;

        // create minheap with priorFN2
        Random nameGen(0,NUMNAMES-1);
        Random temperatureGen(MINTEMP,MAXTEMP);
        Random oxygenGen(MINOX,MAXOX);
        Random respiratoryGen(MINRR,MAXRR);
        Random bloodPressureGen(MINBP,MAXBP);
        Random nurseOpinionGen(MINOPINION,MAXOPINION);
        PQueue aQueue(priorityFn2, MINHEAP, LEFTIST);
        for (int i=0;i<10;i++){
            Patient patient(nameDB[nameGen.getRandNum()],
                        temperatureGen.getRandNum(),
                        oxygenGen.getRandNum(),
                        respiratoryGen.getRandNum(),
                        bloodPressureGen.getRandNum(),
                        nurseOpinionGen.getRandNum());
            aQueue.insertPatient(patient);
        }

        // now set to max heap with priorFN1
        aQueue.setPriorityFn(priorityFn1, MAXHEAP);

        // check if max heap
        result = aQueue.isMaxHeap(aQueue.m_heap, aQueue.m_priorFunc);

        cout << endl << "SetPriority test: ";
            if (result){
                cout << "PASS" << endl;
            } else {
                cout << "FAIL" << endl;
            }

        all_result = all_result && result;

        return all_result;
    }
    bool mergeTest(){
        bool all_result = true;
        bool result;

        // case 1: edge case
        
        result = true;
        // create empty queue
        PQueue aQueue(priorityFn1, MAXHEAP, SKEW);

        // create valid populated queue
        Random nameGen(0,NUMNAMES-1);
        Random temperatureGen(MINTEMP,MAXTEMP);
        Random oxygenGen(MINOX,MAXOX);
        Random respiratoryGen(MINRR,MAXRR);
        Random bloodPressureGen(MINBP,MAXBP);
        Random nurseOpinionGen(MINOPINION,MAXOPINION);
        PQueue bQueue(priorityFn1, MAXHEAP, SKEW);
        for (int i=0;i<10;i++){
            Patient patient(nameDB[nameGen.getRandNum()],
                        temperatureGen.getRandNum(),
                        oxygenGen.getRandNum(),
                        respiratoryGen.getRandNum(),
                        bloodPressureGen.getRandNum(),
                        nurseOpinionGen.getRandNum());
            bQueue.insertPatient(patient);
        }

        aQueue.mergeWithQueue(bQueue);
        // check that first queue populated and second empty (merge occured)
        result = aQueue.m_size==10 && bQueue.m_size==0;

        cout << endl << "Merge test edge case: ";
            if (result){
                cout << "PASS" << endl;
            } else {
                cout << "FAIL" << endl;
            }

        all_result = all_result && result;

        // case 2: error case 

        result = true;
        // create two queues with different priority functions
        PQueue cQueue(priorityFn1, MAXHEAP, LEFTIST);
        PQueue dQueue(priorityFn2, MINHEAP, LEFTIST);

        try{
            cQueue.mergeWithQueue(dQueue);
        } catch (domain_error& e) {
            result = result && (string(e.what())=="Cannot merge queues with different priority functions. Merge failed.");
        }

        cout << endl << "Merge test error case: ";
            if (result){
                cout << "PASS" << endl;
            } else {
                cout << "FAIL" << endl;
            }

        all_result = all_result && result;
    }
    bool copyTest(){
        bool all_result = true;
        bool result;

        // case 1: normal case

        result = true;
        // create populated queue
        Random nameGen(0,NUMNAMES-1);
        Random temperatureGen(MINTEMP,MAXTEMP);
        Random oxygenGen(MINOX,MAXOX);
        Random respiratoryGen(MINRR,MAXRR);
        Random bloodPressureGen(MINBP,MAXBP);
        Random nurseOpinionGen(MINOPINION,MAXOPINION);
        PQueue aQueue(priorityFn2, MINHEAP, LEFTIST);
        for (int i=0;i<10;i++){
            Patient patient(nameDB[nameGen.getRandNum()],
                        temperatureGen.getRandNum(),
                        oxygenGen.getRandNum(),
                        respiratoryGen.getRandNum(),
                        bloodPressureGen.getRandNum(),
                        nurseOpinionGen.getRandNum());
            aQueue.insertPatient(patient);
        }

        // create new queue using populated queue
        PQueue bQueue(aQueue);

        // check that queues are equal
        result = result && bQueue.isEqual(bQueue.m_heap, aQueue.m_heap);

        cout << endl << "Copy constructor test normal case: ";
            if (result){
                cout << "PASS" << endl;
            } else {
                cout << "FAIL" << endl;
            }

        all_result = all_result && result;

        // case 2: edge case

        result = true;
        // create empty queue
        PQueue cQueue(priorityFn1, MAXHEAP, LEFTIST);

        // create heap using empty heap
        PQueue dQueue(cQueue);

        // check that member variables are equal
        result = result && (dQueue.m_priorFunc==priorityFn1) && (dQueue.m_heapType==MAXHEAP) && (dQueue.m_structure==LEFTIST);

        cout << endl << "Copy constructor test edge case: ";
            if (result){
                cout << "PASS" << endl;
            } else {
                cout << "FAIL" << endl;
            }

        all_result = all_result && result;

        return all_result;
    }
    bool assignmentTest(){
        bool all_result = true;
        bool result;

        // case 1: normal case

        result = true;
        // create populated queue
        Random nameGen(0,NUMNAMES-1);
        Random temperatureGen(MINTEMP,MAXTEMP);
        Random oxygenGen(MINOX,MAXOX);
        Random respiratoryGen(MINRR,MAXRR);
        Random bloodPressureGen(MINBP,MAXBP);
        Random nurseOpinionGen(MINOPINION,MAXOPINION);
        PQueue aQueue(priorityFn2, MINHEAP, LEFTIST);
        for (int i=0;i<10;i++){
            Patient patient(nameDB[nameGen.getRandNum()],
                        temperatureGen.getRandNum(),
                        oxygenGen.getRandNum(),
                        respiratoryGen.getRandNum(),
                        bloodPressureGen.getRandNum(),
                        nurseOpinionGen.getRandNum());
            aQueue.insertPatient(patient);
        }

        // assign populated queue to an empty queue
        PQueue bQueue(priorityFn2, MINHEAP, LEFTIST);
        bQueue = aQueue;

        // check that queues are equal
        result = result && bQueue.isEqual(bQueue.m_heap, aQueue.m_heap);

        cout << endl << "Assignment operator test normal case: ";
            if (result){
                cout << "PASS" << endl;
            } else {
                cout << "FAIL" << endl;
            }

        all_result = all_result && result;

        // case 2: edge case

        result = true;
        PQueue cQueue(priorityFn1, MAXHEAP, SKEW);
        for (int i=0;i<10;i++){
            Patient patient(nameDB[nameGen.getRandNum()],
                        temperatureGen.getRandNum(),
                        oxygenGen.getRandNum(),
                        respiratoryGen.getRandNum(),
                        bloodPressureGen.getRandNum(),
                        nurseOpinionGen.getRandNum());
            cQueue.insertPatient(patient);
        }

        // assign empty queue to the populated queue
        PQueue dQueue(priorityFn1, MAXHEAP, SKEW);
        cQueue = dQueue;

        // check that populated queue is now empty
        result = result && (!cQueue.m_heap);

        cout << endl << "Assignment operator test edge case: ";
            if (result){
                cout << "PASS" << endl;
            } else {
                cout << "FAIL" << endl;
            }

        all_result = all_result && result;

        return all_result;
    }
};

int main(){
    Tester test;

    test.insertTest();
    test.removeTest();
    test.leftistTest();
    test.mergeTest();
    test.setPriorityTest();
    test.copyTest();
    test.assignmentTest();

    Random nameGen(0,NUMNAMES-1);
    Random temperatureGen(MINTEMP,MAXTEMP);
    Random oxygenGen(MINOX,MAXOX);
    Random respiratoryGen(MINRR,MAXRR);
    Random bloodPressureGen(MINBP,MAXBP);
    Random nurseOpinionGen(MINOPINION,MAXOPINION);
    PQueue aQueue(priorityFn2, MINHEAP, LEFTIST);
    for (int i=0;i<10;i++){
        Patient patient(nameDB[nameGen.getRandNum()],
                    temperatureGen.getRandNum(),
                    oxygenGen.getRandNum(),
                    respiratoryGen.getRandNum(),
                    bloodPressureGen.getRandNum(),
                    nurseOpinionGen.getRandNum());
        aQueue.insertPatient(patient);
    }
    cout << "\nDump of the leftist heap queue with priorityFn2 (MINHEAP):\n";
    aQueue.dump();
    aQueue.setStructure(SKEW);
    cout << "\nDump of the skew heap queue with priorityFn2 (MINHEAP):\n";
    aQueue.dump();
    cout << "\nPreorder traversal of the nodes in the queue with priorityFn2 (MINHEAP):\n";
    aQueue.printPatientQueue();
    aQueue.setPriorityFn(priorityFn1, MAXHEAP);
    cout << "\nDump of the skew queue with priorityFn1 (MAXHEAP):\n";
    aQueue.dump();

    return 0;
}

int priorityFn1(const Patient & patient) {
    //this function works with a MAXHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [115-242]
    //temperature + respiratory + blood pressure
    //the highest priority would be 42+40+160 = 242
    //the lowest priority would be 35+10+70 = 115
    //the larger value means the higher priority
    int priority = patient.getTemperature() + patient.getRR() + patient.getBP();
    return priority;
}

int priorityFn2(const Patient & patient) {
    //this function works with a MINHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [71-111]
    //nurse opinion + oxygen
    //the highest priority would be 1+70 = 71
    //the lowest priority would be 10+101 = 111
    //the smaller value means the higher priority
    int priority = patient.getOpinion() + patient.getOxygen();
    return priority;
}