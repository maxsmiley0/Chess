#include "Set.h"
#include <iostream>

using namespace std;

Set::Set()
{
    head = nullptr; //creates an empty set, i.e. no nodes, head points to nullptr
    mSize = 0;
}

Set::~Set()
{
    while (head != nullptr) //loop untill the head is the nullptr
    {
        Node* p = head;     //temporary pointer to the head
        head = p->next;     //head of the list is moved to the next element
        delete p;           //delete the previous end
    }                       //end result: no nodes, and head points to the nullptr
}

Set::Set(const Set& other)
{
    head = nullptr;         //head points to the nullptr
    
    for (Node* p = other.head; p != nullptr; p = p->next) //loops through the other set
    {
        insert(p->value);   //inserts each element of other set into this set
    }                       //insert also define the next and prev pointer for each node
    mSize = other.mSize;
}

Set& Set::operator=(const Set& other) //assigns an existing set to another
{
    if (this != &other)               //skips the middleman if both sets are already the same
    {
        Set tempSet(other);           //temporary set equal to the other set
        swap(tempSet);                //swaps tempSet with this set
    }                                 //local set destructor is called, tempSet nodes deleted
    return *this;                     //returns our newly swapped set
}

bool Set::empty() const
{
    if (head == nullptr)              //if head == nullptr then the set is empty
    {
        return true;
    }
    return false;                     //if the head != nullptr then the set has some nodes,
}                                     //and therefore is not empty

int Set::size() const
{
    return mSize;
}

bool Set::insert(const Coord& value)
{
    if (head == nullptr)              //Special Case: Empty Set
    {
        mSize = 1;
        head = new Node;              //declare a new node
        head->value = value;          //initialize its value
        head->next = nullptr;         //set the prev pointer to nullptr (since it's the head)
    }
    else
    {
        mSize++;
        Node* p = head;               //General Case: Set already has some elements (or one)
        Node* newNode = new Node;     //Allocate memory for new node
        while (p->next != nullptr)    //Loop until we get to the end of the linked list
        {
            p = p->next;
        }
        newNode->value = value;       //Assign newNode value to the value passed by parameter
        p->next = newNode;            //p is pointing to the tail, so we extend the list
        newNode->next = nullptr;      //newNode is the new tail, so next should be nullptr
    }                                 //node
    return true;                      //return true, because insertion was successful
}

bool Set::contains(const Coord& value) const
{
    Node* p = head;                 //This will traverse the list until we find value
    
    while(p != nullptr)             //p cannot go past the end of the list
    {
        if (p->value == value)      //return true if value is found
        {
            return true;
        }
        p = p->next;                //iterate through the list
    }
    return false;                   //return false if p is not found
}

Coord Set::get(int i) const
{
    if (i < 0 || i >= size())
    {
        cerr << "Index out of bounds in Set::get(int)" << endl;
        exit(1);
    }
    
    Node* p = head;
    
    for (int j = 0; j < i; j++)
    {
        p = p->next;
    }
    
    return p->value;
}

void Set::swap(Set& other)
{
    Node* tempHead = head; //temporary node pointing towards this object's head
    head = other.head; //set this object's head to the other head
    other.head = tempHead; //set the other head to the temp head
}
//This works, because each head Nodes inherit the next and previous pointers, which
//in turn inherits the next and previous pointers, etc. etc

void Set::dump() const
{
    Node* p = head;
    while (p != nullptr)
    {
        cout << p->value.getX() << " " << p->value.getY() << endl;
        p = p->next;
    }
}
