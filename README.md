# Project Title
Clerkburk (CCP-Task-Manager)

## Future Plans
 * Implement AI task management
   * Find a way to implement an AI that can remeber parameters and inputs by the user
    * Implement a very simple chat bot which helps the AI to get information like for example the AI types on its on in the console: "/get task" and the AI will read the program's output and with that input it can work on its own.
    * Regularly ask the AI if it wants to do something.
 * Fix bug in which the program doesnt stop asking for input when interrupted early by a thread
 * Fill in Requirements in README.md

## Requirements
```json
In VSC @workspace/.vscode/tasks.json...
  "type": "cppbuild",
  "label": "C/C++: g++-13 Aktive Datei kompilieren",
  "command": "/usr/bin/g++13",
  "args": [
    "-fdiagnostics-color=always",
    "-g",
    "-std=c++2b",
    "-pthread",
    "-O3",
    "${file}",
    "-o",
    "${fileDirname}/${fileBasenameNoExtension}"
  ],
```


* Ubuntu (Github Codespaces):
  * sudo apt-get update
  * sudo apt install build-essential
  * sudo add-apt-repository ppa:ubuntu-toolchain-r/test
  * sudo apt-get install g++-13

* Windows 
 * Not sure yet

* MacOS
 * Not sure yet


## Notes/Archive of Ideas to be copy-pasted
int main() {
    system("manage-bde -unlock D: -pw");
}



// Configurations
constexpr unsigned int HIRING_FOLKS = 1; // std::thread::hardware_concurrency(); // Default thread count

// Dev note to ThreadParty: Special thanks to Github-Copilot. I literally have no idea whats going on here
class ThreadParty { // Depedancies: itself
  // A class to handle multi-threading
  private:
    std::vector<std::thread> workers; // Vector of worker threads
    std::queue<std::function<void()>> tasks; // Tasks to be done, now using packaged_task
    std::mutex tasksMutex; // Mutex to lock the tasks
    std::condition_variable cv; // Condition variable to notify the threads
    bool terminate = false; // Flag to indicate whether the threads should terminate

  public:
    explicit ThreadParty() {
      // Create the worker threads
      
      for (size_t i = 0; i < HIRING_FOLKS; i++) {
        workers.emplace_back([this]() {
            while (true) {
              std::function<void()> task;

              {
                // Wait for a task to be added to the deque
                std::unique_lock<std::mutex> lock(tasksMutex);
                this->cv.wait(lock, [this] { return this->terminate or !this->tasks.empty(); });

                // If the thread should terminate and there are no tasks, return
                if (this->terminate && this->tasks.empty()) { return; }

                // Get the first task and remove it from the deque
                task = std::move(this->tasks.front());
                this->tasks.pop();
              }

              // Execute the task
              try{task();} catch (...) {std::cout << "An error occured in the thread pool" << std::endl;}
            }
            });
        }
    }



    template<typename F, typename... Args>
    auto assign(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
      // Assign a task to the thread pool and return a future object. If you want to run a task without a return value, use the << operator instead.
      // Example: threadpool.assign(sum, 1, 2); -> None
      // Example 2: std::future<int> result = threadpool.assign(sum, 1, 2); -> 3

      using return_type = typename std::result_of<F(Args...)>::type;

      auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
      );

      std::future<return_type> result = task->get_future();

      {
        std::lock_guard<std::mutex> lock(tasksMutex);

        if (terminate) {
          throw std::runtime_error("Sorry, lads and ladies. The party is over.");
        }

        tasks.emplace([task]() { (*task)(); });
      }
      cv.notify_one();

      return result;
    }



    ThreadParty& operator << (std::function<void()> func) {
      // Simply run an void function with no args. Better be careful with this one. it doesnt handle exceptions very well
      // Example: threadpool << greet << greetTwo; -> "Hello World! Hello World, again!"

      {
        std::lock_guard<std::mutex> lock(tasksMutex);

        if (terminate) {
          throw std::runtime_error("Sorry, lads and ladies. The party is over.");
        }

        tasks.emplace(func);
      }
      cv.notify_one();

      return *this;
    }




    void stop() {
      // Set the terminate flag, notify all threads, and join all threads
      
      try {
        {
          std::lock_guard<std::mutex> lock(tasksMutex);
          terminate = true;
        }
        
        cv.notify_all();

        for (std::thread& worker : workers) {
          if (worker.joinable() and worker.get_id() != std::this_thread::get_id()) { worker.join(); }
        }
      } catch (...) {std::cout << "An error occured while stopping the thread pool\n";}
    }
    ~ThreadParty() {stop();}
};