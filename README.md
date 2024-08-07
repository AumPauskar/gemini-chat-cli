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
    - Persistent way (Recommended):
        - Open the `.bashrc` or `.zshrc` file in the home directory.
        - Add the following line at the end of the file:
            ```bash
            export GEMINI_API=<API_KEY>
            ```
5. (Optional) Add an alias
    - Open the `.bashrc` or `.zshrc` file in the home directory.
    - Add the following line at the end of the file:
        ```bash
        alias gem='~/path/to/posix-chat'
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


### Usage

Assuming you have saves the alias in the `.bashrc` or `.zshrc` file, built the program and set the environment variable, you can run the program using the following command:
```bash
gem
```
This will open an interactive chat interface in the CLI.

```bash
gem -q "Hello"
```
This will send the message "Hello" to the chatbot and print the response.

```bash
gem -h
```
This will print the help message.
