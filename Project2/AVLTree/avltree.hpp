#include <iostream>
#include <algorithm>
#include <optional>
#include <unistd.h>

template <typename T, typename U>
class AVLNode{
    
    public :
        T key;
        U value;
        AVLNode<T,U> * left;
        AVLNode<T,U> * right;
        int height;

        AVLNode<T,U>(const T& k, const U& v)
        {
            key = k;
            value = v;
            left = nullptr;
            right = nullptr;
            height = 1;
        }        
        
};

template <typename T, typename U>
class AVLTree {
    public : 
        AVLNode<T,U> * root = nullptr;
        ~AVLTree() {
            removeall(root);
        }

        void insert(const T& key, const U& value);
        U search(const T& key);
        bool remove(const T& key);

        //for checking
        void preorder(AVLNode<T,U>*& node){
            if(!node) return;

            std::cout<<node->key<<": "<<node->value<<std::endl;
            preorder(node->left);
            preorder(node->right);
        }
        void inorder(AVLNode<T,U>*& node){
            if(!node) return;

            inorder(node->left);
            std::cout<<node->key<<": "<<node->value<<std::endl;
            inorder(node->right);
        }

    private :
        int getHeight(AVLNode<T,U>*& node);
        int getBalance(AVLNode<T,U>*& node);
        AVLNode<T,U>* rotate_left(AVLNode<T,U>*& node);
        AVLNode<T,U>* rotate_right(AVLNode<T,U>*& node);

        AVLNode<T,U>* insert(AVLNode<T,U>*& node, const T& key, const U& value);
        U search(AVLNode<T,U>*& node, const T& key);
        AVLNode<T,U>* remove(AVLNode<T,U>*& node, const T& key);
        void removeall(AVLNode<T,U>*& node);

};

template <typename T, typename U>
int AVLTree<T,U>::getHeight(AVLNode<T,U>*& node){
    if(!node) return 0;
    return node->height;
}

template <typename T, typename U>
int AVLTree<T,U>::getBalance(AVLNode<T,U>*& node){
    if(!node) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

template<typename T, typename U>
void AVLTree<T,U>::insert(const T& key, const U& value) {
    root = insert(root, key, value);
}

template<typename T, typename U>
U AVLTree<T,U>::search(const T& key) {
    return search(root, key);
}

template<typename T, typename U>
bool AVLTree<T,U>::remove(const T& key) {
    if(!search(root,key).length()) return false;
    root = remove(root, key);
    return true;
}


int maxHeight(const AVLNode<T,U>*& node1, const AVLNode<T,U>*& node2){
    int height1, height2;
    if (node1 == nullptr)
        height1 = 0;
    else
        height1 = node1->height;

    if (node2 == nullptr)
        height2 = 0;
    else
        height2 = node2->height;


    if (height1 > height2)
        return height1;
    else
        return height2;
}


template<typename T, typename U>
AVLNode<T,U>* AVLTree<T,U>::rotate_left(AVLNode<T,U>*& node){
    //TODO    
    AVLNode* tmp = node->right;
    node->right = tmp->left;
    tmp->left = node;

    // change heights
    node->height = maxHeight(node->left, node->right) + 1;
    tmp->height = maxHeight(tmp->left, tmp->right) + 1;
    return tmp;

}

template<typename T, typename U>
AVLNode<T,U>* AVLTree<T,U>::rotate_right(AVLNode<T,U>*& node){
    //TODO
    
    AVLNode* tmp = node->left;
    node->left = tmp->right;
    tmp->right = node;

    // change heights
    node->height = maxHeight(node->left, node->right) + 1;
    tmp->height = maxHeight(tmp->left, tmp->right) + 1;
    return tmp;

}

template<typename T, typename U>
AVLNode<T,U>* AVLTree<T,U>::insert(AVLNode<T,U>*& node, const T& key, const U& value) {
    //TODO
    if (node == nullptr){
        AVLNode<T, U>* newNode = new AVLNode<T, U>;
        newNode->key = key;
        newNode->value = value;
        newNode->height = 1;
    }
    else if(key < node->key){
        node->left = insert(node->left, key, value);
    }
    else{
        node->right = insert(node->right, key, value);
    }

    node->height = maxHeight(node->left, node->right) + 1;


    int balanceDifference = getBalance(node);
    //right-left
    if (balanceDifference < -1 && key < node->right->key){
        node->right = rotate_right(node->right);
        node = rotate_left(node);
    }
    //right-right
    else if (balanceDifference < -1 && key > node->right->key)
        node = rotate_left(node);
    //left-right
    else if (balanceDifference > -1 && key > node->left->key){
        node->left = rotate_left(node->left);
        node = rotate_right(node);
    }
    //right-left
    else if (balanceDifference > -1 && key < node->left->key)
        node = rotate_right(node);

    return node;
}

template<typename T, typename U>
U AVLTree<T,U>::search(AVLNode<T,U>*& node, const T& key) {
    //TODO
    //return NULL if there are no such key, return value if there is

}

template<typename T, typename U>
AVLNode<T,U>* AVLTree<T,U>::remove(AVLNode<T,U>*& node, const T& key) {
    //TODO
}

template<typename T, typename U>
void AVLTree<T,U>::removeall(AVLNode<T,U>*& node) {
    //TODO
    //for destructor
        
    }
    
}
