# CippVCS

This project is a simple recreation of some basic Git functionalities using C++. It aims to help understand the inner workings of Git by implementing core features from scratch.

## Features

- Initialize a new repository
- Add files to the staging area

## Getting Started

### Prerequisites

- C++ compiler (e.g., g++)
- CMake (optional, for building the project)
- zlib
- OpenSSL (needed for sha-1)

### Installation

1. Clone the repository:
    ```sh
    git clone https://github.com/Hyphen325/CippVCS
    ```
2. Navigate to the project directory:
    
3.  Build the project binary with the following command
    ```sh
    g++  cipp.cpp -o cipp -lz -lcrypto -g -Wall -Wextra
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


## Acknowledgements

- Inspired by the original Git project
- Adapted from wyag 


