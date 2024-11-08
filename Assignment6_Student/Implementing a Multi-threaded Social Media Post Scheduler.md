# Assignment 6b: Implementing a Multi-threaded Social Media Post Scheduler with ThreadPool

## Background

In today's digital landscape, social media managers often need to schedule and manage multiple posts across different platforms simultaneously. This assignment will deepen your understanding of threading concepts by implementing a social media post scheduling system. Think of each social media platform as a separate entity managed by the system, each post as a shared resource, and potential scheduling conflicts as race conditions.

## Objective

Create a C++ application that simulates a social media post scheduling system, focusing on thread synchronization, resource sharing, and concurrent operations. This system will demonstrate real-world applications of mutexes, condition variables, and thread management using a thread pool.

## Core Components to Implement

### 1. **`Post` Class**

Represents a social media post.

#### **Members:**

- **Private:**
  - `mutable std::mutex postMutex;`  
    *Mutex for thread-safe operations on the post.*
  - `std::string content;`  
    *The content of the post.*
  - `Platform platform;`  
    *The target platform for the post (e.g., Facebook, Twitter).*
  - `PostStatus status;`  
    *The current status of the post (`PENDING`, `POSTED`, `FAILED`).*

#### **Methods:**

- **Public:**
  - `Post(const std::string& content, Platform platform);`  
    *Constructor to initialize the post with content and target platform.*
  - `void updateStatus(PostStatus newStatus);`  
    *Updates the status of the post in a thread-safe manner.*
  - `PostStatus getStatus() const;`  
    *Retrieves the current status of the post in a thread-safe manner.*
  - `Platform getPlatform() const;`  
    *Returns the target platform of the post.*
  - `std::string getContent() const;`  
    *Returns the content of the post.*

### 2. **`PostQueue` Class**

A thread-safe queue for managing posts to be processed.

#### **Members:**

- **Private:**
  - `std::mutex queueMutex;`  
    *Mutex to protect access to the queue.*
  - `std::condition_variable queueCV;`  
    *Condition variable to notify worker threads of new posts.*
  - `std::deque<std::shared_ptr<Post>> posts;`  
    *Deque container holding the posts.*
  - `bool shutDown{ false };`  
    *Flag to indicate if the queue is shutting down.*

#### **Methods:**

- **Public:**
  - `void addPost(std::shared_ptr<Post> post);`  
    *Adds a new post to the queue and notifies worker threads.*
  - `std::shared_ptr<Post> getNextPost();`  
    *Retrieves the next post from the queue in a thread-safe manner.*
  - `void shutdown();`  
    *Signals all worker threads to shut down after processing remaining posts.*
  - `bool isEmpty();`  
    *Checks if the queue is empty.*

### 3. **`PlatformManager` Class**

Manages the processing of posts for a specific social media platform.

#### **Members:**

- **Private:**
  - `std::atomic<bool> isActive{ true };`  
    *Atomic flag indicating if the platform manager is active.*
  - `Platform platformType;`  
    *The specific platform this manager handles.*

#### **Methods:**

- **Public:**
  - `PlatformManager(Platform type);`  
    *Constructor to initialize the manager with a specific platform.*
  - `void processPost(std::shared_ptr<Post> post);`  
    *Processes a single post, simulating posting to the platform.*
  - `void togglePlatform();`  
    *Toggles the active status of the platform manager (e.g., to simulate downtime).*
  - `std::string getPlatformName() const;`  
    *Returns the name of the platform as a string.*

### 4. **`ThreadPool` Class**

Manages a pool of worker threads to process posts concurrently.

#### **Members:**

- **Private:**
  - `std::vector<std::thread> workers;`  
    *Vector of worker threads.*
  - `PostQueue& postQueue;`  
    *Reference to the shared post queue.*
  - `std::unordered_map<Platform, PlatformManager*>& platformManagers;`  
    *Reference to a map of `Platform` to `PlatformManager` pointers.*
  - `std::atomic<bool> stop;`  
    *Flag to indicate shutdown.*

#### **Methods:**

- **Public:**
  - `ThreadPool(size_t numThreads, PostQueue& queue, std::unordered_map<Platform, PlatformManager*>& managers);`  
    *Constructor to initialize the thread pool with a specified number of threads.*
  - `~ThreadPool();`  
    *Destructor to clean up the threads.*
  - `void shutdown();`  
    *Signals the thread pool to stop processing and shuts down the queue.*

- **Private:**
  - `void workerFunction();`  
    *Function executed by each worker thread to process posts from the queue.*

## Requirements

1. **Thread Safety**
   - Protect shared resources using appropriate mutex locks.
   - Use condition variables for queue management.
   - Implement atomic operations where appropriate.

2. **Post Management**
   - Allow scheduling posts for different platforms.
   - Handle post status updates in a thread-safe manner.
   - Ensure that posts are processed by the correct `PlatformManager`.

3. **Platform Simulation**
   - Simulate posting delays using `std::this_thread::sleep_for`.
   - Handle platform availability changes (e.g., simulate a platform going offline).
   - Implement basic error handling for failed posts.

4. **Concurrency**
   - Use a thread pool to simulate concurrent processing of posts.
   - Ensure that all posts are eventually processed or marked as failed upon shutdown.

## Implementation Tasks

1. **Create the Basic Class Structure**
   - Define all classes (`Post`, `PostQueue`, `PlatformManager`, `ThreadPool`, `Assignment6b`) with their members and methods as specified.

2. **Implement Thread-Safe Post Management**
   - Use mutexes within the `Post` class to protect status updates.
   - Ensure that `PostQueue` operations are thread-safe using mutexes and condition variables.

3. **Create Platform Simulation Logic**
   - In `PlatformManager`, implement the logic to process posts only for its designated platform.
   - Use `std::this_thread::sleep_for` to simulate network delays or processing time.
   - Implement the `togglePlatform` method to simulate platform downtime.

4. **Implement the ThreadPool**
   - In the `ThreadPool` class, implement a pool of worker threads that continuously fetch and process posts from the `PostQueue`.
   - Each worker thread should retrieve posts from the queue and use the appropriate `PlatformManager` to process the post.
   - Ensure that the thread pool handles shutdown gracefully.

5. **Implement Error Handling**
   - Update post statuses to `FAILED` if a platform manager is inactive.
   - Handle cases where the queue is shut down while there are still posts pending.

6. **Add Scheduling and Timing Mechanisms**
   - *(Optional)* Extend the `Post` class to include scheduling times.
   - Ensure posts are processed in order of scheduling time if implemented.

## Example Usage

```cpp
class Assignment6b {
public:
   static void Execute() {
      // Create platform managers
      PlatformManager facebook(Platform::FACEBOOK);
      PlatformManager twitter(Platform::TWITTER);
      PlatformManager instagram(Platform::INSTAGRAM);
      PlatformManager linkedin(Platform::LINKEDIN);
      PlatformManager snapchat(Platform::SNAPCHAT);
      PlatformManager stackoverflow(Platform::STACKOVERFLOW);

      // Map from Platform to PlatformManager*
      std::unordered_map<Platform, PlatformManager*> platformManagers;
      platformManagers[Platform::FACEBOOK] = &facebook;
      platformManagers[Platform::TWITTER] = &twitter;
      platformManagers[Platform::INSTAGRAM] = &instagram;
      platformManagers[Platform::LINKEDIN] = &linkedin;
      platformManagers[Platform::SNAPCHAT] = &snapchat;
      platformManagers[Platform::STACKOVERFLOW] = &stackoverflow;

      // Create post queue
      PostQueue queue;

      // Create some sample posts
      queue.addPost(std::make_shared<Post>("Hello Facebook!", Platform::FACEBOOK));
      queue.addPost(std::make_shared<Post>("Another Facebook post", Platform::FACEBOOK));
      queue.addPost(std::make_shared<Post>("Yet another Facebook post", Platform::FACEBOOK));

      queue.addPost(std::make_shared<Post>("Tweet tweet!", Platform::TWITTER));
      queue.addPost(std::make_shared<Post>("Another tweet", Platform::TWITTER));
      queue.addPost(std::make_shared<Post>("Yet another tweet", Platform::TWITTER));

      queue.addPost(std::make_shared<Post>("Instagram photo time!", Platform::INSTAGRAM));
      queue.addPost(std::make_shared<Post>("Another Instagram post", Platform::INSTAGRAM));
      queue.addPost(std::make_shared<Post>("Yet another Instagram post", Platform::INSTAGRAM));

      queue.addPost(std::make_shared<Post>("LinkedIn post", Platform::LINKEDIN));
      queue.addPost(std::make_shared<Post>("Another LinkedIn post", Platform::LINKEDIN));
      queue.addPost(std::make_shared<Post>("Yet another LinkedIn post", Platform::LINKEDIN));

      queue.addPost(std::make_shared<Post>("Snapchat story", Platform::SNAPCHAT));
      queue.addPost(std::make_shared<Post>("Another Snapchat story", Platform::SNAPCHAT));
      queue.addPost(std::make_shared<Post>("Yet another Snapchat story", Platform::SNAPCHAT));

      queue.addPost(std::make_shared<Post>("StackOverflow question", Platform::STACKOVERFLOW));
      queue.addPost(std::make_shared<Post>("Another StackOverflow question", Platform::STACKOVERFLOW));
      queue.addPost(std::make_shared<Post>("Yet another StackOverflow question", Platform::STACKOVERFLOW));

      // Create ThreadPool
      size_t numThreads = std::thread::hardware_concurrency();
      if (numThreads == 0) numThreads = 4; // Default to 4 if unable to detect
      ThreadPool pool(numThreads, queue, platformManagers);

      // Wait for some time
      std::this_thread::sleep_for(std::chrono::seconds(2));

      // Shutdown the queue and the thread pool
      pool.shutdown();
      // The pool destructor will join the threads
   }
};
```

## Process Flow

```plaintext
Main Thread:
-------------
   |
   v
+--------------------------+
| Create PlatformManagers  |
+--------------------------+
   |
   v
+----------------------------+
| Create platformManagers map |
+----------------------------+
   |
   v
+------------------+
| Create PostQueue |
+------------------+
   |
   v
+------------------------+
| Add sample posts to    |
| the PostQueue          |
+------------------------+
   |
   v
+-----------------------------------------+
| Create ThreadPool (starts worker threads)|
+-----------------------------------------+
   |
   v
+------------------------------+
| Wait for 2 seconds           |
+------------------------------+
   |
   v
+------------------------------+
| Shutdown the ThreadPool      |
| (which also shuts down       |
| the PostQueue)               |
+------------------------------+
   |
   v
+-----------+
| End of    |
| Execute() |
+-----------+

Worker Threads (managed by ThreadPool):
---------------------------------------
[Each worker thread executes the following loop]

Worker Thread N:
   |
   v
+---------------------------+
| While not stopped:        |
|                           |
| - Get next post from      |
|   PostQueue               |
|   (blocks if queue is     |
|    empty and not shutdown)|
|                           |
| - If post is nullptr:     |
|     Exit loop             |
|                           |
| - Determine PlatformManager|
|   based on post's platform|
|                           |
| - PlatformManager         |
|   processes the post      |
+---------------------------+
   |
   v
[Loop repeats until shutdown]

PlatformManager::processPost():
-------------------------------
   |
   v
+------------------------------+
| If PlatformManager is active:|
|   - Simulate delay           |
|     (sleep for 500 ms)       |
|   - Update post status to    |
|     POSTED                   |
|   - Output: "Posted to       |
|     [Platform]: [Content]"   |
| Else:                        |
|   - Update post status to    |
|     FAILED                   |
+------------------------------+

```

## Learning Objectives

By completing this assignment, you will:

- Understand practical applications of thread synchronization.
- Learn to manage shared resources safely.
- Gain experience with condition variables and mutexes.
- Practice implementing thread-safe queues.
- Learn to handle concurrent operations in a real-world scenario.
- Understand how to implement and manage a thread pool.