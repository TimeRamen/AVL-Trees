#include <iostream>
#define COUNT 10
#define RANDM 100
#include <time.h>   //using time(NULL)//
#include <stdlib.h> //using srand and rand//

using namespace std;

template <typename key, typename info>
class Dictionary{
    private:
        struct AVLnode
        {
            key ID;
            info Data;
            int bfactor; //balance factor
            AVLnode *llink;//link to the left
            AVLnode *rlink;//link to the right
        };
        AVLnode *root;





int height(AVLnode *p) const{
if(p==NULL)
    return 0;
else
    return 1+max(height(p->llink),height(p->rlink));//max determines the larger of the two integers.
}




/*
void CopyAll(AVLnode *p)
{
    insert(p->ID,p->Data);
    CopyAll(p->rlink);
    CopyAll(p->llink);
}
*/




void copyTree(AVLnode* &copied, AVLnode* other){
if(other == NULL)
    copied = NULL;
else{
    copied = new AVLnode;
    copied->Data = other->Data;
    copied->ID = other->Data;
    copied->bfactor = other->bfactor;
    copyTree(copied->llink,other->llink);
    copyTree(copied->rlink,other->rlink);
}
}//end copy tree








void insertIntoAVL(AVLnode* &root,AVLnode *newNode, bool& isTaller)
{


    if(root==NULL)
    {
        root = newNode;
        isTaller = true;
    }


    else if(root->ID == newNode->ID){
        root->Data = newNode->Data;
        //cerr<<"No duplicates allowed."<<endl;
}

    else if(root->ID > newNode->ID)//newnode goes to left subtree
    {

        insertIntoAVL(root->llink, newNode, isTaller);

        if(isTaller){//after insertion, the subtree grew in height

            switch(root->bfactor)
            {
            case -1:
                balanceLeft(root);
                isTaller = false;
                break;

            case 0:
                root->bfactor = -1;
                isTaller = true;
                break;

            case 1:
                root->bfactor = 0;
                isTaller = false;
            }//end switch

            }//end if
    }//end if

    else
    {

        insertIntoAVL(root->rlink,newNode,isTaller);

        if(isTaller){//after insertion, the subtree grew in height

            switch(root->bfactor)
            {
            case -1:
                root->bfactor = 0;
                isTaller = false;
                break;

            case 0:
                root->bfactor = 1;
                isTaller = true;
                break;

            case 1:
                balanceRight(root);
                isTaller = false;
            }//end switch

            }//end if

    }//end else

}//insertintoAVL












    void removeFromAVL(AVLnode* &curRoot, key & k, AVLnode* & toSwapWith, AVLnode* & newLink, bool & isSmaller, bool & wasDeleted){
        //cout << curRoot->bfactor<<endl;
        if (curRoot == NULL){
            cerr << "No node with such id found"<<endl;
        }


        else if (k==curRoot->ID || (toSwapWith!=NULL && curRoot->rlink==NULL)){
            //cout << "equal"<<endl;
            if (toSwapWith!=NULL){
                //cout << "swap"<<endl;
                toSwapWith->ID = curRoot->ID;
                toSwapWith->Data = curRoot->Data;
            }
            if (curRoot->llink==NULL && curRoot->rlink==NULL){
                //cout << "case 1"<<endl;
                isSmaller=true;
                wasDeleted = true;
                delete curRoot;
            } else if (curRoot->llink==NULL){
                //cout << "case 2"<<endl;
                newLink = curRoot->rlink;
                isSmaller = true;
                wasDeleted = true;
                delete curRoot;
            } else if (curRoot->rlink==NULL){
                //cout << "case 3"<<endl;
                newLink = curRoot->llink;
                isSmaller = true;
                wasDeleted = true;
                delete curRoot;
            } else {
                //cout << "case 4"<<endl;
                toSwapWith = curRoot;
                removeFromAVL(curRoot->llink, k, toSwapWith, newLink, isSmaller, wasDeleted);
                if (isSmaller){
                    if (wasDeleted){
                        curRoot->llink = newLink;
                        newLink = NULL;
                        wasDeleted = false;
                    }
                    switch (curRoot->bfactor) {
                    case -1:
                        isSmaller = true;
                        curRoot->bfactor = 0;
                        break;
                    case 0:
                        isSmaller = false;
                        curRoot->bfactor = 1;
                        break;
                    case 1:
                        balanceRight(curRoot);
                        isSmaller = true;
                    }

                }
            }
        } else if (k<curRoot->ID){
            //cout << "left"<<endl;
            removeFromAVL(curRoot->llink, k, toSwapWith, newLink, isSmaller, wasDeleted);
            if (isSmaller){
                if (wasDeleted){
                    curRoot->llink = newLink;
                    newLink = NULL;
                    wasDeleted = false;
                }
                switch (curRoot->bfactor) {
                case -1:
                    //cout << "wow -1"<<endl;
                    isSmaller = true;
                    curRoot->bfactor = 0;
                    break;
                case 0:
                    //cout << "wow 0"<<endl;
                    isSmaller = false;
                    curRoot->bfactor = 1;
                    break;
                case 1:
                    //cout << "wow 1"<<endl;
                    balanceRight(curRoot);
                    isSmaller = true;
                }


            }
        } else if (k>curRoot->ID){
            //cout << "right"<<endl;
            removeFromAVL(curRoot->rlink, k, toSwapWith, newLink, isSmaller, wasDeleted);
            if (isSmaller){
                if (wasDeleted){
                    curRoot->rlink = newLink;
                    newLink = NULL;
                    wasDeleted = false;
                }
                switch (curRoot->bfactor) {
                case -1:
                    balanceLeft(curRoot);
                    isSmaller = true;
                    break;
                case 0:
                    isSmaller = false;
                    curRoot->bfactor = -1;
                    break;
                case 1:
                    isSmaller = true;
                    curRoot->bfactor = 0;
                }


            }
        }
    };



















AVLnode * minValueNode(AVLnode* p)
{
    AVLnode* current = p;

    /* loop down to find the leftmost leaf */
    while (current->llink != NULL)
        current = current->llink;

    return current;
}


AVLnode * maxValueNode(AVLnode* p)
{
    AVLnode* current = p;

    /* loop down to find the leftmost leaf */
    while (current->rlink != NULL)
        current = current->rlink;

    return current;
}



void destroy(AVLnode *p){
    if(p!=NULL)
    {
        destroy(p->llink);
        destroy(p->rlink);
        delete p;
        p = NULL;
    }
};




int max(int x, int y) const{
    if(x>=y)
        return x;
    else
        return y;
};//returns larger of x and y






int nodeCount(AVLnode *p) const{
//int i = 1;


    if(p!=NULL){
        return 1+(nodeCount(p->llink)+nodeCount(p->rlink));
    }else{
        return 0;

    }



};//returns number of nodes in the tree that p points to



int leavesCount(AVLnode *p) const
{
    if(p==NULL)
        return 0;
    if(p->llink==NULL&&p->rlink==NULL){
        return 1;
    }
    return (leavesCount(p->llink)+leavesCount(p->rlink));
};//returns number of leaves in the tree that p points to




void inorder(AVLnode *p) const
{
    if(p!=NULL)
    {
        inorder(p->llink);
        cout<<p->ID<<" -> ";
        inorder(p->rlink);
    }
}



void preorder(AVLnode *p) const
{
    if(p!=NULL)
    {
        cout << p->ID <<" -> ";
        preorder(p->llink);
        preorder(p->rlink);
    }
}



void postorder(AVLnode *p) const
{
    if(p!=NULL)
    {
        postorder(p->llink);
        postorder(p->rlink);
        cout << p->ID <<" -> ";
    }
}



void rotateRight(AVLnode * &root){
AVLnode *p;//pointer to the root of the left subtree of root
if (root == NULL)
    cerr<< "Error in the tree"<<endl;
else if(root->llink==NULL)
    cerr<< "Error in the tree:"<<" No left subtree to rotate."<<endl;
else
{
    p = root->llink;
    root->llink = p->rlink;//left subtree of p becomes right subtree of root
    p->rlink = root;
    root = p;//make p the new root node
}
}//rotateLeft



void rotateLeft(AVLnode* &root){
AVLnode *p;//pointer to the root of the right subtree of root
if (root == NULL)
    cerr<< "Error in the tree"<<endl;
else if(root->rlink==NULL)
    cerr<< "Error in the tree:"<<" No right subtree to rotate."<<endl;
else
{
    p = root->rlink;
    root->rlink = p->llink;//left subtree of p becomes right subtree of root
    p->llink = root;
    root = p;//make p the new root node
}
}//rotateLeft



void balanceLeft(AVLnode* &root){
    AVLnode *p;
    AVLnode *w;

    p = root->llink;//p points to left subtree of root

    switch(p->bfactor)
    {
    case -1:
        root->bfactor = 0;
        p->bfactor = 0;
        rotateRight(root);
        break;
    case 0:
        cerr<<"Error: Cannot balance from the left."<<endl;
        break;
    case 1:
        w = p->rlink;
        switch(w->bfactor){
        case -1:
            root->bfactor = 1;
            p->bfactor = 0;
            break;
        case 0:
            root->bfactor = 0;
            p->bfactor = 0;
            break;
        case 1:
            root->bfactor = 0;
            p->bfactor = -1;
        }//end switch
        w->bfactor = 0;
        rotateLeft(p);
        root->llink = p;
        rotateRight(root);
    }//end switch
}//end balanceleft



void balanceRight(AVLnode* &root){
    AVLnode *p;
    AVLnode *w;
    p = root->rlink; //p points to the left subtree of root

    switch(p->bfactor)
    {
    case -1:
        w = p->llink;
        switch(w->bfactor)
        {
        case -1:
            root->bfactor = 0;
            p->bfactor = 1;
            break;
        case 0:
            root->bfactor = 0;
            p->bfactor = 0;
            break;
        case 1:
            root->bfactor = -1;
            p->bfactor = 0;
        }//end switch
        w->bfactor = 0;
        rotateRight(p);
        root->rlink = p;
        rotateLeft(root);
        break;

    case 0:
        cerr<<"Error: Cannot balance from the left." <<endl;
        break;
    case 1:
        root->bfactor = 0;
        p->bfactor = 0;
        rotateLeft(root);
    }//end switch

}//end balanceright




void printRow(AVLnode* p, int level)
{
    if (p == NULL){
        cout<<" ";
        return;
        }
    if (level == 1)
        cout<<p->ID << " ";
    else if (level > 1)
    {
        printRow(p->llink, level-1);
        printRow(p->rlink, level-1);
    }
}


void printRowInfo(AVLnode* p, int level)
{
    if (p == NULL){
        cout<<" ";
        return;
        }
    if (level == 1){
        cout<<p->ID <<","<<p->Data<<" ";}
    else if (level > 1)
    {
        printRowInfo(p->llink, level-1);
        printRowInfo(p->rlink, level-1);
    }
}


void printColumn(AVLnode *p, int space)
{
    // Base case
    if (p == NULL)
        return;

    // Increase distance between levels
    space += COUNT;

    // Process right child first
    printColumn(p->rlink, space);

    // Print current node after space
    // count
    cout<<endl;
    for (int i = COUNT; i < space; i++)
        cout<<" ";
    cout<< p->ID;

    // Process left child
    printColumn(p->llink, space);
}



void printColumnInfo(AVLnode *p, int space)
{
    // Base case
    if (p == NULL)
        return;

    // Increase distance between levels
    space += COUNT;

    // Process right child first
    printColumnInfo(p->rlink, space);

    // Print current node after space
    // count
    cout<<endl;
    for (int i = COUNT; i < space; i++)
        cout<<" ";
    cout<< p->ID<<","<< p->Data;

    // Process left child
    printColumnInfo(p->llink, space);
}

void printColumnDetail(AVLnode *p, int space)
{
    // Base case
    if (p == NULL)
        return;

    // Increase distance between levels
    space += COUNT;

    // Process right child first
    printColumnDetail(p->rlink, space);

    // Print current node after space
    // count
    cout<<endl;
    for (int i = COUNT; i < space; i++)
        cout<<" ";
    cout<< p->ID<<","<< p->Data<<","<<p->bfactor;

    // Process left child
    printColumnDetail(p->llink, space);
}


void printHoriDetail(){
printColumnDetail(root,0);
};


    public:


        const Dictionary<key,info>& operator=(const Dictionary<key,info>& D){
            if(this!= &D)
            {
                if(root != NULL)
                    destroy(root);
                if(D.root == NULL)
                    root = NULL;
                else
                    copyTree(root,D.root);
            }//end
            return *this;
        };//overloading of assignment operator


        bool isEmpty() const{
            return (root == NULL);
        };//returns 1 if tree is empty, 0 if not


        void inorderTraversal() const{
            inorder(root);
        };


        void preorderTraversal() const{
            preorder(root);
        };


        void postorderTraversal() const{
            postorder(root);
        };


        int treeHeight() const{
            return height(root);
        };//returns height of tree


        int treeNodeCount() const{//} const{
            return nodeCount(root);
        };//returns number of nodes in tree


        int treeLeavesCount()const{//const{
            return leavesCount(root);
        };//returns number of leaves


        void destroyTree(){
            destroy(root);
        };//deallocates memory space occupied by AVL tree


        Dictionary(const Dictionary<key,info> &D){
            if(D.root == NULL)
                root = NULL;
            else
                copyTree(root,D.root);
        };//copy constructor


        Dictionary(){
            root = NULL;
        };//constructor


        ~Dictionary(){
            destroy(root);
        };//destructor



        bool search(const key& item) const{
    AVLnode *current;
    bool found = false;

    if(root == NULL)
        cerr<<"Cannot search the empty tree."<<endl;
    else{
        current = root;
        while(current!=NULL && !found){
            if(current->ID == item)
                found = true;
            else if(current->key > item)
                current = current->llink;
            else
                current = current->rlink;
        }//end while
    }//end else
    return found;
}//end search




void insert(const key &newItem, const info &newData)
{
    bool isTaller = false;
    AVLnode *newNode;

    newNode = new AVLnode;
    newNode->ID = newItem;
    newNode->Data = newData;
    newNode->bfactor = 0;
    newNode->llink = NULL;
    newNode->rlink = NULL;

    insertIntoAVL(root,newNode,isTaller);
}



    void remove(key k){
        bool isSmaller = false;
        bool wasDeleted = false;
        AVLnode* toSwapWith = NULL;
        AVLnode* newLink = NULL;
        removeFromAVL(root, k, toSwapWith, newLink, isSmaller, wasDeleted);
    };



/*
    Dictionary<key,info> operator+(const Dictionary<key,info>& x)const{
    Dictionary<key,info> result;
    AVLnode* a = this->root;
    AVLnode* b = x.root;
    //AVLnode* c = result.root;


    result.CopyAll(a);
    result.CopyAll(b);
    return result;
    };

void merge(const Dictionary<key,info>& x,const Dictionary<key,info>& y){
AVLnode* p = x.root;
AVLnode* q = y.root;

CopyAll(p);
CopyAll(q);


}
*/


void printVert()
{
    int h = height(root);
    int i;
    int j = h;
    int k;
    for (i=1; i<=h; i++,j--)
    {
        for(k=1;k<j;k++){
            cout<<" ";
        }
        printRow(root, i);
        cout<<endl<<endl;
    }

};


void printHori(){
printColumn(root,0);
};




void printVertInfo()
{
    int h = height(root);
    int i;
    int j = h;
    int k;
    for (i=1; i<=h; i++,j--)
    {
        for(k=1;k<j;k++){
            cout<<" ";
        }
        printRowInfo(root, i);
        cout<<endl<<endl;
    }

};




void printHoriInfo(){
printColumnInfo(root,0);
};




void printDetail(){
printHoriDetail();
cout<<endl<<endl;
cout<<endl<<"Height of AVLTree: "<<treeHeight()<<endl;
cout<<endl<<"Number of leaves:  "<<treeLeavesCount()<<endl;
cout<<endl<<"Number of nodes:   "<<treeNodeCount()<<endl;
}


















};

/*
    template <typename key,typename info>
    ostream& operator<< (ostream& os, const Dictionary<key,info>& x){
    x.printDetail(os);
    return os;
    }*/


int random(int r = RANDM){
return (rand()%r);
}


int main()
{

    srand(0);//time(NULL));
    Dictionary<int,int> A;
    int i = 0;
    //int m = 100;
    //int n = 10;
    int j,k;
    for(i;i<COUNT;i++){
        j = random();
        k = random();
        A.insert(j,k);

        cout<<i+1<<"\tnode inserted "<<j<<" and "<<k<<endl;
    }
    cout<<endl<<endl;
    A.printDetail();
    A.remove(j);
    A.printDetail();

{
/*
    cout<<endl<<endl;

    A.printVertInfo();

    A.insert(1,1);
    A.insert(2,2);
    A.insert(3,3);
    A.insert(4,4);

A.printDetail();

    cout<<endl<<endl;
    cout<<endl<<endl;

    A.printVert();

    cout<<endl<<endl;

    A.insert(5,5);
    A.insert(6,6);
    A.insert(7,7);

    A.printHoriInfo();

    cout<<endl<<endl;
    cout<<endl<<endl;

    A.printVertInfo();

        cout<<endl<<endl;
    cout<<endl<<endl;


    A.remove(6);

    A.printHori();
    cout<<endl<<endl;
    A.printVert();
cout<<endl<<endl;
    A.remove(1);

    A.printHori();
    cout<<endl<<endl;
    A.printVert();


    A.insert(1,1);
    A.insert(6,6);

    cout<<endl<<endl;

    A.printHori();

    /*
    A.postorderTraversal();
    cout<<endl;
    A.preorderTraversal();
    cout<<endl;
    A.inorderTraversal();


        cout<<endl<<endl;
    cout<<endl<<endl;
            cout<<endl<<endl;
    cout<<endl<<endl;

    //A.printDetail();

Dictionary<int,int> B;
B = A;
B.printDetail();

B.insert(12,3);
B.insert(-12,3);
B.insert(0,9);
B.insert(100,1);
B.insert(999,999);
B.insert(1000,1);
B.insert(-1,3);
B.insert(-199,8);
B.insert(99,9);

B.printDetail();
*/
//int i = B.treeLeavesCount();
//cout<< i ;

//Dictionary<int,int> C = A+B;
//C.printDetail();
}



    return 0;
}
