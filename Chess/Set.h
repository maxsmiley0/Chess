#ifndef Set_h
#define Set_h

#include <string>
#include "Coord.h"

class Set
{
public:
    Set();
    //empty constructor
    ~Set();
    //destructor, should delete head node
    Set(const Set& other);
    //creates a new Set object from another Set object
    Set& operator=(const Set& other);
    //sets the value of an existing  set to another set
    bool empty() const;
    //return true if empty, false if has some elements
    int size() const;
    //return size of Set
    bool insert(const Coord& value);
    //returns true if value was inserted, false otherwise
    //can only be inserted if value not yet in set
    bool contains(const Coord& value) const;
    //Returns true if a set contains a Coord
    Coord get(int i) const;
    //Returns the ith Coord of the Set
    void swap(Set& other);
    //swaps elements and size of arrays
    //useful for assignment operator
    void dump() const; //tester fnct to print all elms of set to cout
    
private:
    struct Node
    {
        Coord value;
        Node* next;
    };
    
    Node* head;
};

#endif /* Set_h */
