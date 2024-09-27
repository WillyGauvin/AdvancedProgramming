#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

struct Tasks {

   class Toy {
   public:
      Toy() = default;
      virtual ~Toy() = default;
   };

   // Task 1: Create a unique_ptr to a Toy
   static std::unique_ptr<Toy> createUniqueToy() {
      // return the unique_ptr
       return std::make_unique<Toy>();
   }

   // Task 2: Transfer ownership of a unique_ptr
   static std::unique_ptr<Toy> transferOwnership(std::unique_ptr<Toy> toy) {
      // return the result of transferring ownership
       return std::move(toy);
   }

   // Task 3: Create a shared_ptr to a Toy
   static std::shared_ptr<Toy> createSharedToy() {
      // return the shared_ptr
       return std::make_shared<Toy>();
   }

   // Task 4: Create a weak_ptr from a shared_ptr
   static std::weak_ptr<Toy> createWeakPtr(std::shared_ptr<Toy> toy) {
      // return the weak_ptr
       return std::weak_ptr<Toy>(toy);
   }

   // Task 5: Implement a function to check if a weak_ptr is valid
   static bool isWeakPtrValid(std::weak_ptr<Toy> weakToy) {
      // return whether the weak_ptr is valid
       return !weakToy.expired();
   }

   // Task 6: Implement a toy box using unique_ptr
   class ToyBox {
   public:
      void addToy(std::unique_ptr<Toy> toy) {
         // add the toy to the box
          toys.push_back(std::move(toy));
      }

      size_t getToyCount() const {
         // return the number of toys in the box
          return toys.size();
      }

      std::unique_ptr<Toy> removeToy(size_t index) {
         // check if the index is valid
         if (index >= toys.size())
         {
             return nullptr;
         }
         // transfer ownership of the toy at the index
         std::unique_ptr<Toy> toy = std::move(toys[index]);
         // remove the toy from the box
         toys.erase(toys.begin() + index);
         // return the toy (out of this function)
         return toy;
      }

   private:
      std::vector<std::unique_ptr<Toy>> toys;
   };

   // Task 7: Implement a function to create a custom deleter for a shared_ptr
   static std::shared_ptr<Toy> createSharedToyWithCustomDeleter() {
       return std::shared_ptr<Toy>(new Toy(), [](Toy* ptr) {
           //this is our custom deleter
           std::cout << "Toy deleted!" << std::endl;
           delete ptr;
           });
   }


   // Task 8: Implement a function to create a vector of shared_ptrs
   static std::vector<std::shared_ptr<Toy>> createSharedToyVector(int count) {
       // create a vector of shared_ptrs to Toy objects
       std::vector<std::shared_ptr<Toy>> toys;

       for (int i = 0; i < count; i++)
       {
           toys.push_back(std::make_shared<Toy>());
       }
       return toys;
      // populate the vector with the specified number of shared_ptrs
      // return the vector
   }

   // Task 9: Implement a function to find the shared_ptr with the highest use_count
   static std::shared_ptr<Toy> findMostSharedToy(const std::vector<std::shared_ptr<Toy>>& toys) {
      // return nullptr if the vector is empty
       if (toys.empty())
       {
           return nullptr;
       }
       std::shared_ptr<Toy> maxToy = toys[0];

       for (size_t i = 1; i < toys.size(); ++i)
       {
           if (toys[i].use_count() > maxToy.use_count())
           {
               maxToy = toys[i];
           }
       }

       return maxToy;
   }
};

struct Assignment2a {
   static void Execute()
   {
      // Task 1: Create a unique_ptr to a Toy
      auto toy = Tasks::createUniqueToy();

      std::cout << "Toy created" << std::endl;

      // Task 2: Transfer ownership of a unique_ptr
      auto movedToy = Tasks::transferOwnership(std::move(toy));

      std::cout << "Toy ownership transferred" << std::endl;

      // Task 3: Create a shared_ptr to a Toy
      auto sharedToy = Tasks::createSharedToy();

      std::cout << "Shared pointer created" << std::endl;

      // Task 4: Create a weak_ptr from a shared_ptr
      auto weakToy = Tasks::createWeakPtr(sharedToy);

      std::cout << "Weak pointer created" << std::endl;

      // Task 5: Implement a function to check if a weak_ptr is valid
      bool valid = Tasks::isWeakPtrValid(weakToy);
      std::cout << "Weak pointer is " << (valid ? "valid" : "invalid") << std::endl;

      // Task 6: Implement a toy box using unique_ptr
      Tasks::ToyBox box;
      box.addToy(Tasks::createUniqueToy());
      std::cout << "Toy count in box: " << box.getToyCount() << std::endl;

      // Task 7: Implement a function to create a custom deleter for a shared_ptr
      auto customDeleterToy = Tasks::createSharedToyWithCustomDeleter();

      std::cout << "Custom deleter shared pointer created" << std::endl;

      // Task 8: Implement a function to create a vector of shared_ptrs
      auto sharedToys = Tasks::createSharedToyVector(5);

      std::cout << "Shared pointer vector created" << std::endl;

      // Task 9: Implement a function to find the shared_ptr with the highest use_count
      auto mostSharedToy = Tasks::findMostSharedToy(sharedToys);

      std::cout << "Most shared toy found" << std::endl;
   }
};