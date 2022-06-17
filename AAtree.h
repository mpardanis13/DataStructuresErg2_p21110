#include <iostream>
#include <stdlib.h> // Eisagoyme thn bibliothikh dioti xreiazomaste th stathera NULL.
using namespace std;

template <class T>
class AATree;

// Dhlwsh ths klashs twn kombwn toy AA dendroy.
template <class T>
class AATreeNode
{
public:
    // Dhlwsh dyo constructor ths klashs me xrhsh initializer list.
    AATreeNode() : leftChild(NULL), rightChild(NULL), level(1) {}
    AATreeNode(const int d[], T leia,
               AATreeNode *l, AATreeNode *r,
               int lvl = 1)
        : loot(leia), leftChild(l), rightChild(r), level(lvl)
    {
        for (int i = 0; i < 3; i++)
            cell[i] = d[i];
    }
    friend class AATree<T>; // Dinoyme prosbash sta private melh ths klashs AATree.

private:
    T loot;                             // leia
    int cell[3];                        // keli
    int level;                          // epipedo
    AATreeNode *leftChild, *rightChild; // paidia komboy
};

// Dhlwsh ths klashs toy AA dendroy
template <class T>
class AATree
{
public:
    typedef AATreeNode<T> Node;
    AATree();
    ~AATree();
    bool isGreaterThan(const int x[], const int y[]) const;
    T find(const int x[]) const;
    void insert(const int x[], T leia);
    void remove(const int x[]);
    void makeEmpty();

private:
    Node *root; // riza dendroy
    Node *nullNode;

    T elementAt(Node *t) const;

    void insert(const int x[], T leia, Node *&t);
    void remove(const int x[], Node *&t);
    void makeEmpty(Node *&t);

    void skew(Node *&t) const;
    void split(Node *&t) const;

    void rotateWithLeftChild(Node *&t) const;
    void rotateWithRightChild(Node *&t) const;
};

// Dhlwsh constructor klashs AA dendroy.
template <class T>
AATree<T>::
    AATree()
{
    nullNode = new Node;
    nullNode->leftChild = nullNode->rightChild = nullNode;
    nullNode->level = 0;
    root = nullNode;
}

// Dhlwsh destructor ths klashs AA dendroy.
template <class T>
AATree<T>::
    ~AATree()
{
    makeEmpty();
    delete nullNode;
}

// Dhmosia methodo gia th diagrafh enos oloklhroy dendroy
template <class T>
void AATree<T>::
    makeEmpty()
{
    makeEmpty(root);
}

// Idiwtikh methodo gia th diagrafh enos oloklhroy dendroy
template <class T>
void AATree<T>::
    makeEmpty(Node *&t)
{
    if (t != nullNode)
    {
        makeEmpty(t->leftChild);
        makeEmpty(t->rightChild);
        delete t;
    }
    t = nullNode;
}

// Eisagwsh stoixeioy sto dendro
template <class T>
void AATree<T>::
    insert(const int x[], T leia)
{
    insert(x, leia, root);
}

template <class T>
void AATree<T>::
    insert(const int x[], T leia, Node *&t)
{
    if (t == nullNode)
        t = new Node(x, leia, nullNode, nullNode);
    else if (isGreaterThan(t->cell, x))
        insert(x, leia, t->leftChild);
    else if (isGreaterThan(x, t->cell))
        insert(x, leia, t->rightChild);
    else
    {
        t->loot += leia; // Ean to keli yparxei hdh prosthetoyme thn leia sthn hdh yparxoysa
    }

    skew(t);
    split(t);
}

// Diagrafh stoixeioy apo to dendro
template <class T>
void AATree<T>::
    remove(const int x[])
{
    remove(x, root);
}

template <class T>
void AATree<T>::
    remove(const int x[], Node *&t)
{
    static Node *lastNode, *deletedNode = nullNode;

    if (t != nullNode)
    {
        // Bhma1:  Anazhthsh sto dendro kai
        //         thesimo twn lastNode kai deletedNode
        lastNode = t;
        if (isGreaterThan(t->cell, x))
            remove(x, t->leftChild);
        else
        {
            deletedNode = t;
            remove(x, t->rightChild);
        }
        // Bhma 2: An briskomaste sto pato toy dendroy
        //         kai yparxei to x to diagrafoyme
        if (t == lastNode)
        {
            bool notFound = false;
            for (int i = 0; i < 3; i++)
                if (x[i] != deletedNode->cell[i])
                {
                    notFound = true;
                    break;
                }
            if (deletedNode == nullNode || notFound)
            {
                cerr << "Item not found!" << endl;
                exit(1);
            }
            deletedNode->loot = t->loot;
            for (int i = 0; i < 3; i++)
                deletedNode->cell[i] = t->cell[i];
            deletedNode = nullNode;
            t = t->rightChild;
            delete lastNode;
        }
        // Bhma 3: Diaforetika den eimaste sto bathos,
        //         epanorthwtikes energeies.

        else if (t->leftChild->level < t->level - 1 ||
                 t->rightChild->level < t->level - 1)
        {
            if (t->rightChild->level > --t->level)
                t->rightChild->level = t->level;
            skew(t);
            skew(t->rightChild);
            skew(t->rightChild->rightChild);
            split(t);
            split(t->rightChild);
        }
    }
}

// Dhlwsh methodoy anazhthshs stoixeioy
template <class T>
T AATree<T>::find(const int x[]) const
{
    Node *current = root;
    for (int i = 0; i < 3; i++)
        nullNode->cell[i] = x[i];

    for (;;)
    {
        if (isGreaterThan(current->cell, x))
            current = current->leftChild;
        else if (isGreaterThan(x, current->cell))
            current = current->rightChild;
        else if (current != nullNode)
            return elementAt(current);
        else
            return elementAt(NULL);
    }
}

// Boithitikh methodos gia thn anazhthsh
template <class T>
T AATree<T>::
    elementAt(Node *t) const
{
    return t == NULL ? 0 : t->loot;
}

template <class T>
void AATree<T>::
    rotateWithLeftChild(Node *&k2) const
{
    Node *k1 = k2->leftChild;
    k2->leftChild = k1->rightChild;
    k1->rightChild = k2;
    k2 = k1;
}

template <class T>
void AATree<T>::
    rotateWithRightChild(Node *&k1) const
{
    Node *k2 = k1->rightChild;
    k1->rightChild = k2->leftChild;
    k2->leftChild = k1;
    k1 = k2;
}

// Dhlwsh ths leitoyrgias skew
template <class T>
void AATree<T>::
    skew(Node *&t) const
{
    if (t->leftChild->level == t->level)
        rotateWithLeftChild(t);
}

// Dhlwsh ths leitoyrgias split
template <class T>
void AATree<T>::
    split(Node *&t) const
{
    if (t->rightChild->rightChild->level == t->level)
    {
        rotateWithRightChild(t);
        t->level++;
    }
}

template <class T>
bool AATree<T>::isGreaterThan(const int x[], const int y[]) const
{
    if (x[0] > y[0])
        return true;
    else if (x[0] < y[0])
        return false;
    else if (x[1] > y[1])
        return true;
    else if (x[1] < y[1])
        return false;
    else if (x[2] > y[2])
        return true;
    else
        return false;
}
