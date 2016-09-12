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
  Node<T> * head;

};