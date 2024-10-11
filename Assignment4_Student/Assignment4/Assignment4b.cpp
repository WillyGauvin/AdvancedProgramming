#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <memory>
#include <exception>
#include <stdexcept>
#include <type_traits>
#include <optional>
#include <variant>
#include <expected>

// Custom exception classes
class FileNotFoundException : public std::runtime_error {
public:
   explicit FileNotFoundException(const std::string& filename) :
       std::runtime_error("File not found : " + filename) {}
   // TODO: Implement the constructor to initialize the runtime_error with an appropriate message

};

class DivisionByZeroException : public std::runtime_error {
public:
   DivisionByZeroException()
       : std::runtime_error("Division by zero error") {}
   // TODO: Implement the constructor to initialize the runtime_error with an appropriate message
};

// Class demonstrating exception throwing
class Calculator {
public:
   // Function that throws an exception on division by zero
    double divide(double numerator, double denominator)
    {
        if (denominator == 0.0)
        {
            throw DivisionByZeroException();
        }
        return numerator / denominator;
    }
   // TODO: Implement divide to throw DivisionByZeroException when denominator is zero

   // Function that throws a custom exception
    void openFile(const std::string& filename)
    {
        bool fileExists = false; //Assume the file doesn't exist
        if (!fileExists)
        {
            throw FileNotFoundException(filename);
        }
    }
   // TODO: Implement openFile to throw FileNotFoundException if the file does not exist
};

// Class demonstrating exception safety guarantees
class ResourceManager {
public:
   // TODO: Implement addResource to add a resource and throw an exception if resource is negative
   // Basic Guarantee: Ensures resources are not leaked even if an exception is thrown
    void addResource(int resource)
    {
        resources.push_back(resource);

        //Simulated exception
        if (resource < 0)
        {
            throw std::runtime_error("Negative resource not allowed");
        }
    }

   // Strong Guarantee: Operation either completes fully or has no effect
    void replaceResources(const std::vector<int>& newResources)
    {
        std::vector<int> temp = newResources; //May throw
        resources.swap(temp); // No-throw
    }
   // TODO: Implement replaceResources to replace resources with strong exception safety

private:
   std::vector<int> resources;
};

// Class demonstrating the use of noexcept
class NoThrowOperations {
public:
   // Function that guarantees not to throw exceptions
   // TODO: Implement safeOperation to perform operations that do not throw exceptions
    void safeOperation() noexcept
    {
        std::cout << "Safe operation executed." << std::endl;
    }


   // Function that conditionally noexcept based on the type
   // TODO: Implement processResource to process the resource with conditional noexcept
   template <typename T>
   void processResource(T&& resource) noexcept(std::is_nothrow_move_constructible_v<T>)
   {
       std::cout << "Procesesing resource" << std::endl;

   }
};

// Class demonstrating nested exceptions
class NestedExceptionDemo {
public:
   // TODO: Implement innerFunction to throw a runtime_error
    void innerFunction()
    {
        throw std::runtime_error("Inner exception");
    }

   // TODO: Implement outerFunction to call innerFunction and throw a nested exception
    void outerFunction()
    {
        try 
        {
            innerFunction();
        }
        catch (...)
        {
            std::throw_with_nested(std::runtime_error("Outer Exception"));
        }
    }
};

// Function demonstrating stack unwinding
static void riskyFunction()
{
// TODO: Implement riskyFunction to perform operations that may throw exceptions and demonstrate stack unwinding
    std::unique_ptr<int> ptr(new int(10));
    throw std::runtime_error("Error in riskyFunction");
    //ptr is automatically deleted during the stack unwinding.
}

// Class demonstrating custom exception handling
class ExceptionHandler {
public:
   // TODO: Implement handleDivision to handle DivisionByZeroException
    static void handleDivision()
    {
        Calculator calc;
        try
        {
            double result = calc.divide(10.0f, 0.0); //Will throw an exception
            std::cout << "Division Result: " << result << std::endl;
        }
        catch(const DivisionByZeroException& e)
        {
            std::cerr << "Caught DivisionByZeroException: " << e.what() << std::endl;
        }
    }

   // TODO: Implement handleFileOpening to handle FileNotFoundException
    static void handleFileOpening()
    {
        Calculator calc;
        try
        {
            calc.openFile("Data.text");
        }
        catch (const FileNotFoundException& e)
        {
            std::cerr << "Caught FileNotFoundException: " << e.what() << std::endl;
        }
    }

   // TODO: Implement handleNestedExceptions to handle nested exceptions
    static void handleNestedExceptions()
    {
        NestedExceptionDemo demo;
        try
        {
            demo.outerFunction();
        }
        catch (const std::exception& e)
        {
            std::cerr << "Caught exception: " << e.what() << std::endl; //Outer
            try
            {
                std::rethrow_if_nested(e);
            }
            catch(const std::exception& nested)
            {
                std::cerr << "Nested exception: " << nested.what() << std::endl; //Inner

            }
        }
    }

   // TODO: Implement handleStackUnwinding to handle exceptions during stack unwinding
    static void handleStackUnwinding()
    {
        try
        {
            riskyFunction();
        }
        catch (const std::exception& e)
        {
            std::cerr << "Caught Exception during stack unwinding" << e.what() << std::endl;
        }
    }
};

// Class demonstrating exception safety guarantees
class ExceptionSafetyDemo {
public:
   // TODO: Implement demonstrateBasicGuarantee to show basic exception safety
    void demonstrateBasicGuarantee()
    {
        ResourceManager manager;
        try
        {
            manager.addResource(1);
            manager.addResource(-1); //This will throw
        }
        catch(const std::exception& e)
        {
            std::cerr << "Basic guarantee Caught Exception: " << e.what() << std::endl;
        }
    }

   // TODO: Implement demonstrateStrongGuarantee to show strong exception safety
    void demonstrateStrongGuarantee()
    {
        ResourceManager manager;
        std::vector<int> newResources = { 2,3,4 };

        try
        {
            manager.replaceResources(newResources);
            std::cout << "Resources replaced successfully." << std::endl;
        }
        catch(const std::exception& e)
        {
            std::cerr << "Strong guarantee caught exception" << e.what() << std::endl;
        }
    }
};

// Class demonstrating the use of noexcept
class NoexceptDemo {
public:
    void executeSafeOperation() noexcept
    {
        operations.safeOperation();
    }
   // TODO: Implement executeSafeOperation to call NoThrowOperations::safeOperation

    void executeProcessResource()
    {
        operations.processResource<int>(5);

    }
   // TODO: Implement executeProcessResource to call NoThrowOperations::processResource
private:
    NoThrowOperations operations;
};

// Class demonstrating alternatives to exceptions
class AlternativeErrorHandling {
public:
   // Using std::optional
    std::optional<int> findValue(const std::vector<int>& vec, int target)
    {
        auto it = std::find(vec.begin(), vec.end(), target);
        if (it != vec.end())
        {
            return *it;
        }
        return std::nullopt;
    }
   // TODO: Implement findValue to return an optional<int> based on the search result

   // Using std::variant
    std::variant<int, std::string> compute(int a, int b)
    {
        if (b == 0)
        {
            return std::string("Division by zero");
        }
        return a / b;
    }
   // TODO: Implement compute to return either the division result or an error message

   // Using std::expected (C++23)
    std::expected<int, std::string> safeDivide(int a, int b)
    {
        if (b == 0)
        {
            return std::unexpected<std::string>("Division by zero");
        }
        return a / b;
    }
   // TODO: Implement safeDivide to return expected<int, string> based on division validity
};

// Class demonstrating smart pointers and exception safety
class SmartPointerDemo {
public:
   // TODO: Implement demonstrateUniquePtr to show exception safety with std::unique_ptr
    void demonstrateUniquePtr()
    {
        try
        {
            std::unique_ptr<int> ptr = std::make_unique<int>(42);
            std::cout << "UniquePtr Value: " << *ptr << std::endl;

            //Simular Exception
            throw std::runtime_error("Exception after unique_ptr creation");
        }
        catch(const std::exception& e)
        {
            std::cerr << "Caught Exception with UniquePtr: " << e.what() << std::endl;
            // ptr is automatically deleted
        }
    }

   // TODO: Implement demonstrateSharedPtr to show exception safety with std::shared_ptr
    void demonstrateSharedPtr()
    {
        try
        {
            std::shared_ptr<int> ptr1 = std::make_shared<int>(100);
            std::shared_ptr<int> ptr2 = ptr1;

            std::cout << "SharedPtr Value: " << *ptr1 << std::endl;

            //Simular Exception
            throw std::runtime_error("Exception after shared_ptr creation");
        }
        catch (const std::exception& e)
        {
            std::cerr << "Caught Exception with SharedPtr: " << e.what() << std::endl;
            //ptr1 and ptr2 are automatically deleted / managed;
        }
    }
};

// Class demonstrating exception handling best practices
class BestPracticesDemo {
public:
   // TODO: Implement executeBestPractices to demonstrate best practices in exception handling
    void executeBestPractices()
    {
        //1. Use exceptions sparingly
        try
        {
            throw std::runtime_error("An exceptional case");
        }
        catch(const std::exception& e)
        {
            std::cerr << "Handled runtime error: " << e.what() << std::endl;
        }

        //2. Catch by const reference to avoid slicing
        try
        {
            throw FileNotFoundException("config.yaml");
        }
        catch (const FileNotFoundException& e)
        {
            std::cerr << "Handled FileNotFound error: " << e.what() << std::endl;
        }
        
        // 3. Order catch blocks from most specific to most general
        try
        {
            throw DivisionByZeroException();
        }
        catch (const DivisionByZeroException& e)
        {
            std::cerr << "Handled DivisionByZeroException error: " << e.what() << std::endl;

        }
        catch (const std::exception& e)
        {
            std::cerr << "Handled general exception: " << e.what() << std::endl;
        }

        noexceptDemo.executeSafeOperation();
        noexceptDemo.executeProcessResource();
    }

private:
   NoexceptDemo noexceptDemo;
};

// Class demonstrating nested exceptions
class NestedExceptionHandler {
public:
    void executeNestedException()
    {
        NestedExceptionDemo demo;
        ExceptionHandler::handleNestedExceptions();
    }
   // TODO: Implement executeNestedException to handle nested exceptions using NestedExceptionDemo and ExceptionHandler
};

// Assignment Execution Class
struct Assignment4b {
   static void Execute() {
      // Test ExceptionHandler
      std::cout << "Testing Division By Zero Exception" << std::endl;
      ExceptionHandler::handleDivision();
      std::cout << std::endl;

      std::cout << "Testing File Not Found Exception" << std::endl;
      ExceptionHandler::handleFileOpening();
      std::cout << std::endl;

      std::cout << "Testing Nested Exceptions" << std::endl;
      ExceptionHandler::handleNestedExceptions();
      std::cout << std::endl;

      std::cout << "Testing Stack Unwinding" << std::endl;
      ExceptionHandler::handleStackUnwinding();
      std::cout << std::endl;

      // Test Exception Safety Guarantees
      std::cout << "Testing Basic Guarantee" << std::endl;
      ExceptionSafetyDemo safetyDemo;
      safetyDemo.demonstrateBasicGuarantee();
      std::cout << std::endl;

      std::cout << "Testing Strong Guarantee" << std::endl;
      safetyDemo.demonstrateStrongGuarantee();
      std::cout << std::endl;

      // Test Noexcept
      std::cout << "Testing Noexcept Operations" << std::endl;
      NoexceptDemo noexceptDemo;
      noexceptDemo.executeSafeOperation();
      noexceptDemo.executeProcessResource();
      std::cout << std::endl;

      // Test Alternatives to Exceptions
      std::cout << "Testing Alternatives to Exceptions" << std::endl;
      AlternativeErrorHandling alternative;
      std::vector<int> numbers = { 1, 2, 3, 4, 5 };
      auto found = alternative.findValue(numbers, 3);
      if (found.has_value()) {
         std::cout << "Found value: " << found.value() << std::endl;
      }
      else {
         std::cout << "Value not found." << std::endl;
      }

      auto computeResult = alternative.compute(10, 2);
      if (std::holds_alternative<int>(computeResult)) {
         std::cout << "Compute Result: " << std::get<int>(computeResult) << std::endl;
      }
      else {
         std::cout << "Compute Error: " << std::get<std::string>(computeResult) << std::endl;
      }
      std::cout << std::endl;

      // Test Smart Pointers and Exception Safety
      std::cout << "Testing Smart Pointers and Exception Safety" << std::endl;
      SmartPointerDemo smartDemo;
      smartDemo.demonstrateUniquePtr();
      smartDemo.demonstrateSharedPtr();
      std::cout << std::endl;

      // Test Best Practices
      std::cout << "Testing Exception Handling Best Practices" << std::endl;
      BestPracticesDemo bestPractices;
      bestPractices.executeBestPractices();
      std::cout << std::endl;
   }
};