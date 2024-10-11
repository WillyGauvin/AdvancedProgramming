#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <memory>
#include <stdexcept>


class ConstantArray {
private:
   const int MAX_SIZE = 100; // Const member variable
   std::vector<int> data;
public:
    // TODO: Implement constructor that takes a const reference to a vector of ints
    // It should throw an invalid_argument exception if the initialData size exceeds MAX_SIZE
    ConstantArray(const std::vector<int>& _data) : data(_data)
    {
        if (_data.size() > MAX_SIZE)
        {
            throw std::invalid_argument("Initial data siz exceeds maximum size");
        }
    }

   // TODO: Implement const member function 'at' that returns an int
   // It should throw an out_of_range exception if the index is invalid or data size >= MAX_SIZE
    int at(size_t index) const
    {
        if (index >= data.size() || data.size() >= MAX_SIZE)
        {
            throw std::out_of_range("Index out of range");
        }
        return data.at(index);
    }

   // TODO: Implement non-const member function 'at' that returns a reference to int
   // It should throw an out_of_range exception if the index is invalid or data size >= MAX_SIZE
    int& at(size_t index)
    {
        if (index >= data.size() || data.size() >= MAX_SIZE)
        {
            throw std::out_of_range("Index out of range");
        }
        return data.at(index);
    }

   // TODO: Implement const member function 'size' that returns the size of the data
    size_t size() const
    {
        return data.size();
    }
   // TODO: Implement const member function 'getData' that returns a const reference to the data vector
    const std::vector<int>& getData() const
    {
        return data;
    }
   // TODO: Implement const member function 'getConstValue' that returns the MAX_SIZE
    const int getConstValue() const
    {
        return MAX_SIZE;
    }
};

class MathOperations {
public:
   // TODO: Implement static function 'sum' that takes a const reference to a vector of ints and returns their sum
    static int sum(const std::vector<int>& ints)
    {
        return std::accumulate(ints.begin(), ints.end(), 0);
    }
   // TODO: Implement static function 'printArray' that takes a const pointer to int and a size, and prints the array
    static void printArray(const int* arr, size_t size)
    {
        for (size_t i = 0; i < size; i++)
        {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;
    }
   // TODO: Implement static function 'findMax' that takes a const reference to a vector of ints and returns a const pointer to the maximum element
    static const int* findMax(const std::vector<int>& numbers)
    {
        return numbers.empty() ? nullptr : &(*std::max_element(numbers.begin(), numbers.end()));
    }
   // TODO: Implement static function 'passByValue' that takes an int by value and increments it
    static void passByValue(int x)
    {
        x++;
    }
   // TODO: Implement static function 'passByReference' that takes an int by reference and increments it
    static void passByReference(int& x)
    {
        x++;
    }
   // TODO: Implement static function 'passByConstReference' that takes a const reference to an int and increments it using const_cast
    static void passByConstReference(int& x)
    {
        auto test = const_cast<int*>(&x);
        (*test)++;
    }
   // TODO: Implement static function 'passByPointer' that takes a pointer to int and increments the value it points to
    static void passByPointer(int* x)
    {
        (*x)++;
    }
    // TODO: Implement static function 'passByConstPointer' that takes a const pointer to int and increments the value it points to using const_cast
    static void passByConstPointer(const int* x)
    {
        auto test = const_cast<int*>(x);
        (*test)++;
    }
};

class ConstDemo {
private:
   mutable int accessCount = 0;
public:
   // TODO: Implement const member function 'getAccessCount' that increments and returns accessCount
    int getAccessCount() const
    {
        accessCount++;
        return accessCount;
    }
};

struct Assignment4a {
   static void Execute() {
      // Example of pointer to const (const data, mutable pointer)
      int value = 10;
      const int* constPtr = &value;
      std::cout << "Pointer to const value: " << *constPtr << std::endl;
      // *constPtr = 20;  // This would be an error
      constPtr = nullptr;  // This is allowed

      // Example of const pointer (mutable data, const pointer)
      int* const ptrConst = &value;
      *ptrConst = 20;  // This is allowed
      // ptrConst = nullptr;  // This would be an error

      // Example of const reference
      const int& constRef = value;
      std::cout << "Const reference value: " << constRef << std::endl;
      // constRef = 20;  // This would be an error

      // Test ConstantArray
      ConstantArray arr({ 1, 2, 3, 4, 5 });
      std::cout << "ConstantArray at index 2: " << arr.at(2) << std::endl;

      // Demonstrating const overloading
      int& nonConstRef = arr.at(2);  // Calls non-const version
      const ConstantArray constArr({ 5, 4, 3, 2, 1 });
      int constVal = constArr.at(2);  // Calls const version
      std::cout << "Const ConstantArray at index 2: " << constVal << std::endl;

      std::cout << "ConstantArray size: " << arr.size() << std::endl;
      const auto& data = arr.getData();
      std::cout << "First element of getData(): " << data[0] << std::endl;

      // Demonstrating function returning const value
      const int constReturnValue = arr.getConstValue();
      std::cout << "Const return value: " << constReturnValue << std::endl;

      // Test MathOperations
      std::vector<int> numbers = { 1, 2, 3, 4, 5 };
      std::cout << "Sum of numbers: " << MathOperations::sum(numbers) << std::endl;

      int staticArray[] = { 5, 4, 3, 2, 1 };
      MathOperations::printArray(staticArray, 5);

      const int* maxPtr = MathOperations::findMax(numbers);
      if (maxPtr) {
         std::cout << "Max number: " << *maxPtr << std::endl;
      }

      // Demonstrating pass by value vs pass by reference
      int testValue = 50;
      std::cout << "Initial value: " << testValue << std::endl;
      MathOperations::passByValue(testValue);
      std::cout << "After pass by value: " << testValue << std::endl;  // Value unchanged
      MathOperations::passByReference(testValue);
      std::cout << "After pass by reference: " << testValue << std::endl;  // Value changed
      MathOperations::passByConstReference(testValue);
      std::cout << "After pass by const reference: " << testValue << std::endl;  // Value changed
      MathOperations::passByPointer(&testValue);
      std::cout << "After pass by pointer: " << testValue << std::endl;  // Value changed
      MathOperations::passByConstPointer(&testValue);
      std::cout << "After pass by const pointer: " << testValue << std::endl;  // Value changed

      // Test ConstDemo
      const ConstDemo demo;
      std::cout << "Access count: " << demo.getAccessCount() << std::endl;
      std::cout << "Access count: " << demo.getAccessCount() << std::endl;

      // Demonstrating const with smart pointers
      const std::unique_ptr<int> constUniquePtr = std::make_unique<int>(100);
      std::cout << "Const unique_ptr value: " << *constUniquePtr << std::endl;
      // *constUniquePtr = 200;  // This would be an error

      // Example of const_cast (use with caution!)
      const int constValue = 50;
      int* nonConstPtr = const_cast<int*>(&constValue);
      *nonConstPtr = 60;  // This is undefined behavior if constValue was originally const
      std::cout << "After const_cast: " << constValue << " (Note: this may not reflect the actual change due to undefined behavior)" << std::endl;

      // Note on compiler optimizations:
      // Const correctness allows for potential performance improvements through compiler optimizations.
      // While not easily demonstrated in a simple program, using const where appropriate can lead to 
      // more efficient code generation by the compiler.

      // Best Practices for Const Correctness:
      // 1. Variables are made const by default (e.g., MAX_SIZE, constRef, constReturnValue)
      // 2. Const references are used for passing large objects to functions (e.g., MathOperations::sum)
      // 3. Member functions are marked as const when they don't modify object state (e.g., ConstantArray::at, size, getData)
      // 4. Const pointers and references are returned when modification should not be allowed (e.g., ConstantArray::getData)
      // 5. Const usage is consistent throughout the codebase
   }
};