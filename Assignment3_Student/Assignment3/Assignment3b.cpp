#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <utility>
#include <algorithm>

class Engine {
private:
    std::string model;
    int horsepower;
    std::unique_ptr<std::string> serialNumber;
    bool isPrototype;

public:
    Engine(const std::string& _model, int _horsepower, const std::string& _serialNumber, bool _isPrototype = false) :
        model(_model), horsepower(_horsepower), isPrototype(_isPrototype)
    {
        serialNumber = std::make_unique<std::string>(_serialNumber);
    }

    Engine(const Engine& other)
        : model(other.model), horsepower(other.horsepower), isPrototype(other.isPrototype)
    {
        serialNumber = std::make_unique<std::string>(*other.serialNumber);
    }

    ~Engine() {}

    Engine(Engine&& other) noexcept
        : model(std::move(other.model)), horsepower(other.horsepower), isPrototype(other.isPrototype) {

        other.horsepower = 0;
        serialNumber = std::move(other.serialNumber);
    }

    // Task 5: Implement the move assignment operator (noexcept)
    Engine& operator=(Engine&& other) noexcept {
        // Handle self-assignment
        if (this == &other)
            return *this;
        // Implement move assignment operator
        model = std::move(other.model);
        horsepower = other.horsepower;
        serialNumber = std::move(other.serialNumber);
        other.horsepower = 0;
        return *this;
    }

    const std::string& getSerialNumber() const { return *serialNumber; }
    const std::string& getModel() const { return model; }
    void setModel(const std::string& _model) { model = _model; }
    const int getHorsepower() const { return horsepower; }
};

class AssemblyLine {
private:
    std::string name;
    std::vector<std::shared_ptr<Engine>> engines;

public:
    AssemblyLine(const std::string& _name) : name(_name)
    {

    }

    void addEngine(std::shared_ptr<Engine> engine) { engines.push_back(engine); }
    void removeEngine(const std::string& serialNumber) 
    {
        for (int i = 0; i < engines.size(); i++)
        {
            if (engines[i]->getSerialNumber() == serialNumber)
            {
                engines.erase(engines.begin() + i);
                return;
            }
        }
        std::cout << "No engine with serial number: " << serialNumber << std::endl;
    }
    void transferEngine(const std::string& serialNumber, AssemblyLine& otherline)
    {
       for (int i = 0; i < engines.size(); i++)
       {
           if (engines[i]->getSerialNumber() == serialNumber)
           {
               std::shared_ptr<Engine> engine = std::shared_ptr<Engine>(engines[i]);
               otherline.addEngine(engine);
               removeEngine(serialNumber);
           }
       }
    }
    const std::string& getName() const { return name; }
    const std::vector<std::shared_ptr<Engine>>& getEngines() const { return engines; }
};

class SpecializedTool {
public:
    SpecializedTool(const std::string& _name) : name(_name)
    {
        std::cout << "SpecializedTool created: " << name << std::endl;
    }
    template<typename T> void use(T&& engine)
    {
        workOnEngine(std::forward<T>(engine));
    }
private:
   // Helper method to simulate working on the engine
   void workOnEngine(Engine& engine) {
      std::cout << name << " is working on Engine " << engine.getSerialNumber() << " (lvalue)\n";
   }

   void workOnEngine(const Engine& engine) {
      std::cout << name << " is inspecting Prototype Engine " << engine.getSerialNumber()
         << " (const lvalue)\n";
   }

   void workOnEngine(Engine&& engine) {
      std::cout << name << " is modifying temporary Engine " << engine.getSerialNumber()
         << " (rvalue)\n";
   }

   std::string name;
};

struct Assignment3b {
   static void Execute()
   {
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