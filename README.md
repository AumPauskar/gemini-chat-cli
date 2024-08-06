# README

## Building the program

### Installing prerequisites

- Ubuntu
    ```bash
    sudo apt update 
    sudo apt install libcurl4-openssl-dev libjansson-dev
    ```
- Fedora
    ```bash
    sudo dnf install curl-devel jansson-devel
    ```

### Building the program
```bash
gcc posix-chat.c -o posix-chat -lcurl -ljansson
```