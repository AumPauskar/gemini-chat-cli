#!/bin/bash

# Define the Google API key (you should store this securely)
GOOGLE_API_KEY=$(printenv GEMINI_API)

# Set the endpoint URL
ENDPOINT="https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash-latest:generateContent?key=$GOOGLE_API_KEY"

# Function to send a message to the API
send_message() {
  local prompt="$1"

  # Use curl to send the request to the Google API
  response=$(curl -s -X POST "$ENDPOINT" \
    -H "Content-Type: application/json" \
    -d '{
      "contents": [{
        "parts": [{
          "text": "'"${prompt}"'"
        }]
      }]
    }')

  # Print the entire response for debugging
#   echo "Debug: Full API response: $response"

  # Extract and print the response (modify according to Google API response format)
  message=$(echo "$response" | jq -r '.candidates[0].content.parts[0].text')
  echo "$message"
}

# Main loop
while true; do
  # Read user input
  echo -n "You: "
  read -r user_input

  # Exit if the user types 'exit'
  if [[ "$user_input" == "exit" ]]; then
    echo "Exiting..."
    break
  fi

  # Send the user input to the Google API and get the response
  echo "Gemini: $(send_message "$user_input")"
done