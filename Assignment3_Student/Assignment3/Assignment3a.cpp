#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>

struct Tasks {

   class Book {
   public:
      // Task 1: Implement the Book constructor
      Book(const std::string& title, int pages) 
          : title(title), pages(pages) {
          static int serialCounter = 1000;
          serialNumber = std::make_unique<std::string>(std::to_string(serialCounter++));
      }

      // Task 2: Implement the copy constructor (deep copy)
      Book(const Book& other) 
        : title(other.title), pages (other.pages){
         // Implement deep copy of the book, including serialNumber
          serialNumber = std::make_unique<std::string>(*other.serialNumber);
      }

      // Task 3: Implement the move constructor (noexcept)
      Book(Book&& other) noexcept 
        : title(std::move(other.title)), pages(other.pages){
         // Implement move constructor
          other.pages = 0;
          serialNumber = std::move(other.serialNumber);
      }

      // Task 4: Implement the copy assignment operator (deep copy)
      Book& operator=(const Book& other) {
         // Handle self-assignment
          if (this == &other)
              return *this;
         // Implement copy assignment operator with deep copy
          title = other.title;
          pages = other.pages;
          serialNumber = std::make_unique<std::string>(*other.serialNumber);
          return *this;
      }

      // Task 5: Implement the move assignment operator (noexcept)
      Book& operator=(Book&& other) noexcept {
         // Handle self-assignment
          if (this == &other)
            return *this;
         // Implement move assignment operator
          title = std::move(other.title);
          pages = other.pages;
          serialNumber = std::move(other.serialNumber);
          other.pages = 0;
          return *this;
      }

      // Task 6: Implement the destructor
      ~Book() {
         // Ensure proper resource cleanup
      }

      // Accessors
      const std::string& getSerialNumber() const {
         // Return the serial number
          return *serialNumber;
      }

      const std::string& getTitle() const {
         // Return the title
          return title;
      }

      void setTitle(const std::string& title) {
         // Update the title
          this->title = title;
      }

      int getPages() const {
         // Return the number of pages
          return pages;
      }

   private:
      std::string title;
      int pages;
      std::unique_ptr<std::string> serialNumber; // Unique ownership
   };

   // Task 7: Implement a function to create a Book object
   static Book createBook(const std::string& title, int pages) {
      // Return a Book object
       return Book(title, pages);
   }

   // Task 8: Implement a function to copy a Book
   static Book copyBook(const Book& book) {
      // Return a copy of the Book
       return Book(book);
   }

   // Task 9: Implement a function to move a Book
   static Book moveBook(Book&& book) {
      // Return a moved Book
       return std::move(book);
   }

   // Task 10: Implement the Librarian class
   class Librarian {
   public:
      // Constructor
      Librarian(const std::string& name) 
        : name(name){
         // Initialize the librarian with a name
      }

      // Task 11: Implement the process method with perfect forwarding
      template<typename T>
      void process(T&& book) {
         // Process the book, preserving value category
          handleBook(std::forward<T>(book));
         // Call the appropriate handleBook method
      }

   private:
      std::string name;

      // Helper methods
      void handleBook(Book& book) {
         // Handle lvalue Book
          std::cout << name << " is processing Book " << book.getSerialNumber() << " (lvalue)\n";
      }

      void handleBook(const Book& book) {
         // Handle const lvalue Book
          std::cout << name << " is reviewing Book " << book.getSerialNumber() << " (const lvalue)\n";
      }

      void handleBook(Book&& book) {
         // Handle rvalue Book
          std::cout << name << " is catatloging temporary Book " << book.getSerialNumber() << " (rvalue)\n";
      }
   };

};

struct Assignment3a {
   static void Execute() {
      auto book1 = Tasks::createBook("C++ Programming", 500);
      std::cout << "Book created: " << book1.getSerialNumber() << std::endl;

      auto bookCopy = Tasks::copyBook(book1);
      std::cout << "Book copied: " << bookCopy.getSerialNumber() << std::endl;

      auto bookMoved = Tasks::moveBook(std::move(book1));
      std::cout << "Book moved: " << bookMoved.getSerialNumber() << std::endl;

      Tasks::Librarian librarian("Alice");

      librarian.process(bookCopy); // lvalue
      librarian.process(Tasks::Book("Python Programming", 400)); // rvalue
   }
};
