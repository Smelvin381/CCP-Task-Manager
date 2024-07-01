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



#include <iostream> // Mostly for std::string and std::cout
#pragma once // Include guard



namespace cslib { // Namespace for the library
class MessageStyling { // Static, Depedancies: itself (USE_ANSI_ESCAPE_CODE, MSG_OUT_PREFIX, MessageType)
  protected:
    #ifdef _WIN32
      static constexpr bool USE_ANSI_ESCAPE_CODE = false; // Windows 10 doesn't support ANSI escape codes
    #elif __linux__
      static constexpr bool USE_ANSI_ESCAPE_CODE = true; // Linux supports ANSI escape codes
    #elif __APPLE__
      static constexpr bool USE_ANSI_ESCAPE_CODE = true; // MacOS supports ANSI escape codes
    #else
      static constexpr bool USE_ANSI_ESCAPE_CODE = false; // Just to be sure
    #endif



    static constexpr const char* MSG_OUT_PREFIX[11] = { // List of all the console ouput prefixes. Note: I apologize for using magic numbers
      USE_ANSI_ESCAPE_CODE ? "\033[0m" : "", // Reset, 0
      USE_ANSI_ESCAPE_CODE ? "\033[0m ** \033[36m" : " ** ", // Default, 1, Cyan
      USE_ANSI_ESCAPE_CODE ? "\033[0m >> \033[30m" : " >> ", // Info, 2, Black
      USE_ANSI_ESCAPE_CODE ? "\033[0m ΔΔ \033[33m" : " ΔΔ ", // Warning, 3, Yellow
      USE_ANSI_ESCAPE_CODE ? "\033[0m !! \033[31m" : " !! ", // Error, 4, Red
      USE_ANSI_ESCAPE_CODE ? "\033[0m ++ \033[32m" : " ++ ", // Static, 5, Green
      USE_ANSI_ESCAPE_CODE ? "\033[0m :: \033[36m" : " :: ", // Debug, 6, Cyan
      USE_ANSI_ESCAPE_CODE ? "\033[0m ?? \033[34m" : " ?? ", // Question, 7, Blue
      USE_ANSI_ESCAPE_CODE ? "\033[0m -> \033[35m" : " -> ", // Input, 8, Magenta
      USE_ANSI_ESCAPE_CODE ? "\033[0m XX \033[1m\033[31m" : " XX ", // Fatal, 9, Red
      USE_ANSI_ESCAPE_CODE ? "\033c" : ""}; // Reset the console, 10

  public:
    enum class MessageType : unsigned int {
      RESET, // ANSI escape code to reset the console
      DEFAULT, // The default output
      INFO, // Output which describes an occurence: "File saved successfully" or "Good Morning!"
      WARNING, // Output which describes an occurence that might be a possible problem: "File content is empty"
      ERROR, // For straight up telling that something went wrong but its still working: "File not found"
      REPLY, // The way the program gives back information related to the given input: "Here are your notes"
      DEBUG, // Output for debugging: "The value of x is 5"
      QUESTION, // The String that contains the question to which the user gets to answer: "What is your name?"
      INPUT, // Then the user types something into the console and the program reads it: "John"
      FATAL,}; // The program is probably about to crash: "Program closed abruptly. Memory leaks everywhere. Good luck dealing with that."


    // Get the prefix by type
    static constexpr const char* g(const MessageType style) {
      return MSG_OUT_PREFIX[static_cast<unsigned int>(style)];}
};




class MessageBuilding : MessageStyling { // Dynamic, Depedancies: iostream.string, MessageStyling, overloading operators
  // A class to build messages (in a constexpr-ed function) and spill them out.
  public:
    std::string buildupMessage; // The message that is being built up

    
    explicit MessageBuilding(const MessageStyling::MessageType type) : buildupMessage(MessageStyling::g(type)) {} // Set the message type
    ~MessageBuilding() {std::cout << MessageStyling::g(MessageStyling::MessageType::RESET);} // Reset the message type after the object is destroyed



    // Add a message to the buildup message
    MessageBuilding& operator << (const std::string& message) {this->buildupMessage += message; return *this;}
    MessageBuilding& operator << (const char* message) {this->buildupMessage += message; return *this;}
    MessageBuilding& operator << (const char message) {this->buildupMessage += message; return *this;}
    MessageBuilding& operator << (std::ostream& (*func)(std::ostream&)) { // Special thanks to @Co-Pilot
      if (func == static_cast<std::ostream& (*)(std::ostream&)>(std::endl)) { // If the function is std::endl
        this->buildupMessage += "\n    "; // Next line is gonna be attached to the main string
      } return *this;}
  };




class TimeStamp { // Depedancies: itself (overloading)
  // A class to handle the "general" time for example for logs
  private:
    unsigned short hour; // 24
    unsigned short minute; // 60
    unsigned short second; // 60
    unsigned short day; // 28-31
    unsigned short month; // 12
    unsigned short year; // 2000-3000
    std::string dayOfWeek; // Monday-Sunday
    std::string monthName; // January-December
    std::string completeDate; // 31.12.2022

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
      // Example: ts[TimeFormat::HOUR] -> 12

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
      // Example: ts[OtherTimeFormats::COMPLETE_DATE] -> "24.12.2022"

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
                     "." + std::to_string(this->year);}
    
    void print_everything() {
      // Print all the time values
      // Example: 12:34:59 24.12.2022

      std::cout << "gffdg\n";

      std::cout << this->hour << "\n";
      std::cout << this->minute << "\n";
      std::cout << this->second << "\n";
      std::cout << this->day << "\n";
      std::cout << this->month << "\n";
      std::cout << this->year << "\n";
      std::cout << this->dayOfWeek << "\n";
      std::cout << this->monthName << "\n";
      std::cout << this->completeDate << "\n";
    }
};
};