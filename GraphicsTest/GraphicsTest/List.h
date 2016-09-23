template<typename T>
class Node
{
public:
  T value;
  Node<T> * next;
};

template <typename T>
class List
{
public:
  List(){ head = nullptr; }
  //typedef TYPE T;
  void AddToFront(T val){
    Node<T> * n = new Node < T > ;
    n->value = val;
    n->next = head;
    head = n;
  }
  void Remove(Node<T> * target){
    Node<T> * node = head;
    if (target == head){
      head = head->next;
      delete node;
      return;
    }
    while (node){
      if (node->next == target){
        auto* temp = node->next;
        node->next = temp->next;
        delete temp;
        return;
      }
    }
  }
  Node<T> * head;

};