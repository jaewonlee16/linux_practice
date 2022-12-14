#include <iostream>
#include <algorithm>
#include <optional>
#include <unistd.h>

#define RED 1
#define BLACK 0

template <typename T, typename U>
class RBNode{
    
    public :
        T key;
        U value;
	RBNode<T,U> * parent;
        RBNode<T,U> * left;
        RBNode<T,U> * right;
        int color; // 1 -> red, 0 -> black

        RBNode<T,U>(const T& k, const U& v)
        {
            key = k;
            value = v;
            left = nullptr;
            right = nullptr;
            parent = nullptr;
        }        
        
};

template <typename T, typename U>
class RBTree {
    public : 
        RBNode<T,U> * root = nullptr;
        ~RBTree() {
            removeall(root);
        }

        void insert(const T& key, const U& value);
        U search(const T& key);
        bool remove(const T& key);

        //for checking
        void preorder(RBNode<T,U>*& node){
            if(!node) return;

            std::cout<<node->key<<": "<<node->value<<std::endl;
            preorder(node->left);
            preorder(node->right);
        }
        void inorder(RBNode<T,U>*& node){
            if(!node) return;

            inorder(node->left);
            std::cout<<node->key<<": "<<node->value<<std::endl;
            inorder(node->right);
        }

    private :
        RBNode<T,U>* rotate_left(RBNode<T,U>*& node);
        RBNode<T,U>* rotate_right(RBNode<T,U>*& node);

        RBNode<T,U>* insert(RBNode<T,U>*& node, const T& key, const U& value);
        U search(RBNode<T,U>*& node, const T& key);
        RBNode<T,U>* remove(RBNode<T,U>*& node, const T& key);
        void removeall(RBNode<T,U>*& node);

        RBNode<T, U>* normalBSTInsertPosition(RBNode<T, U>*& node ,const T& key);
        RBNode<T, U>* insertBalancing(RBNode<T, U>*& node);
        RBNode<T,U>* searchNode(RBNode<T,U>* node, const T& key);
        RBNode<T,U>* minimum_node(RBNode<T,U>* node);
        RBNode<T, U>* deleteBalance(RBNode<T, U>*& node);
};

template<typename T, typename U>
void RBTree<T,U>::insert(const T& key, const U& value) {
    root = insert(root, key, value);
}

template<typename T, typename U>
U RBTree<T,U>::search(const T& key) {
    return search(root, key);
}

template<typename T, typename U>
bool RBTree<T,U>::remove(const T& key) {
    if(!search(root,key).length()) return false;
    root = remove(root, key);
    return true;
}

template<typename T, typename U>
bool isParentRed(RBNode<T,U>* node){
    return node->parent->color == RED;
}

template<typename T, typename U>
bool isLeftChild(RBNode<T,U>* node){
    RBNode<T, U>* tmp = node;
    if (tmp->parent == nullptr)
        return false;
    return tmp->parent->left == tmp;       
}

template<typename T, typename U>
bool isRightChild(RBNode<T,U>* node){
    RBNode<T, U>* tmp = node;
    if (tmp->parent == nullptr)
        return false;
    return tmp->parent->right == tmp;       
}

template<typename T, typename U>
bool isRoot(const RBNode<T, U>* node){
    if (node == nullptr)
        return false;
    return node->parent == nullptr;
}

template<typename T, typename U>
RBNode<T,U>* RBTree<T,U>::rotate_left(RBNode<T,U>*& node){
    //TODO    
    RBNode<T, U>* tmp = node->right;

    //first
    node->right = tmp->left;
    if (node->right != nullptr)
        node->right->parent = node;        

    // second
    tmp->parent = node->parent;
    if (!tmp->parent){
        // when input node is root node
    } else if ( isLeftChild(node))
        tmp->parent->left = tmp;
    else if ( isRightChild(node))
        tmp->parent->right = tmp;        

    // third
    tmp->left = node;
    node->parent = tmp;

    return tmp;
}

template<typename T, typename U>
RBNode<T,U>* RBTree<T,U>::rotate_right(RBNode<T,U>*& node){
    //TODO
    RBNode<T, U>* tmp = node->left;

    //first
    node->left = tmp->right;
    if (node->left != nullptr)
        node->left->parent = node;        

    // second
    tmp->parent = node->parent;
    if (!tmp->parent){
        // when input node is root node
    } else if ( isLeftChild(node))
        tmp->parent->left = tmp;
    else if ( isRightChild(node))
        tmp->parent->right = tmp;        

    // third
    tmp->right = node;
    node->parent = tmp;

    return tmp;
}

template<typename T, typename U>
RBNode<T, U>* RBTree<T, U>::normalBSTInsertPosition(RBNode<T, U>*& node, const T& key){
    RBNode<T, U>* y = node;
    while (1){
        if (node == nullptr)
            break;
        y = node;
        if (key == node->key)
            return y;
        else if (key > node->key)
            node = node->right;
        else if (key < node->key)
            node = node->left;
    }
    return y;
}

template<typename T, typename U>
RBNode<T, U>* RBTree<T, U>::insertBalancing(RBNode<T, U>*& node){
    if (isRoot(node->parent))
        return node;
    else{
        while (!isRoot(node) && isParentRed(node)){
                        
            RBNode<T, U>* grandparent = node->parent->parent;
            RBNode<T, U>* uncle = isLeftChild(node->parent) ? grandparent->right : grandparent->left;
            int uncleColor = uncle ? uncle->color : BLACK;


            // case 3-1: if P id Red and U is Red
            if (isParentRed(node) && uncleColor == RED){
                node->parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = isRoot(grandparent) ? BLACK : RED;
                node = grandparent;
            }        
            else{
                // case 3-2-2
                bool isParentLeftChild = isLeftChild(node->parent);
                if (isParentLeftChild ? isRightChild(node) : isLeftChild(node)){
                    node = node->parent;
                    node = isParentLeftChild ? rotate_left(node) : rotate_right(node);
                }
                // case 3-2-1
                node->color = BLACK;
                grandparent->color = RED;
                node = isParentLeftChild ? rotate_right(grandparent) : rotate_left(grandparent);
                break;

            }
        }
        
    }
    if (node){
        while(node->parent != nullptr){
            node = node->parent;
        }
        node->color = BLACK;
    }
    return node;
}

template<typename T, typename U>
RBNode<T,U>* RBTree<T,U>::insert(RBNode<T,U>*& node, const T& key, const U& value) {
    //TODO
    RBNode<T, U>* insertNode = node;
    RBNode<T, U>* y = normalBSTInsertPosition(insertNode, key);

    // case 0
    if (insertNode != nullptr){   // when key is already detected
        insertNode->value = value;
        return node;
    }

    // new node
    RBNode<T, U>* newNode = new RBNode<T, U>(key, value);
    newNode->color = RED;
    newNode->parent = y;

    // case 1
    if (isRoot(newNode)) { // when root 
        node = newNode;
        newNode->color = BLACK;
        return node;
    }

    if (newNode->key < newNode->parent->key)
        newNode->parent->left = newNode;
    else if (newNode->key > newNode->parent->key)
        newNode->parent->right = newNode;

    //case 2
    if (!isParentRed(newNode))
        return node;
    else
        node = insertBalancing(newNode); 

    return node;
}

template<typename T, typename U>
U RBTree<T,U>::search(RBNode<T,U>*& node, const T& key) {
    //TODO
    //return NULL if there are no such key, return value if there is
    RBNode<T, U>* presentNode = node;
    U result;
    while (presentNode){
        if (presentNode->key == key)
            return presentNode->value;
        else if (key < presentNode->key)
            presentNode = presentNode->left;
        else if (key > presentNode->key)
            presentNode = presentNode->right;
    }
    return result;
}

template<typename T, typename U>
RBNode<T,U>* RBTree<T,U>::searchNode(RBNode<T,U>* node, const T& key){
    RBNode<T, U>* presentNode = node;
    while (presentNode){
        if (presentNode->key == key)
            return presentNode;
        else if (key < presentNode->key)
            presentNode = presentNode->left;
        else if (key > presentNode->key)
            presentNode = presentNode->right;            
    }
    return presentNode;
}


template<typename T, typename U>
void transplant(RBNode<T, U>*& node1, RBNode<T, U>*& node2){
    if (node1 == nullptr)
        return;
    else if (isLeftChild(node1))
        node1->parent->left = node2;
    else if (isRightChild(node1))
        node1->parent->right = node2;
    if (node2 != nullptr)
        node2->parent = node1->parent;
}

template<typename T, typename U>
RBNode<T,U>* RBTree<T,U>::minimum_node(RBNode<T,U>* node){
    RBNode<T, U>* result = node;
    while (result->left != nullptr){
        result = result->left;
    }
    return result;
}

template<typename T, typename U>
bool isBlack(RBNode<T, U>* node){
    if (node == nullptr)
        return true;
    return node->color == BLACK;
}

template<typename T, typename U>
RBNode<T, U>* RBTree<T, U>::deleteBalance(RBNode<T, U>*& node){
    RBNode<T, U> * w, * pNode;
    while(1){
        if (node->color >= 90){
            pNode = node;
            node = node->left ? node->right : node->left;
        }
        else if (node->parent == nullptr || node->color == RED)
            break;
        
        if (node == pNode->left){
            w = pNode->right;

            // case 1
            if (w != nullptr && w->color == RED){
                w->color = BLACK;
                pNode->color = RED;
                w = rotate_left(pNode);        
                pNode = w->left;
                w = pNode->right;
            }
            // case 2
            if (isBlack(w->left) && isBlack(w->right)){
                w->color = RED;
                node = pNode;
                node->color = node->color % 10;
            }
            else {
                //case 3
                if (isBlack(w->right)){
                    w->left->color = BLACK;
                    w->color = RED;
                    w = rotate_right(w);
                    w = pNode->right;
                } 
                // case 4
                w->color = pNode->color;
                pNode->color = BLACK;
                if (w->right != nullptr)
                    w->right->color = BLACK;
                w = rotate_left(pNode);
                node = pNode;
                while (node->parent != nullptr){
                    node->color = node->color % 10;
                    node = node->parent;
                }
            }
        }
        else{
            w = pNode->left;

            // case 1
            if (w != nullptr && w->color == RED){
                w->color = BLACK;
                pNode->color = RED;
                w = rotate_right(pNode);        
                pNode = w->right;
                w = pNode->left;
            }
            // case 2
            if (isBlack(w->left) && isBlack(w->right)){
                w->color = RED;
                node = pNode;
                node->color = node->color % 10;
            }
            else {
                //case 3
                if (isBlack(w->left)){
                    w->right->color = BLACK;
                    w->color = RED;
                    w = rotate_left(w);
                    w = pNode->left;
                } 
                // case 4
                w->color = pNode->color;
                pNode->color = BLACK;
                if (w->left != nullptr)
                    w->left->color = BLACK;
                w = rotate_right(pNode);
                node = pNode;
                while (node->parent != nullptr){
                    node->color = node->color % 10;
                    node = node->parent;
                }
            }
        }
    }
    if (node == nullptr){
        node = pNode;
        node->color = node->color % 10;
    }
    else
        node->color = BLACK;
    
    // make node root
    while (node->parent != nullptr){
        node = node->parent;
    }
    node->color = BLACK;

    return node;
}

template<typename T, typename U>
RBNode<T,U>* RBTree<T,U>::remove(RBNode<T,U>*& node, const T& key) {

    //TODO
    RBNode<T, U>* nodeToBeDeleted = searchNode(node, key);

    if (!nodeToBeDeleted){
        return nodeToBeDeleted;
    }
    RBNode<T, U> * x, * y;
    int originalColor = nodeToBeDeleted->color;
    if (nodeToBeDeleted->left == nullptr && nodeToBeDeleted->right == nullptr){
        x = nodeToBeDeleted->parent;
        // as x is nullptr need to find way to impement in deleteFix(). x->parent leads to error so no way to find sibling(w)
        x->color = 90 + x->color; // exception that need to be added in deleteFix() ---------------------------------------------------------
        transplant(nodeToBeDeleted, nodeToBeDeleted->right);
    }
    else if (nodeToBeDeleted->left == nullptr){
        x = nodeToBeDeleted->right;
        transplant(nodeToBeDeleted, x);
    }
    else if (nodeToBeDeleted->right == nullptr){
        x = nodeToBeDeleted->left;
        transplant(nodeToBeDeleted, x);
    }
    else{
        y = minimum_node(nodeToBeDeleted->right);
        originalColor = y->color;
        x = y->right;
       
        if (y->parent == nodeToBeDeleted){
            if (x == nullptr){
                x = y;
                x->color = 90 + x->color;
            }
        }
        else{
            if (x == nullptr){
                x = y->parent;
                x->color = 90 + x->color;
            }
            transplant(y, y->right);
            y->right = nodeToBeDeleted->right;
            y->right->parent = y;
        }
        transplant(nodeToBeDeleted, y);
        y->left = nodeToBeDeleted->left;
        y->left->parent = y;
        y->color = nodeToBeDeleted->color;

    }
    delete nodeToBeDeleted;
    if (originalColor == BLACK)
        node = deleteBalance(x);

    while (node->parent != nullptr){
        node = node->parent;
    }
    return node;
}

template<typename T, typename U>
void RBTree<T,U>::removeall(RBNode<T,U>*& node) {
    //TODO
    //for destructor
    if (node){
        removeall(node->left);
        removeall(node->right);
        delete node;
    }
}
