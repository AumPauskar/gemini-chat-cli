# README

## Setting up the environment
1. Go to [Google AI playground](https://aistudio.google.com/app/) and create a new project.
2. Go to the project settings and enable the API.
3. Copy the API key in the clipboard.
4. There are two ways to set the environment variable:
    - Non persistent way:
        ```bash
        export GEMINI_API=<API_KEY>
        ```
    - Persistent way:
        - Open the `.bashrc` or `.zshrc` file in the home directory.
        - Add the following line at the end of the file:
            ```bash
            export GEMINI_API=<API_KEY>
            ```

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