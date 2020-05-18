

/*Cerințe comune tuturor temelor 3:
    1. utilizarea sabloanelor
    2. utilizarea STL
    3. utilizarea variabilelor, funcțiilor statice, constantelor și a unei functii const(static done, const done)
    4. utilizarea conceptelor de RTTI raportat la template-uri
    5. tratarea excepțiilor(done)
    6. citirea informațiilor complete a n obiecte, memorarea și afișarea acestora (done)

  Tema2:
    1. Constructori de initializare, parametrizati si de copiere, destructor, iar operatorii <<, >>, =
       supraincarcati. (facut)
    2. Clasele derivate sa contina constructori prin care sa se evidentieze transmiterea parametrilor
       catre constructorul din clasa de baza. (facut T3)
    3. Ilustrarea conceptelor de mostenire si functii virtuale. (facut)
    4. Tratarea exceptiilor.(done la tema 3)
    */
#include <iostream>
using namespace std;


class node
{
protected:
    int value;
    node* left, * right;

public:

    node()
    {
        cout << "INIT CONSTRUCTOR NODE" << endl;
        left = NULL;
        right = NULL;
        value = 0;
    }
    node(int val)
    {
        cout << "CONSTRUCTOR NODE WITH PARAMETERES" << endl;
        value = val;
        left = NULL;
        right = NULL;
    }
    virtual ~node()
    {
        cout << "DESTRUCTOR NODE" << endl;
    }


    friend class Arbore;
    friend class ABC;
    friend class AVL;
};


class avlNode : public node
{
public:
    avlNode* left;
    avlNode* right;
    int height;

    avlNode() :node{}, height{ 1 }
    {
        cout << "AVL NODE CONSTRUCTOR " << endl;
        left = NULL;
        right = NULL;
    }

    avlNode(int val) : node{ val }, height{ 1 }
    {
        cout << "CONSTRUCTOR AVL NODE WITH PARAMETERES" << endl;
        value = val;
        left = NULL;
        right = NULL;
    }

    ~avlNode()
    {
        cout << "DESTRUCTOR AVL NODE" << endl;
    }
    
    friend class Arbore;
    friend class ABC;
    friend class AVL;
};

class Arbore
{
protected:
    int nr_noduri;
public:

    Arbore()
    {
        cout << "ARBORE SIMPLE CONSTRUCTOR" << endl;
        nr_noduri = 0;
    }
    Arbore(int nr)
    {
        cout << "ARBORE PARAM CONSTRUCTOR" << endl;
        nr_noduri = nr;
    }
    virtual ~Arbore() = 0;
};
inline Arbore::~Arbore() { cout << "ARBORE DESTRUCTOR" << endl; }

class ABC : public Arbore
{
protected:
    node* rad;
public:

    friend istream& operator>>(istream& in, ABC& a)
    {
        int val;
        in >> val;
        a.insert(a.rad, val);
        return in;
    }

    friend ostream& operator<<(ostream& os,  ABC& a)
    {
        a.preorder(a.rad);
        return os;
    }

    ABC()
    {
        cout << "ABC simple constructor" << endl;
        rad = new node;
    }
    ABC(int val) :Arbore{ val }
    {
        cout << "ABC param constructor" << endl;
        rad = new node;
    }
    ~ABC()
    {
        cout << "ABC DESTRUCTOR" << endl;
    }

    node* createNode(int val)
    {
        node* p = new node();
        p->value = val;
        p->left = NULL;
        p->right = NULL;
        return p;
    }

    node* insert(node* p, int val)
    {
        if (p == NULL)
            return createNode(val);

        if (val < p->value)
            p->left = insert(p->left, val);
        else if (val > p->value)
            p->right = insert(p->right, val);
        else
            return p;

        return p;
    }

    node* minim(node* p)
    {
        node* temp = p;
        while (temp && temp->left != NULL)
            temp = temp->left;

        return temp;
    }

    void preorder(node* root)
    {
        if (root != NULL)
        {
            cout << root->value << " ";
            preorder(root->left);
            preorder(root->right);
        }
    }

    node* get_root() const
    {
        return rad;
    }

    void afis(node* p) const
    {
        cout << rad->value << endl;
    }

    void setter(int val)
    {
        rad->value = val;
    }
};

class AVL : public Arbore
{
public:
    AVL()
    {
        cout << "AVL SIMPLE CONSTRUCTOR" << endl;
    }
    ~AVL()
    {
        cout << "AVL DESTRUCTOR" << endl;
    }
    template<typename T>
    static T max(T a, T b)
    {
        if (a >= b) return a;
        return b;
    }

    int getHeight(avlNode* node)
    {
        if (node == nullptr)
            return 0;

        return node->height;
    }
    avlNode* updateHeight(avlNode* node)
    {
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        return node;
    }
    avlNode* insert(avlNode* root, int value)
    {
        if (root == nullptr)
            return new avlNode(value);
        try
        {
            if (root->value == value) throw value;
        }
        catch (int i)
        {
            cout << "value: " << i << " No duplicate allowed." << endl;
            return root;
        }
        
        if (root->value < value)
            root->right = insert(root->right, value);
        else
            root->left = insert(root->left, value);

        root = updateHeight(root);

        int balanceFactor = getHeight(root->left) - getHeight(root->right);

        if (balanceFactor > 1 && root->left->value > value)
            return rightRotate(root);

        else if (balanceFactor < -1 && root->right->value < value)
            return leftRotate(root);

        // LR rotation
        else if (balanceFactor > 1 && root->left->value < value)
        {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }

        // RL rotation
        else if (balanceFactor < -1 && root->right->value > value)
        {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }

    avlNode* leftRotate(avlNode* root)
    {
        avlNode* x = root->right;
        avlNode* t = x->left;

        root->right = t;
        x->left = root;

        x = updateHeight(x);
        root = updateHeight(root);

        return x;
    }
    avlNode* rightRotate(avlNode* root)
    {
        avlNode* x = root->left;
        avlNode* t = x->right;

        root->left = t;
        x->right = root;

        root = updateHeight(root);
        x = updateHeight(x);

        return x;
    }
    void printAll(avlNode* root)
    {
        if (root->left != nullptr)
            printAll(root->left);

        cout << root->value << " ";

        if (root->right != nullptr)
            printAll(root->right);
    }
    void preorder(avlNode* root)
    {
        if (root != NULL)
        {
            cout << root->value << " ";
            preorder(root->left);
            preorder(root->right);
        }
    }
};
int main()
{
    cout << "Number of avl objects: ";
    int n, i;
    cin >> n;
    try
    {
        if (n < 1) throw n;
    }
    catch(int exceptie)
    {
        cout << "Value " << exceptie << " is not valid";
        exit(EXIT_FAILURE);
    }
    int x, j, elem;
    for (i = 1; i <= n; i++)
    {
        cout << "Number of nodes in avl tree: ";
        cin >> x;
        try
        {
            if (x < 1) throw x;
        }
        catch (int exceptie)
        {
            cout << "Value " << exceptie << " is not valid";
            exit(EXIT_FAILURE);
        }
        AVL avl;
        avlNode* root = nullptr;

        for (j = 1; j <= x; j++)
        {
            cin >> elem;
            root = avl.insert(root, elem);
        }

        avl.preorder(root);
        cout << endl;
    }

    return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
