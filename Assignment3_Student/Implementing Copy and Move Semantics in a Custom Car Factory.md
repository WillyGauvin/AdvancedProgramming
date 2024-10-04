# Assignment 3: Implementing Copy and Move Semantics in a Custom Car Factory

## Objective

Develop a C++ application that simulates a custom car factory, focusing on **copy and move semantics**, **deep vs. shallow copies**, **smart pointers and ownership**, **perfect forwarding**, and demonstrating an understanding of **lvalues and rvalues**. This assignment will help you apply C++ object semantics to manage resources efficiently in a practical, real-world scenario.

## Background

In a custom car factory, various resources like engines, chassis, and specialized tools need careful management. Actions such as duplicating engine designs, transferring engines between assembly lines, and using tools on different engines mirror the concepts of copying, moving, and perfect forwarding in C++. Efficient resource management in code translates to optimal performance and resource utilization in the factory.

## Requirements

### 1. Engine Class

Implement an `Engine` class that represents a car engine in the factory:

- **Attributes**:
  - `model` (`std::string`): The engine model name.
  - `horsepower` (`int`): The engine's horsepower.
  - `serialNumber` (`std::unique_ptr<std::string>`): A unique identifier for the engine.
  - `isPrototype` (`bool`): Indicates if the engine is a prototype (immutable).

- **Methods**:
  - **Constructor**: Initializes the engine with `model`, `horsepower`, `serialNumber`, and optional `isPrototype` (default is `false`).
  - **Copy Constructor**: Performs a **deep copy**, simulating creating an exact replica of an engine (e.g., duplicating designs for mass production).
  - **Move Constructor** (`noexcept`): Transfers resources efficiently, simulating moving an engine to a different assembly line.
  - **Copy Assignment Operator**: Implements deep copying.
  - **Move Assignment Operator** (`noexcept`): Implements efficient resource transfer.
  - **Destructor**: Ensures proper resource cleanup.

- **Accessors**:
  - **`getSerialNumber() const`**: Returns a const reference to the engine's serial number.
  - **`getModel() const`**: Returns a const reference to the engine's model name.
  - **`setModel(const std::string& model)`**: Updates the engine's model name.
  - **`getHorsepower() const`**: Returns the engine's horsepower.

### 2. AssemblyLine Class

Implement an `AssemblyLine` class that manages a collection of engines:

- **Attributes**:
  - `name` (`std::string`): The name of the assembly line.
  - `engines` (`std::vector<std::shared_ptr<Engine>>`): A list of engines on the line. Using `shared_ptr` to simulate shared ownership (e.g., multiple departments working on the same engine).

- **Methods**:
  - **Constructor**: Initializes the assembly line with a name.
  - **`addEngine(std::shared_ptr<Engine> engine)`**: Adds an engine to the line, simulating collaboration without transferring ownership.
  - **`removeEngine(const std::string& serialNumber)`**: Removes an engine from the line, simulating taking an engine off the line for specialized work.
  - **`transferEngine(const std::string& serialNumber, AssemblyLine& otherLine)`**: Moves an engine to another assembly line using **move semantics**.

- **Accessors**:
  - **`getName() const`**: Returns a const reference to the assembly line's name.
  - **`getEngines() const`**: Returns a const reference to the list of engines on the line.

### 3. SpecializedTool Class

Implement a `SpecializedTool` class to demonstrate **perfect forwarding** and **const correctness**:

- **Attributes**:
  - `name` (`std::string`): The tool's name.

- **Method**:
  - **`template<typename T> void use(T&& engine)`**: Uses the tool on an engine, preserving its value category (lvalue or rvalue) and constness, simulating working on both existing engines and temporary prototype engines.

### 4. Execute Function

Implement a function, e.g., `Assignment3b::Execute()`, which acts as the main function:

- Create several `Engine` objects, including prototypes (immutable engines).
- Demonstrate **deep copying** (creating independent duplicates) and **move semantics** (transferring ownership).
- Create `AssemblyLine` instances and add engines to them.
- Demonstrate transferring engines between assembly lines.
- Use the `SpecializedTool` on engines, showing **perfect forwarding** and handling of lvalues and rvalues.
- Ensure proper use of **smart pointers** (`std::unique_ptr`, `std::shared_ptr`).

## Instructions

1. **Copy Constructors and Deep Copying**:
   - Implement the copy constructor and copy assignment operator in `Engine` to perform a **deep copy** of the `serialNumber`.
   - Explain in comments how this prevents issues like double deletions and ensures independent resource management.

2. **Move Constructors and Efficient Resource Transfer**:
   - Implement move constructor and move assignment operator in `Engine`, marked with `noexcept`.
   - Use `std::move()` to transfer ownership, simulating moving engines without the overhead of copying.

3. **Smart Pointers and Ownership**:
   - Use `std::unique_ptr` for `serialNumber` in `Engine` to represent unique ownership.
   - Use `std::shared_ptr` in `AssemblyLine` to allow multiple assembly lines to work with the same engine (shared ownership).
   - Discuss in comments how smart pointers manage dynamic memory and ownership semantics.

4. **Perfect Forwarding and Value Categories**:
   - In `SpecializedTool`, implement `use()` as a template method utilizing **forwarding references** and `std::forward<>`.
   - Ensure that `use()` preserves the value category and constness of the engine, simulating the tool's ability to work with both temporary (prototype) and existing engines.

5. **Const Correctness**:
   - Mark methods that do not modify the object's state as `const`.
   - Handle `const` engines appropriately in `SpecializedTool::use()`.

6. **Lvalues vs. Rvalues**:
   - Demonstrate the difference between lvalue and rvalue operations in your implementations.

7. **Exception Safety with `noexcept`**:
   - Mark move operations as `noexcept` to enable optimizations and ensure exception safety.

8. **Deep vs. Shallow Copy**:
   - Illustrate the difference between deep and shallow copies in your `Engine` class.

## Example Usage

```cpp
struct Assignment3b {
    static void Execute() {
        // Create engines
        Engine v8("V8 Turbo", 450, "123");
        std::cout << "Engine constructed: " << v8.getSerialNumber() << "\n";

        Engine v6("V6 Eco", 300, "456");
        std::cout << "Engine constructed: " << v6.getSerialNumber() << "\n";

        const Engine prototype("Prototype X1", 500, "789", true); // Immutable prototype engine
        std::cout << "Engine constructed: " << prototype.getSerialNumber() << "\n";

        // Demonstrate deep copy (creating an independent duplicate)
        Engine v8Copy = v8; // Invokes copy constructor
        std::cout << "Engine copied: " << v8Copy.getSerialNumber() << "\n";

        // Modify the copy to show independence
        v8Copy.setModel("V8 Turbo Modified");
        std::cout << "Modified engine copy model to: " << v8Copy.getModel() << "\n";

        // Demonstrate move semantics (transferring ownership)
        Engine v6Moved = std::move(v6); // Invokes move constructor
        std::cout << "Engine moved: " << v6Moved.getSerialNumber() << "\n";

        // v6 is now in a valid but unspecified state

        // Create assembly lines
        AssemblyLine sportsCars("Sports Car Line");
        std::cout << "AssemblyLine created: " << sportsCars.getName() << "\n";

        AssemblyLine ecoCars("Eco-Friendly Line");
        std::cout << "AssemblyLine created: " << ecoCars.getName() << "\n";

        // Add engines to assembly lines using shared ownership
        auto v8CopyShared = std::make_shared<Engine>(v8Copy); // Copy for shared ownership
        std::cout << "Engine copied: " << v8CopyShared->getSerialNumber() << " for shared ownership\n";
        sportsCars.addEngine(v8CopyShared);
        std::cout << "Engine " << v8CopyShared->getSerialNumber() << " added to " << sportsCars.getName() << "\n";

        auto v6MovedShared = std::make_shared<Engine>(v6Moved); // Copy for shared ownership
        std::cout << "Engine copied: " << v6MovedShared->getSerialNumber() << " for shared ownership\n";
        ecoCars.addEngine(v6MovedShared);
        std::cout << "Engine " << v6MovedShared->getSerialNumber() << " added to " << ecoCars.getName() << "\n";

        // Transfer engine between assembly lines (move semantics)
        sportsCars.transferEngine(v8CopyShared->getSerialNumber(), ecoCars);
        std::cout << "Engine " << v8CopyShared->getSerialNumber() << " transferred from "
                  << sportsCars.getName() << " to " << ecoCars.getName() << "\n";

        // Use specialized tool on engines (perfect forwarding)
        SpecializedTool diagnosticTool("Engine Diagnostic Tool");
        diagnosticTool.use(v8Copy);                     // lvalue
        diagnosticTool.use(Engine("V12", 700, "987"));  // rvalue (temporary object)
        diagnosticTool.use(prototype);                  // const lvalue

        // Display engines on assembly lines
        std::cout << "\n" << "Assembly Line: " << sportsCars.getName() << "\n";
        for (const auto& engine : sportsCars.getEngines()) {
            std::cout << "  Engine " << engine->getSerialNumber()
                      << " (" << engine->getModel() << ", " << engine->getHorsepower() << " HP)\n";
        }

        std::cout << "\n" << "Assembly Line: " << ecoCars.getName() << "\n";
        for (const auto& engine : ecoCars.getEngines()) {
            std::cout << "  Engine " << engine->getSerialNumber()
                      << " (" << engine->getModel() << ", " << engine->getHorsepower() << " HP)\n";
        }
    }
};
```

## Key Takeaways

- **Copy Semantics**: Creating independent duplicates is like manufacturing exact replicas of engines for different cars.
- **Move Semantics**: Transferring ownership efficiently mirrors moving engines between assembly lines without unnecessary duplication.
- **Deep vs. Shallow Copy**: Deep copying ensures each engine has its own unique parts, preventing conflicts; shallow copying might lead to shared parts causing issues.
- **Smart Pointers**: Manage ownership and lifetime of resources, similar to tracking which assembly lines are responsible for which engines.
- **Perfect Forwarding**: Allows tools to work with any engine, whether it's a prototype (temporary) or an existing engine, preserving their specific characteristics.
- **Lvalues and Rvalues**: Understanding the difference helps in managing resources effectively, just like knowing whether an engine is ready for use or is a temporary prototype.
- **Exception Safety**: Marking move operations as `noexcept` ensures stability in resource transfers, akin to guaranteeing safe transportation of engines between factory sections.