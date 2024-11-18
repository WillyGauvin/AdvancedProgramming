#include <iostream>
#include <string>
#include <deque>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>
#include <syncstream>
#include <memory>
#include <unordered_map>

// Enum for different social media platforms
enum class Platform {
    FACEBOOK,
    TWITTER,
    INSTAGRAM,
    LINKEDIN,
    SNAPCHAT,
    STACKOVERFLOW
};

// Enum for post status
enum class PostStatus {
    PENDING,
    POSTED,
    FAILED
};

// Class representing a social media post
class Post
{
private:
    mutable std::mutex postMutex;
    std::string content;
    Platform platform;
    PostStatus status;

public:
    Post(const std::string& content, Platform platform)
        : content(content), platform(platform), status(PostStatus::PENDING) {}

    void updateStatus(PostStatus newStatus)
    {
        postMutex.lock();
        status = newStatus;
        postMutex.unlock();
    }

    PostStatus getStatus() const { return status; }

    Platform getPlatform() const { return platform; }
    std::string getContent() const { return content; }

};

// Thread-safe queue for managing posts
class PostQueue
{
private:
    std::mutex queueMutex;
    std::condition_variable queueCV;
    std::deque<std::shared_ptr<Post>> posts;
    bool shutDown{ false };
public:
    void addPost(std::shared_ptr<Post> post)
    {
        queueMutex.lock();
        posts.push_front(post); // Proper transfer of shared ptr?
        queueMutex.unlock();
        queueCV.notify_all();

    }

    std::shared_ptr<Post> getNextPost()
    {
        std::lock_guard<std::mutex> sharepost(queueMutex);

        std::shared_ptr<Post> post = posts[posts.size()-1];
        posts.pop_back();
        return post;
    }

    void shutdown()
    {
        {
            std::lock_guard<std::mutex> sharepost(queueMutex);
            shutDown = true;
        }
        queueCV.notify_all();
    }

    bool isEmpty()
    {
        return posts.empty();
    }

};

// Class to manage social media platform operations
class PlatformManager
{
private:
    std::atomic<bool> isActive{ true };
    Platform platformType;

public:
    PlatformManager(Platform type)
        :platformType(type) {}

    void processPost(std::shared_ptr<Post> post)
    {
        std::osyncstream syncOut(std::cout);

        if (!isActive) //Don't post if we're in downtime
        {
            post->updateStatus(PostStatus::FAILED);
            syncOut << "Post failed: Server is in downtime" << std::endl;
            return;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));

        if (post->getPlatform() == platformType) //Only process if the post is on our platform
        {
            post->updateStatus(PostStatus::POSTED);
            syncOut << "Platform: " << getPlatformName() << " Posted: " << post->getContent() << std::endl;
        }
        else
        {
            post->updateStatus(PostStatus::FAILED);
            syncOut << "FAILED: Wrong platform" << std::endl;
        }



    }

    void togglePlatform()
    {
        isActive.store(!isActive.load());
    }

    std::string getPlatformName() const
    {
        switch (platformType)
        {
        case Platform::FACEBOOK:
            return "FACEBOOK";
            break;
        case Platform::INSTAGRAM:
            return "INSTAGRAM";
            break;
        case Platform::LINKEDIN:
            return "LINKEDIN";
            break;
        case Platform::SNAPCHAT:
            return "SNAPCHAT";
            break;
        case Platform::STACKOVERFLOW:
            return "STACKOVERFLOW";
            break;
        case Platform::TWITTER:
            return "TWITTER";
            break;
        }
    }
};

// ThreadPool class
class ThreadPool
{
private:
    std::vector<std::thread> workers;
    PostQueue& postQueue;
    std::unordered_map<Platform, PlatformManager*>& platformManagers;
    std::atomic<bool> stop;

public:
    ThreadPool(size_t numThreads, PostQueue& queue, std::unordered_map<Platform, PlatformManager*>& managers)
        : postQueue(queue),
        platformManagers(managers)
    {
        for (size_t i = 0; i < numThreads; i++)
        {
            workers.emplace_back([this, i]() {
                workerFunction();
                });
        }
    }
    ~ThreadPool()
    {
        for (auto& a : workers) a.join();
    }
    void shutdown()
    {
        stop.store(true);
        postQueue.shutdown();
    }

private:
    void workerFunction()
    {
        while (!postQueue.isEmpty())
        {
            std::shared_ptr<Post> nextPost = postQueue.getNextPost();
            platformManagers[nextPost->getPlatform()]->processPost(nextPost);
        }
    }
};

// Main class to run the assignment
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
