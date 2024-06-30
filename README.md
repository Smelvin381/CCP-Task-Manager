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
  "args": [
    "-O3", // Optimization level 3 (highest)
    "-std=c++23", // C++23
    "${file}",
    "-o",
    "${fileDirname}/${fileBasenameNoExtension}",
    "-static" // Static linking
  ],
```


* Ubuntu (Github Codespaces):
  * sudo apt-get update
  * sudo apt install build-essential
  * sudo add-apt-repository ppa:ubuntu-toolchain-r/test
  * sudo apt-get install g++-13

* Windows x86_64 (Msys2):
 * Download the newest version of Msys
 * $ pacman -S mingw-w64-ucrt-x86_64-gcc
 * $ pacman -S mingw-w64-ucrt-x86_64-gdb
 * g++ -version should be ±13

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



class JackOfAllTrades { // Depedancies: 
  // An highly specialized class to handle common tasks
  public:
    template <typename T>
    static T get_true_value(const std::string& s) {
      // Convert string to value

      if (std::is_same<T, char>::value) {
        return s.empty() ? '\0' : s[0];
      }


      if (std::is_same<T, bool>::value) {
        if (s == "true" or s == "True" or s == "TRUE" or s == "1") return 1;
        else if (s == "false" or s == "False" or s == "FALSE" or s == "0") return 0;
      }

      std::cout << "Not implemented yet. \n";
      return T();
      }
};




class TimeStamp { // Depedancies: itself (overloading)
  // A class to handle the "general" time for example for logs
  private:
    unsigned short hour;
    unsigned short minute;
    unsigned short second;
    unsigned short day;
    unsigned short month;
    unsigned short year;
    std::string dayOfWeek;
    std::string monthName;
    std::string completeDate;

    static constexpr unsigned short TIME_ZONE = 2; // Time zone (in hours) from UTC. 2+ because i live in Germany
    static constexpr const char* WEEKDAYS[7] = {
      "Sunday",
      "Monday",
      "Tuesday",
      "Wednesday",
      "Thursday",
      "Friday",
      "Saturday"
    };
    static constexpr const char* MONTHS[12] = {
      "January",
      "February",
      "March",
      "April",
      "May",
      "June",
      "July",
      "August",
      "September",
      "October",
      "November",
      "December"
    };

  public:
    enum class TimeFormat {
      HOUR,
      MINUTE,
      SECOND,
      DAY,
      MONTH,
      YEAR,
    };
    enum class OtherTimeFormats {
      COMPLETE_DATE,
      DAY_OF_WEEK,
      MONTH_NAME,
    };



    explicit TimeStamp() {this->update();} // Update the time when the object is created



    const unsigned short& operator [] (const TimeFormat format) const {
      // Get the time by index
      // Example: ts['h'] -> 12

      switch (format) {
        case TimeFormat::HOUR: return this->hour;
        case TimeFormat::MINUTE: return this->minute;
        case TimeFormat::SECOND: return this->second;
        case TimeFormat::DAY: return this->day;
        case TimeFormat::MONTH: return this->month;
        case TimeFormat::YEAR: return this->year;
        default: throw std::invalid_argument("Invalid time format");
      }
    }
    const std::string& operator [] (const OtherTimeFormats format) const {
      // Get the time by index
      // Example: ts['dayOfWeek'] -> "Monday"

      switch (format) {
        case OtherTimeFormats::COMPLETE_DATE: return this->completeDate;
        case OtherTimeFormats::DAY_OF_WEEK: return this->dayOfWeek;
        case OtherTimeFormats::MONTH_NAME: return this->monthName;
        default: throw std::invalid_argument("Invalid time format");
      }
    }



    void update() {
      // Replace all the time values with updated ones
      // Example: 12:34:59 24.12.2022 -> 12:35:00 24.12.2022

      time_t now = time(0);
      struct tm currentTime;

      #ifdef _WIN32
        localtime_s(&currentTime, &now); // For Windows-based systems
      #else
        localtime_r(&now, &currentTime); // For Unix-based systems
      #endif

      this->hour = currentTime.tm_hour + TIME_ZONE;
      this->minute = currentTime.tm_min;
      this->second = currentTime.tm_sec;
      this->day = currentTime.tm_mday;
      this->month = currentTime.tm_mon + 1;
      this->year = currentTime.tm_year + 1900;
      this->dayOfWeek = WEEKDAYS[currentTime.tm_wday];
      this->monthName = MONTHS[currentTime.tm_mon];
      this->completeDate = std::to_string(this->day) +
                     "." + std::to_string(this->month) +
                     "." + std::to_string(this->year);
    }
};









    // Add text
    constexpr void a(std::string_view text) {
      // Add text to the message
      // Example: mb.add_text("Hello, World!") -> "Hello, World!"

      for (const char& c : text) {
        if (this->messageLength < MaxLenght) {
          this->message[this->messageLength++] = c;
        }
      }
    }
    constexpr MessageBuilding& operator << (std::string_view text) {
      // Add text to the message
      // Example: mb << "Hello, World!" -> "Hello, World!"

      for (const char& c : text) {
        if (this->messageLength < MaxLenght) {
          this->message[this->messageLength++] = c;
        }
      }
      return *this;
    }



    // Add character
    constexpr MessageBuilding& operator << (char text) {
      // Add text to the message
      // Example: mb << 'H' -> "H"

      if (text == '\t') { // Break line and add 4 spaces
        if (this->messageLength < MaxLenght) {
          this->message[this->messageLength++] = '\n';
        }
        for (unsigned char i = 0; i < 4; i++) {
          if (this->messageLength < MaxLenght) {
            this->message[this->messageLength++] = ' ';
          }
        }
        return *this;
      }

      if (this->messageLength < MaxLenght) {
        this->message[this->messageLength++] = text;
      }
      return *this;
    }



  private:
    std::string buildupMessage;
  
  public:
    constexpr explicit MessageBuilding(const MessageStyling::MessageType type) : buildupMessage(MessageStyling::g(type)) {} // Set the message type
    constexpr explicit MessageBuilding() : buildupMessage(MessageStyling::g(MessageStyling::MessageType::DEFAULT)) {} // Set the message type



    // Add a message to the buildup message
    template <unsigned int S>
    constexpr void add(const std::array<std::string_view, S> &message) {
      for (const std::string_view &i : message) {this->buildupMessage += i;}
    }

    // Get the string
    constexpr std::string_view g() const {return this->buildupMessage;}