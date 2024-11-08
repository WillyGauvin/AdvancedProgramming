#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <atomic>
#include <future>
#include <chrono>
#include <syncstream> // Added for synchronized output

struct Tasks {
    // Task 1: Basic Thread Management
    // Instructions:
    // Implement the KitchenStaff class with the following members & methods:
    //    a. prepareTask(const std::string& dish) - Outputs "Chef preparing: <dish>"
    //       i. Uses std::osyncstream to synchronize output to std::cout
    //    b. run() - Starts two threads to prepare a main course and wash dishes
    //       i. mainCourse - Calls prepareTask with "Signature Dish"
    //       ii. prepCook - Calls prepareTask with "Washing Dishes" and detaches
    class KitchenStaff {
    public:
        static void prepareTask(const std::string& dish)
        {
            std::osyncstream syncOut(std::cout);
            syncOut << "Chef preparing: " << dish << std::endl;
        }

        static void run()
        {
            std::thread mainCourse(prepareTask, "Signature Dish");
            mainCourse.join();

            std::thread prepCook(prepareTask, "Washing Dishes");
            prepCook.join();
        }

    };

    // Task 2: Basic Mutex
    // Instructions:
    // Implement the BathroomAccess class with the following members & methods:
    //    a. bathroomKey - A mutex to control access to the bathroom
    //    b. visitorCount - An integer to track the number of visitors
    //    c. signIn() - Increments the visitorCount while holding the bathroomKey
    //    d. getVisitorCount() const - Returns the visitorCount
    class BathroomAccess {
    private:
        std::mutex bathroomKey;
        int visitorCount{ 0 };
    public:
        void signIn()
        {
            bathroomKey.lock();
            ++visitorCount;
            bathroomKey.unlock();
        }
        int getVisitorCount() const { return visitorCount; }
    };

    // Task 3: Shared Mutex
    // Instructions:
    // Implement the LibraryBook class with the following members & methods:
    //    a. bookAccess - A shared_mutex to control access to the bookContent
    //    b. bookContent - A string to store the book's content
    //    c. updateContent(const std::string& newContent) - Updates the bookContent
    //       i. Locks the bookAccess with a unique_lock
    //       ii. Sets the bookContent to newContent
    //    d. readContent() const - Returns the bookContent
    //       i. Locks the bookAccess with a shared_lock
    //       ii. Returns the bookContent
    class LibraryBook {
    private:
        mutable std::shared_mutex bookAccess;
        std::string bookContent{ "Initial Content" };
    public:
        void updateContent(const std::string& newContent)
        {
            std::unique_lock<std::shared_mutex> writer(bookAccess);
            bookContent = newContent;
        }
        std::string readContent() const
        {
            std::shared_lock<std::shared_mutex> reader(bookAccess);
            return bookContent;
        }
    };

    // Task 4: Lock Guard
    // Instructions:
    // Implement the HotelRoom class with the following members & methods:
    //    a. roomKey - A mutex to control access to the occupancyCount
    //    b. occupancyCount - An integer to track the number of guests
    //    c. checkIn() - Increments the occupancyCount while holding the roomKey
    class HotelRoom {
    private:
        std::mutex roomKey;
        int occupancyCount{ 0 };
    public:
        void checkIn()
        {
            std::lock_guard<std::mutex> keyCard(roomKey);
            ++occupancyCount;
        }

    };

    // Task 5: Unique Lock
    // Instructions:
    // Implement the DiningTable class with the following members & methods:
    //    a. tableAccess - A mutex to control access to the reservationCount
    //    b. reservationCount - An integer to track the number of reservations
    //    c. manageReservation() - Increments and decrements the reservationCount
    //       i. Uses a unique_lock to lock and unlock the tableAccess
    //    d. deferredReservation() - Increments the reservationCount
    //       i. Uses a unique_lock with std::defer_lock to defer locking the tableAccess
    class DiningTable {
    private:
        std::mutex tableAccess;
        int reservationCount{ 0 };
    public:
        void manageReservation()
        {
            std::unique_lock<std::mutex> reservation(tableAccess);
            ++reservationCount;

            reservation.unlock();
            // handle other hosting duties

            reservation.lock();
            --reservationCount;
        }
        void deferredReservation()
        {
            std::unique_lock<std::mutex> reservation(tableAccess, std::defer_lock);
            reservation.lock();
            ++reservationCount;

        }

    };

    // Task 6: Shared Lock
    // Instructions:
    // Implement the MuseumExhibit class with the following members & methods:
    //    a. exhibitAccess - A shared_mutex to control access to the artifacts
    //    b. artifacts - A vector of integers to store artifact IDs
    //    c. addArtifact(int artifactId) - Adds an artifact to the artifacts vector
    //       i. Locks the exhibitAccess with a unique_lock
    //       ii. Adds the artifactId to the artifacts vector
    //    d. hasArtifact(int artifactId) const - Returns true if the artifactId is in the artifacts vector
    //       i. Locks the exhibitAccess with a shared_lock
    //       ii. Searches the artifacts vector for artifactId
    class MuseumExhibit {
    private:
        mutable std::shared_mutex exhibitAccess;
        std::vector<int> artifacts;
    public:
        void addArtifact(int artifactId)
        {
            std::unique_lock<std::shared_mutex> curator(exhibitAccess);
            artifacts.push_back(artifactId);
        }
        bool hasArtifact(int artifactId) const
        {
            std::shared_lock<std::shared_mutex> visitor(exhibitAccess);
            return std::find(artifacts.begin(), artifacts.end(), artifactId) != artifacts.end();
        }

    };

    // Task 7: Condition Variable
    // Instructions:
    // Implement the KitchenOrderSystem class with the following members & methods:
    //    a. orderLock - A mutex to control access to the orderReady and orderComplete
    //    b. orderReady - A condition_variable to signal when an order is ready
    //    c. orderComplete - A boolean to track if an order is complete
    //    d. orderDetails - A string to store the order details
    //    e. waitForOrder() - Waits for an order to be ready and outputs the orderDetails
    //       i. Uses a unique_lock with orderLock and orderReady.wait to wait for orderComplete
    //       ii. Outputs "Order ready: <orderDetails>"
    //    f. completeOrder(const std::string& order) - Completes an order and notifies the waiter
    //       i. Locks the orderLock with a lock_guard
    //       ii. Sets the orderDetails to order and orderComplete to true
    //       iii. Notifies the waiter with orderReady.notify_one
    //    g. announceStaffMeal(const std::string& meal) - Announces a staff meal and notifies all waiters
    //       i. Locks the orderLock with a lock_guard
    //       ii. Sets the orderDetails to meal and orderComplete to true
    //       iii. Notifies all waiters with orderReady.notify_all
    class KitchenOrderSystem {
    private:
        std::mutex orderLock;
        std::condition_variable orderReady;
        bool orderComplete{ false };
        std::string orderDetails;

    public:
        void waitForOrder()
        {
            std::unique_lock<std::mutex> waiter(orderLock);
            orderReady.wait(waiter, [this]() {return orderComplete; });
            std::osyncstream(std::cout) << "Order ready: " << orderDetails << std::endl;
        }

        void completeOrder(const std::string& order)
        {
            {
                std::lock_guard<std::mutex> kitchen(orderLock);
                orderDetails = order;
                orderComplete = true;
            }
            orderReady.notify_one(); //notify one thread that the shared resource is now avaialable

        }
        void announceStaffMeal(const std::string& meal)
        {
            {
                std::lock_guard<std::mutex> chef(orderLock);
                orderDetails = meal;
                orderComplete = true;
            }
            orderReady.notify_all();
        }


    };

    // Task 8: Atomics
    // Instructions:
    // Implement the TicketSystem class with the following members & methods:
    //    a. availableTickets - An atomic integer to track the number of available tickets
    //    b. salesOpen - An atomic boolean to track if ticket sales are open
    //    c. sellTicket() - Decrements the availableTickets
    //    d. setTicketCount(int count) - Sets the availableTickets to count
    //    e. getAvailableTickets() const - Returns the availableTickets
    //    f. toggleSales() - Toggles the salesOpen boolean
    class TicketSystem {
    private:
        std::atomic<int> availableTickets;
        std::atomic<bool> salesOpen;
    public:
        void sellTicket()
        {
            availableTickets--;
        }
        void setTicketCount(int count)
        {
            availableTickets.store(count);
        }
        int getAvailableTickets() const
        {
            return availableTickets.load();
        }
        void toggleSales()
        {
            salesOpen.store(!salesOpen);
        }
    };

    // Task 9: Future and Promise
    // Instructions:
    // Implement the DeliveryService class with the following members & methods:
    //    a. processDelivery(int orderId) - Returns a future<int> to track the delivery status
    //       i. Creates a promise<int> and gets the future from the promise
    //       ii. Starts a thread to simulate a delivery with a 1-second delay
    //       iii. Sets the promise value to orderId * 2
    //       iv. Returns the future
    //    b. handleDeliveryError() - Handles a delivery error by setting an exception on a promise
    //       i. Creates a promise<int> and gets the future from the promise
    //       ii. Throws a runtime_error
    //       iii. Sets the promise exception with std::current_exception
    class DeliveryService {
    public:
        static std::future<int> processDelivery(int orderId)
        {
            std::promise<int> deliveryPromise;
            std::future<int> tracking = deliveryPromise.get_future();

            std::thread([deliveryPromise = std::move(deliveryPromise), orderId]() mutable {
                std::this_thread::sleep_for(std::chrono::seconds(100));
                deliveryPromise.set_value(orderId * 2);
                }).detach();

            return tracking;
        }
        static void handleDeliveryError()
        {
            std::promise<int> deliveryPromise;
            std::future<int> tracking = deliveryPromise.get_future();

            try {
                throw std::runtime_error("Delivery failed");
            }
            catch (...)
            {
                deliveryPromise.set_exception(std::current_exception());
            }

        }

    };
};

struct Assignment6a {
   static void Execute() {

      // 1. Kitchen Staff Demo
      {
         std::osyncstream syncOut(std::cout);
         syncOut << "\n1. Kitchen Staff (Basic Thread Management):" << std::endl;
         syncOut << "----------------------------------------" << std::endl;
      }
      Tasks::KitchenStaff::run();
      std::this_thread::sleep_for(std::chrono::milliseconds(100));

      // 2. Bathroom Access Demo
      {
         std::osyncstream syncOut(std::cout);
         syncOut << "\n2. Bathroom Access (Basic Mutex):" << std::endl;
         syncOut << "--------------------------------" << std::endl;
      }
      Tasks::BathroomAccess bathroom;
      std::vector<std::thread> visitors;
      for (int i = 0; i < 5; ++i) {
         visitors.emplace_back([&bathroom, i]() {
            bathroom.signIn();
            std::osyncstream(std::cout) << "Visitor " << i << " used the bathroom" << std::endl;
            });
      }
      for (auto& v : visitors) v.join();
      {
         std::osyncstream syncOut(std::cout);
         syncOut << "Total bathroom visits: " << bathroom.getVisitorCount() << std::endl;
      }

      // 3. Library Book Demo
      {
         std::osyncstream syncOut(std::cout);
         syncOut << "\n5. Library Book (Shared Mutex):" << std::endl;
         syncOut << "-------------------------------" << std::endl;
      }
      Tasks::LibraryBook book;
      std::vector<std::thread> readers;
      for (int i = 0; i < 3; ++i) {
         readers.emplace_back([&book, i]() {
            std::osyncstream(std::cout) << "Reader " << i << " reads: " << book.readContent() << std::endl;
            });
      }
      std::thread writer([&book]() {
         book.updateContent("Updated Content");
         std::osyncstream(std::cout) << "Writer updated the book" << std::endl;
         });
      writer.join();
      for (auto& r : readers) r.join();

      // 4. Hotel Room Demo
      {
         std::osyncstream syncOut(std::cout);
         syncOut << "\n6. Hotel Room (Lock Guard):" << std::endl;
         syncOut << "----------------------------" << std::endl;
      }
      Tasks::HotelRoom room;
      std::vector<std::thread> guests;
      for (int i = 0; i < 3; ++i) {
         guests.emplace_back([&room, i]() {
            room.checkIn();
            std::osyncstream(std::cout) << "Guest " << i << " checked in" << std::endl;
            });
      }
      for (auto& g : guests) g.join();

      // 5. Restaurant Table Demo
      {
         std::osyncstream syncOut(std::cout);
         syncOut << "\n7. Restaurant Table (Unique Lock):" << std::endl;
         syncOut << "-----------------------------------" << std::endl;
      }
      Tasks::DiningTable table;
      std::thread host([&table]() {
         table.manageReservation();
         std::osyncstream(std::cout) << "Host managed table reservation" << std::endl;
         });
      host.join();

      // 6. Museum Exhibit Demo
      {
         std::osyncstream syncOut(std::cout);
         syncOut << "\n8. Museum Exhibit (Shared Lock):" << std::endl;
         syncOut << "---------------------------------" << std::endl;
      }
      Tasks::MuseumExhibit museum;
      std::thread curator([&museum]() {
         museum.addArtifact(1);
         std::osyncstream(std::cout) << "Curator added artifact 1" << std::endl;
         });
      std::vector<std::thread> museumVisitors;
      for (int i = 0; i < 3; ++i) {
         museumVisitors.emplace_back([&museum, i]() {
            if (museum.hasArtifact(1)) {
               std::osyncstream(std::cout) << "Visitor " << i << " viewed artifact 1" << std::endl;
            }
            });
      }
      curator.join();
      for (auto& mv : museumVisitors) mv.join();

      // 7. Kitchen Order System Demo
      {
         std::osyncstream syncOut(std::cout);
         syncOut << "\n10. Kitchen Order System (Condition Variable):" << std::endl;
         syncOut << "--------------------------------------------" << std::endl;
      }
      Tasks::KitchenOrderSystem kitchen;
      std::thread waiter([&kitchen]() {
         {
            std::osyncstream(std::cout) << "Waiter waiting for order..." << std::endl;
         }
         kitchen.waitForOrder();
         });
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      kitchen.completeOrder("Steak, medium rare");
      waiter.join();

      // 8. Ticket System Demo
      {
         std::osyncstream syncOut(std::cout);
         syncOut << "\n11. Ticket System (Atomics):" << std::endl;
         syncOut << "-----------------------------" << std::endl;
      }
      Tasks::TicketSystem tickets;
      tickets.setTicketCount(100);
      std::vector<std::thread> agents;
      for (int i = 0; i < 5; ++i) {
         agents.emplace_back([&tickets, i]() {
            tickets.sellTicket();
            std::osyncstream(std::cout) << "Agent " << i << " sold a ticket. Remaining: "
               << tickets.getAvailableTickets() << std::endl;
            });
      }
      for (auto& a : agents) a.join();

      // 9. Delivery Service Demo
      {
         std::osyncstream syncOut(std::cout);
         syncOut << "\n12. Delivery Service (Future/Promise):" << std::endl;
         syncOut << "--------------------------------------" << std::endl;
         syncOut << "Waiting for delivery..." << std::endl;
      }
      auto orderTracking = Tasks::DeliveryService::processDelivery(42);
      int deliveryResult = orderTracking.get();
      {
         std::osyncstream syncOut(std::cout);
         syncOut << "Delivery completed with tracking number: " << deliveryResult << std::endl;
      }

      try {
         Tasks::DeliveryService::handleDeliveryError();
      }
      catch (const std::exception& e) {
         std::osyncstream(std::cout) << "Delivery error handled: " << e.what() << std::endl;
      }
   }
};