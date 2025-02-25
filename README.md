# CippVCS

This project is a simple recreation of some basic Git functionalities using C++. It aims to help understand the inner workings of Git by implementing core features from scratch.

## Features

- Initialize a new repository
- Add files to the staging area

## Getting Started

### Prerequisites

- C++ compiler (e.g., g++)
- CMake (optional, for building the project)

### Installation

1. Clone the repository:
    ```sh
    git clone https://github.com/Hyphen325/CippVCS
    ```
2. Navigate to the project directory:
    ```sh
    
    ```
3.  Execute the project binary with commands
    ```sh
    cipp init
    cipp add
    ```

## Usage

1. Initialize a new repository:
    ```sh
    ./cipp init
    ```
2. Add files to the staging area:
    ```sh
    ./cipp add <filename>
    ```
3. Commit changes:
    ```sh
    ./cipp commit -m "Your commit message"
    ```
4. View commit history:
    ```sh
    ./cipp log
    ```
5. Checkout to a previous commit:
    ```sh
    ./cipp checkout <commit_hash>
    ```


## License

This project is licensed under the MIT License. 

## Acknowledgements

- Inspired by the original Git project
- Thanks to all contributors

