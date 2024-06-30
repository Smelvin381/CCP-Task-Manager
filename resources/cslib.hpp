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

#include <iostream> // Input and output (Includes <string>)
#include <array> // Arrays


namespace cslib { // Namespace for the library
class MessageStyling { // Depedancies: iostream.string_view, itself (MSG_OUT_PREFIX, MessageType)
  protected:
    #ifdef _WIN32
      static constexpr bool USE_ANSI_ESCAPE_CODE = false; // Windows doesn't support ANSI escape codes
    #elif __linux__
      static constexpr bool USE_ANSI_ESCAPE_CODE = true; // Linux supports ANSI escape codes
    #elif __APPLE__
      static constexpr bool USE_ANSI_ESCAPE_CODE = true; // MacOS supports ANSI escape codes
    #else
      static constexpr bool USE_ANSI_ESCAPE_CODE = false; // Just to be sure
    #endif

    static constexpr std::string_view MSG_OUT_PREFIX[11] = { // List of all the console ouput prefixes. Note: I apologize for using magic numbers
      USE_ANSI_ESCAPE_CODE ? "\033[0m" : "", // Reset, 0
      USE_ANSI_ESCAPE_CODE ? "\033[0m ** \033[36m" : " ** ", // Default, 1, Cyan
      USE_ANSI_ESCAPE_CODE ? "\033[0m >> \033[30m" : " >> ", // Info, 2, Black
      USE_ANSI_ESCAPE_CODE ? "\033[0m ΔΔ \033[33m" : " ΔΔ ", // Warning, 3, Yellow
      USE_ANSI_ESCAPE_CODE ? "\033[0m !! \033[31m" : " !! ", // Error, 4, Red
      USE_ANSI_ESCAPE_CODE ? "\033[0m ++ \033[32m" : " ++ ", // Static, 5, Green
      USE_ANSI_ESCAPE_CODE ? "\033[0m :: \033[36m" : " :: ", // Debug, 6, Cyan
      USE_ANSI_ESCAPE_CODE ? "\033[0m ?? \033[34m" : " ?? ", // Question, 7, Blue
      USE_ANSI_ESCAPE_CODE ? "\033[0m -> \033[35m" : " -> ", // Input, 8, Magenta
      USE_ANSI_ESCAPE_CODE ? "\033[0m XX \033[1m \033[31m" : " XX ", // Fatal, 9, Red
      USE_ANSI_ESCAPE_CODE ? "\033c" : "" // Reset the console, 10
    };

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
      FATAL, // The program is probably about to crash: "Program closed abruptly. Memory leaks everywhere. Good luck dealing with that."
    };



    // Get the prefix by type
    static constexpr std::string_view g(const MessageType style) {
      return MSG_OUT_PREFIX[static_cast<unsigned int>(style)];}
};




class MessageBuilding { // Depedancies: MessageStyling
  // A class to build messages and spill them out.
  private:
    static constexpr unsigned char MAX_MESSAGE_LENGTH = 0xFF; // The maximum length of a message
    char message[MAX_MESSAGE_LENGTH] = {}; // The message to be built
  
  public:
    constexpr explicit MessageBuilding(const MessageStyling::MessageType messageType) {}



    // Add string into build message
    template <unsigned int SIZE>
    constexpr void a(const std::array<std::string_view, SIZE>& strings) {
      for (const std::string_view string : strings) {

        for (const char& character : string) {
          if (message[MAX_MESSAGE_LENGTH - 1] == '\0') return;
          if (character == '\t') {
        for (unsigned char i = 0; i < 4; i++) {
          if (message[MAX_MESSAGE_LENGTH - 1] == '\0') return;
          for (unsigned char j = 0; j < MAX_MESSAGE_LENGTH; j++) {
            if (message[j] == '\0') {
          message[j] = ' ';
          break;
            }
          }
        }
          } else {
        for (unsigned char i = 0; i < MAX_MESSAGE_LENGTH; i++) {
          if (message[i] == '\0') {
            message[i] = character;
            break;
          }
        }
          }
        }

      }
    }



    // Get the message as a char array
    constexpr char* g() {return message;}
};
}; // End of the namespace cslib
