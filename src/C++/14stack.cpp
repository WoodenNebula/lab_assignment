#include <iostream>
#include <vector>
#include <optional>
#include <algorithm>

template <typename T>
class Stack {
public:
    Stack() : m_StackPtr(nullptr), m_Length(0) {}
    ~Stack() { Reset(); }

    inline size_t GetLength() const { return m_Length; }

    void Push(T value);

    std::optional<T> Pop();

    void Reset();

    const std::vector<T> StackTrace() const;

    void StackTraceView() const;

private:
    struct StackNode {
        T value;
        StackNode* prev;

        StackNode(T val) : value(val), prev(nullptr) {}
        ~StackNode() {}
    };

private:
    StackNode* m_StackPtr;
    size_t m_Length;
};



template <typename T>
void Stack<T>::Push(T value) {
    StackNode* node = new StackNode(value);

    if (m_StackPtr != nullptr) {
        node->prev = m_StackPtr;
    }
    else {
        node->prev = nullptr;
    }

    m_StackPtr = node;

    m_Length++;
}


template <typename T>
std::optional<T> Stack<T>::Pop() {
    if (m_StackPtr != nullptr) {
        StackNode* nodeToPop = m_StackPtr;
        m_StackPtr = nodeToPop->prev;

        T retVal = nodeToPop->value;

        delete nodeToPop;
        m_Length--;

        return retVal;
    }
    return {};
}

template <typename T>
void Stack<T>::Reset() {
    while (m_Length) {
        Pop();
    }
}

template <typename T>
const std::vector<T> Stack<T>::StackTrace() const {
    std::vector<T> trace;
    StackNode* traverse = m_StackPtr;

    while (traverse != nullptr) {
        trace.push_back(traverse->value);
        traverse = traverse->prev;
    }

    std::reverse(trace.begin(), trace.end());
    return trace;
}

template <typename T>
void Stack<T>::StackTraceView() const {
    const auto& stackTrace = StackTrace();

    for (size_t i = 0; i < stackTrace.size() - 1; i++)
        std::cout << stackTrace.at(i) << " <- ";

    std::cout << stackTrace.at(stackTrace.size() - 1) << std::endl;
}


int main() {
    std::cout << "Int stack:\n";
    Stack<int> intStack;

    intStack.Push(1);
    intStack.Push(5);
    intStack.Push(9);
    intStack.Push(21);

    std::cout << "Length of stack = " << intStack.GetLength() << std::endl;
    intStack.StackTraceView();
    std::cout << "Popped = " << intStack.Pop().value_or(0) << std::endl;
    intStack.StackTraceView();

    intStack.Reset();

    std::cout << "\nString Stack: \n";
    Stack<std::string> myStack;
    std::cout << "Popped = " << myStack.Pop().value_or("Nothing") << std::endl;

    myStack.Push("First");
    myStack.Push("In");
    myStack.Push("Last");
    myStack.Push("Out");

    myStack.StackTraceView();

    std::cout << "Popped = " << myStack.Pop().value_or("Nothing") << std::endl;

    myStack.StackTraceView();

    myStack.Reset();

}