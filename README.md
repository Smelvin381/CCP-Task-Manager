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





