#include <iostream>
#include <string>
#include <cmath>

#define COUNT 10
#define KEY_LENGTH 3

typedef struct Node {
    int key;
    std::string value;
    struct Node * left;
    struct Node * right;
    struct Node * parent;
} Node;

bool is_empty (Node * tree) {
    return tree == nullptr;
}

void empty_exc () {
    std::cout << "Error!" << std::endl;
}

int string_hash(std::string value) {
    int hash = 0;
    unsigned int length = value.length();
    for (unsigned int i = 0; i < length && i < KEY_LENGTH; ++i) {
        hash += (value[i] % 32) * ((int)pow(10, KEY_LENGTH - i));
    }
    return hash;
}

//вставка строки value по ключу x в дерево
void insert(Node * &tree, int x, std::string value) {
    Node * new_node = new Node;
    new_node->key = x;
    new_node->value = value;

    new_node->left = nullptr;
    new_node->right = nullptr;

    if (is_empty(tree)) {
        tree = new_node;
        return;
    }

    Node * temp = tree;

    while (!is_empty(temp)) {
        if (x <= temp->key) {
            if (temp->left == nullptr) {
                new_node->parent = temp;
                temp->left = new_node;
                return;
            }
            temp = temp->left;
        }
        else if (x > temp->key){
            if (temp->right == nullptr) {
                new_node->parent = temp;
                temp->right = new_node;
                return;
            }
            temp = temp->right;
        }
    }
}

Node* max_node (Node * tree) {
    if (is_empty(tree)) {
        empty_exc();
        return tree;
    }

    while (!is_empty(tree->right)) {
        tree = tree->right;
    }

    return tree;
}

Node* min_node (Node * tree) {
    if (is_empty(tree)) {
        empty_exc();
        return tree;
    }

    while (!is_empty(tree->left)) {
        tree = tree->left;
    }

    return tree;
}

Node* get_node (Node * tree, int x) {
    if (is_empty(tree)) return tree;

    if (x == tree->key) {
        return tree;
    }
    else if (x > tree->key) {
        return get_node(tree->right, x);
    }
    else if(x < tree->key) {
        return get_node(tree->left, x);
    }

    return tree;
}

Node* prev_iter (Node * tree, int x) {
    if (is_empty(tree)){
        return tree;
    }

    Node * temp = get_node(tree, x);
    if (temp == min_node(tree)) {
        return temp;
    }

    if (temp->left) {
        return max_node(temp->left);
    }

    Node * parent = temp->parent;
    while(temp == parent->left) {
        temp = parent;
        if (parent->parent) {
            parent = parent->parent;
        }
    }

    return parent;
}

Node* next_iter (Node * tree, int x) {
    if (is_empty(tree)) {
        return tree;
    }

    Node * temp = get_node(tree, x);
    if (temp->right) {
        return min_node(temp->right);
    }

    Node * parent = temp->parent;
    while(temp == parent->right) {
        temp = parent;
        if (parent->parent) {
            parent = parent->parent;
        }
    }

    return parent;
}

bool search_elem (Node * tree, int x) {
    if (is_empty(tree)) {
        empty_exc();
        return false;
    }

    while (!is_empty(tree)) {
        if (x == tree->key) {
            return true;
        }
        else if (x > tree->key) {
            tree = tree->right;
        }
        else if (x < tree->key) {
            tree = tree->left;
        }
    }

    return false;
}

void printing_search (Node * tree, int x) {
    if (!is_empty(tree)) {
        printing_search (tree->left, x);
        if (tree->key == x) std::cout << tree->value << " ";
        printing_search (tree->right, x);
    }
}

void delete_elem (Node * &tree, int x) {
    if (is_empty(tree)) {
        empty_exc();
        return;
    }

    if (!search_elem(tree, x)) {
        std::cout << "Element not found" << std::endl;
        return;
    }

    Node * temp = get_node(tree, x);
    Node * parent;

    //если нет потомков
    if (temp->left == nullptr && temp->right == nullptr) {
        if (temp->parent) {
            parent = temp->parent;

            if (temp == parent->left) {
                parent->left = nullptr;
            }
            else {
                parent->right = nullptr;
            }
        }
        delete temp;
        return;
    }
    //если два потомка
    else if (temp->left != nullptr && temp->right != nullptr) {
        //находим предыдущую итерацию
        Node * prev = prev_iter(tree, x);
        //находим, слева или справа находился предыдущий элемент
        if (prev->parent->left == prev) {
            //и удаляем на него ссылку
            prev->parent->left = nullptr;
        }
        else if (prev->parent->right == prev) {
            prev->parent->right = nullptr;
        }
        temp->key = prev->key;
        //очищаем память
        delete prev;
        return;
    }
    //только один потомок
    else {
        Node * child;

        if (temp->left) {
            child = temp->left;
        }
        else {
            child = temp->right;
        }

        if (temp->parent) {
            parent = temp->parent;

            if (parent->left == temp) {
                parent->left = child;
            }
            else if (parent->right == temp) {
                parent->right = child;
            }

            child->parent = parent;
        }
        else {
            child->parent = nullptr;
            tree = child;
            return;
        }

        delete temp;
        return;
    }
}

void inorder_print (Node * tree) {
    if (!is_empty(tree)) {
        inorder_print (tree->left);
        std::cout << tree->value << " ";
        inorder_print (tree->right);
    }
}

void print_2d(Node * tree, int distance) {
    //проверка на пустой узел
    if (is_empty(tree)) {
        return;
    }

    //увеличиваем расстояние между значениями узлов
    distance += COUNT;
    //вызываемся из правого поддерева
    print_2d(tree->right, distance);

    //"окружаем" каждое значение пробелами
    std::cout << std::endl;

    //печатаем пробелы в зависимости от уровня вложенности
    for (int i = COUNT; i < distance; i++) {
        std::cout << " ";
    }

    std::cout << tree->key << "<" << "\n";
    //вызываемся из правого поддерева
    print_2d(tree->left, distance);
}

void remove_tree(Node * &tree) {
    if (tree != nullptr) {
        remove_tree(tree->left);
        remove_tree(tree->right);
        delete tree;
        tree = nullptr;
    }
}

int main() {
    std::cout << "-------------------------------------\n"
            "Available commands: \n"
            "add <num> \n"
            "remove <num> \n"
            "remove_tree \n"
            "search <string[" << KEY_LENGTH << "]> \n"
            "prev_iter <num> \n"
            "next_iter <num> \n"
            "get_node <num> \n"
            "max_node \n"
            "min_node \n"
            "print \n"
            "------------------------------------------"<< std::endl;

    std::string user_in, user_value;
    Node * root = new Node;
    root = nullptr;

    while (std::cin >> user_in) {
        int num;

        //добавление
        if (user_in == "add") {
            std::cout << "Editing mode. Enter 0 for done" << std::endl;

            while (std::cin >> user_value) {
                if (user_value == "0") break;
                num = string_hash(user_value);
                insert(root, num, user_value);
            };
            std::cout << "Done." << std::endl;
        }

        //удаление эл-та с x ключом
        if (user_in == "remove") {
            std::cin >> user_value;
            num = string_hash(user_value);
            delete_elem(root, num);
        }

        //удаление дерева
        if (user_in == "remove_tree") {
            if (!is_empty(root)) remove_tree(root);
            else {
                std::cout << "Tree is empty!" << std::endl;
            }
        }

        //поиск строки из 3 char
        if (user_in == "search") {
            std::cin >> user_value;
            num = string_hash(user_value);

            if (search_elem(root, num)) {
                printing_search(root, num);
                std::cout << std::endl;
            }
            else {
                std::cout << "There is no such element." << std::endl;
            }
        }

        //предыдущая интерация
        if (user_in == "prev_iter") {
            std::cin >> num;
            std::cout << prev_iter(root, num)->key << std::endl;
        }

        //следующая интерация
        if (user_in == "next_iter") {
            std::cin >> num;
            std::cout << next_iter(root, num)->key << std::endl;
        }

        //узел с указанным значением
        if (user_in == "get_node") {
            std::cin >> num;
            std::cout << get_node(root, num) << std::endl;
        }

        //максимальное значение
        if (user_in == "max_node") {
            std::cout << max_node(root)->key << std::endl;
        }

        //минимальное значение
        if (user_in == "min_node") {
            std::cout << min_node(root)->key << std::endl;
        }

        if (user_in == "hash") {
            std::cin >> user_value;
            std::cout << string_hash(user_value) << std::endl;
        }

        //обход
        if (user_in == "print") {
            if (!is_empty(root)) {
                std::cin >> num;
                switch (num) {
                    case 1:
                        inorder_print(root);
                        std::cout << std::endl;
                        break;
                    case 2:
                        print_2d(root, 0);
                        std::cout << std::endl;
                        break;
                    default:
                        std::cout << "Wrong input!" << std::endl;
                        break;
                }
            }
            else {
                std::cout << "Tree is empty!" << std::endl;
            }
        }
    }
    return 0;
}