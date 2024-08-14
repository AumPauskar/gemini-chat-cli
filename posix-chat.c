#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <jansson.h>
#include <ctype.h>

#define BASE_URL "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash-latest:generateContent?key="

// Structure to hold the response data
struct MemoryStruct {
    char *memory;
    size_t size;
};

// Callback function to write data to memory
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t real_size = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + real_size + 1);
    if (ptr == NULL) {
        printf("Not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, real_size);
    mem->size += real_size;
    mem->memory[mem->size] = 0;

    return real_size;
}

// Function to check if a string is a number
int is_number(const char *str) {
    while (*str) {
        if (!isdigit(*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}

// Function to construct the full URL
char* construct_url(const char *api_key) {
    char *url = malloc(strlen(BASE_URL) + strlen(api_key) + 1);
    if (url == NULL) {
        printf("Not enough memory (malloc returned NULL)\n");
        exit(1);
    }
    snprintf(url, strlen(BASE_URL) + strlen(api_key) + 1, "%s%s", BASE_URL, api_key);
    return url;
}

// Function to send a message to the API
void send_message(const char *prompt, const char *api_key) {
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;

    struct MemoryStruct chunk;
    chunk.memory = malloc(1); // Initial allocation
    chunk.size = 0; // No data at this point

    // Initialize CURL
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if(curl) {
        char *escaped_prompt = curl_easy_escape(curl, prompt, 0);
        char data[4096];
        snprintf(data, sizeof(data), "{\"contents\":[{\"parts\":[{\"text\":\"%s\"}]}]}", escaped_prompt);
        curl_free(escaped_prompt);

        // Construct the full URL
        char *url = construct_url(api_key);

        // Set URL and other options
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Send all data to this function
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

        // We pass our 'chunk' struct to the callback function
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        // Perform the request, res will get the return code
        res = curl_easy_perform(curl);

        // Check for errors
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            // Now parse the JSON response
            json_error_t error;
            json_t *root = json_loads(chunk.memory, 0, &error);
            if(root) {
                json_t *candidates = json_object_get(root, "candidates");
                if(json_is_array(candidates)) {
                    json_t *first_candidate = json_array_get(candidates, 0);
                    json_t *content = json_object_get(first_candidate, "content");
                    json_t *parts = json_object_get(content, "parts");
                    if(json_is_array(parts)) {
                        json_t *first_part = json_array_get(parts, 0);
                        const char *message = json_string_value(json_object_get(first_part, "text"));
                        printf("Gemini: %s\n", message);
                    }
                }
                json_decref(root);
            } else {
                fprintf(stderr, "JSON Error: %s\n", error.text);
            }
        }

        // Cleanup
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        free(chunk.memory);
        free(url);
    }

    curl_global_cleanup();
}

int main(int argc, char *argv[]) {
    const char *GOOGLE_API_KEY = getenv("GEMINI_API");
    if (GOOGLE_API_KEY == NULL) {
        fprintf(stderr, "Environment variable GEMINI_API not set.\n");
        return 1;
    }

    if (argc == 3 && strcmp(argv[1], "-q") == 0) {
        // Single message mode
        send_message(argv[2], GOOGLE_API_KEY);
        return 0;
    }

    if (argc == 4 && strcmp(argv[1], "-q") == 0 && is_number(argv[2])) {
        int number = atoi(argv[2]);
        
        // Allocate memory for the message and constraint
        char message[256];
        char constraint[64];

        // Copy argv[3] into message
        strncpy(message, argv[3], sizeof(message) - 1);
        message[sizeof(message) - 1] = '\0';  // Ensure null termination

        // Format the constraint string
        snprintf(constraint, sizeof(constraint), ". Answer this query in %d words or less", number);

        // Check if there's enough space in the message buffer to concatenate
        if (strlen(message) + strlen(constraint) + 1 > sizeof(message)) {
            fprintf(stderr, "Error: Message too long to concatenate constraint.\n");
            return 1;
        }

        // Append constraint to the message
        strncat(message, constraint, sizeof(message) - strlen(message) - 1);

        // Send the message
        send_message(message, GOOGLE_API_KEY);

        return 0;
    }

    if (argc == 2 && strcmp(argv[1], "-h") == 0) {
        // Help message
        printf("Usage\n----------------------------------------\n");
        printf("1. Interactive mode\t\t\t: gem\n");
        printf("2. Single message mode\t\t\t: gem -q \"prompt\"\n");
        printf("2a. Single message mode with word limit\t: gem -q <word limit> \"prompt\"\n");
        printf("\t\t\t\t Example: gem -q 50 \"prompt\"\n\n");
        printf("3. Help\t\t\t\t\t: gem -h\n");
        printf("\n- ->exit<- keyword may be used to quit interactive chat\n");
        printf("- The gem command is interchangeable with %s\n", argv[0]);
        return 0;
    }

    char user_input[1024];

    while(1) {
        printf("You: ");
        if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
            break;
        }

        // Remove newline character from input
        size_t len = strlen(user_input);
        if (len > 0 && user_input[len - 1] == '\n') {
            user_input[len - 1] = '\0';
        }

        // Exit the loop if the user types "exit"
        if (strcmp(user_input, "exit") == 0) {
            printf("Exiting...\n");
            break;
        }

        // Send the user input to the API and get the response
        send_message(user_input, GOOGLE_API_KEY);
    }

    return 0;
}
