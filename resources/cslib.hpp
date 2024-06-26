// This file is part of ClerkBurkv0.1
//
// ClerkBurk is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// ClerkBurk is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with ClerkBurk. If not, see <https://www.gnu.org/licenses/>.
//
// Contact: @Smelvin381 on GitHub
//
// Functions, classes and constants in this file aren't project specific but created...
// ...for demanding tasks.



#pragma once // Include only once

#include <functional> // Function objects
#include <iostream> // Input and output
#include <ctime> // Time



namespace cslib { // Namespace for the library
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




class MessageStyling { // Depedancies: itself (MSG_OUT_PREFIX, MessageType)
  protected:
    static constexpr bool USE_ANSI_ESCAPE_CODE = true; // Hard coded since the platform is highly likely to support ANSI escape codes
    static constexpr std::array<std::string_view, 11> MSG_OUT_PREFIX = { // List of all the console ouput prefixes. Note: I apologize for using magic numbers
      USE_ANSI_ESCAPE_CODE ? "\033[0m" : "", // Reset, 0
      USE_ANSI_ESCAPE_CODE ? "\033[0m ** \033[36m" : " ** ", // Default, 1, Cyan
      USE_ANSI_ESCAPE_CODE ? "\033[0m >> \033[30m" : " >> ", // Info, 2, Black
      USE_ANSI_ESCAPE_CODE ? "\033[0m ΔΔ \033[33m" : " ΔΔ ", // Warning, 3, Yellow
      USE_ANSI_ESCAPE_CODE ? "\033[0m !! \033[31m" : " !! ", // Error, 4, Red
      USE_ANSI_ESCAPE_CODE ? "\033[0m ++ \033[32m" : " ++ ", // Static, 5, Green
      USE_ANSI_ESCAPE_CODE ? "\033[0m :: \033[36m" : " :: ", // Debug, 6, Cyan
      USE_ANSI_ESCAPE_CODE ? "\033[0m ?? \033[34m" : " ?? ", // Question, 7, Blue
      USE_ANSI_ESCAPE_CODE ? "\033[0m -> \033[35m" : " -> ", // Input, 8, Magenta
      USE_ANSI_ESCAPE_CODE ? "\033[0m XX \033[1m \033[31m" : " FATAL: ", // Fatal, 9, Red
      USE_ANSI_ESCAPE_CODE ? "\033c" : "" // Reset the console, 10
    };

  public:
    enum class MessageType : unsigned int {
      RESET = 0, // ANSI escape code to reset the console
      DEFAULT = 1, // The default output
      INFO = 2, // Output which describes an occurence: "File saved successfully" or "Good Morning!"
      WARNING = 3, // Output which describes an occurence that might be a possible problem: "File content is empty"
      ERROR = 4, // For straight up telling that something went wrong but its still working: "File not found"
      REPLY = 5, // The way the program gives back information related to the given input: "Here are your notes"
      DEBUG = 6, // Output for debugging: "The value of x is 5"
      QUESTION = 7, // The String that contains the question to which the user gets to answer: "What is your name?"
      INPUT = 8, // Then the user types something into the console and the program reads it: "John"
      FATAL = 9, // The program is probably about to crash: "Program closed abruptly. Memory leaks everywhere. Good luck dealing with that."
    };



    static const std::string_view& g(const MessageType style) {
      // Get the prefix by type
      // Example: MessageType::INFO -> "\033[0m >> \033[30m"

      switch (style) {
        case MessageType::RESET: return MSG_OUT_PREFIX[static_cast<unsigned int>(MessageType::RESET)];
        case MessageType::DEFAULT: return MSG_OUT_PREFIX[static_cast<unsigned int>(MessageType::DEFAULT)];
        case MessageType::INFO: return MSG_OUT_PREFIX[static_cast<unsigned int>(MessageType::INFO)];
        case MessageType::WARNING: return MSG_OUT_PREFIX[static_cast<unsigned int>(MessageType::WARNING)];
        case MessageType::ERROR: return MSG_OUT_PREFIX[static_cast<unsigned int>(MessageType::ERROR)];
        case MessageType::REPLY: return MSG_OUT_PREFIX[static_cast<unsigned int>(MessageType::REPLY)];
        case MessageType::DEBUG: return MSG_OUT_PREFIX[static_cast<unsigned int>(MessageType::DEBUG)];
        case MessageType::QUESTION: return MSG_OUT_PREFIX[static_cast<unsigned int>(MessageType::QUESTION)];
        case MessageType::INPUT: return MSG_OUT_PREFIX[static_cast<unsigned int>(MessageType::INPUT)];
        case MessageType::FATAL: return MSG_OUT_PREFIX[static_cast<unsigned int>(MessageType::FATAL)];
        default: return MSG_OUT_PREFIX[static_cast<unsigned int>(MessageType::DEFAULT)];
      }
    }
    
    

    static void reset_styling() {std::cout << MSG_OUT_PREFIX[static_cast<unsigned int>(MessageType::RESET)];} // Reset the console styling



    static std::string ask_question(const std::string& question) {
      // Ask a question and get the answer
      // Example: "What is your name?" -> "John"

      if (!question.empty()) {
        std::cout << MessageStyling::g(MessageStyling::MessageType::QUESTION) <<
        question << MessageStyling::g(MessageStyling::MessageType::INPUT);
      }

      std::string userInput;
      std::getline(std::cin, userInput);
      std::cout << MessageStyling::g(MessageStyling::MessageType::RESET);

      return userInput;
    }
    template <typename T>
    static T ask_question(const std::string& question) {
      try {return JackOfAllTrades::get_true_value<T>(ask_question(question));}
      catch (const std::invalid_argument& e) {
        std::cout << MessageStyling::g(MessageStyling::MessageType::ERROR) << e.what() << "\n";
      } return T();
    }
};




class MessageBuilding { // Depedancies: itself (overloading), MessageStyling
  // A class to build messages and spill them out.
  private:
    std::string buildupMessage;
  
  public:
    explicit MessageBuilding(const MessageStyling::MessageType type) : buildupMessage(MessageStyling::g(type)) {} // Set the message type
    explicit MessageBuilding() : buildupMessage(MessageStyling::g(MessageStyling::MessageType::DEFAULT)) {} // Set the message type
    ~MessageBuilding() {std::cout << MessageStyling::g(MessageStyling::MessageType::RESET);} // Reset the message type after the object is destroyed

    // Add a message to the buildup message
    template <typename T>
    MessageBuilding& operator << (const T& message) {this->buildupMessage += std::to_string(message); return *this;} // Add a message to the buildup message
    MessageBuilding& operator << (const std::string& message) {this->buildupMessage += message; return *this;} // Add a message to the buildup message
    MessageBuilding& operator << (const std::string_view& message) {this->buildupMessage += message; return *this;} // Add a message to the buildup message
    MessageBuilding& operator << (const char* message) {this->buildupMessage += message; return *this;} // Add a message to the buildup message
    MessageBuilding& operator << (const char message) {this->buildupMessage += message; return *this;} // Add a message to the buildup message
    MessageBuilding& operator << (std::ostream& (*func)(std::ostream&)) { // Special thanks to @Co-Pilot
      if (func == static_cast<std::ostream& (*)(std::ostream&)>(std::endl)) { // If the function is std::endl
        this->buildupMessage += "\n    "; // Next line is gonna be attached to the main string
      }
      return *this; // Allow chaining
    }



    // Get the string
    const std::string& womit() const {return this->buildupMessage;}
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
}; // End of the namespace cslib
