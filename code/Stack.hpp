#pragma once

template <typename T>
class Stack
{
	private:
		struct Node
		{
			T value;
			Node* previous;

			Node(const T& = T(), Node* const = nullptr);
			Node(T&&, Node* const = nullptr);
		};

		Node* topNode;
		size_t length;

	public:
		Stack();
		~Stack();

		bool isEmpty() const;
		int getSize() const;
		void print() const;
		const T& top() const;
		T& top();

		bool push(const T&);
		bool push(T&&);
		bool pop();
		void transferStackInReverse(Stack<T>&);
};

template <typename T>
Stack<T>::Node::Node(const T& value_, Node* const previous_) :
	value(value_),
	previous(previous_)
{}

template <typename T>
Stack<T>::Node::Node(T&& value_, Node* const previous_) :
	value(std::move(value_)),
	previous(previous_)
{}

template <typename T>
Stack<T>::Stack() :
	topNode(nullptr),
	length(0)
{}

template <typename T>
Stack<T>::~Stack()
{
	while (!isEmpty()) pop();
}

template <typename T>
bool Stack<T>::isEmpty() const
{
	if (length == 0) return 1;
	return 0;
}

template <typename T>
int Stack<T>::getSize() const
{
	return length;
}

template <typename T>
void Stack<T>::print() const
{
	Node* temp = topNode;
	while (temp != nullptr)
	{
		std::cout << temp->value << ' ';
		temp = temp->previous;
	}
	std::cout << std::endl;
}

template <typename T>
const T& Stack<T>::top() const
{
	if (isEmpty()) return *(new T);
	return topNode->value;
}

template <typename T>
T& Stack<T>::top()
{
	if (isEmpty()) return *(new T);
	return topNode->value;
}

template <typename T>
bool Stack<T>::push(const T& value)
{
	Node* temp = new(std::nothrow) Node(value, topNode);
	if (temp == nullptr) return 0;
	topNode = temp;
	length++;
	return 1;
}

template <typename T>
bool Stack<T>::push(T&& value)
{
	Node* temp = new(std::nothrow) Node(std::move(value), topNode);
	if (temp == nullptr) return 0;
	topNode = temp;
	length++;
	return 1;
}

template <typename T>
bool Stack<T>::pop()
{
	if (isEmpty()) return 0;
	Node* temp = topNode;
	topNode = topNode->previous;
	delete temp;
	length--;
	return 1;
}

template <typename T>
void Stack<T>::transferStackInReverse(Stack<T>& other)
{
	while (!other.isEmpty())
	{
		push(other.top());
		other.pop();
	}
}