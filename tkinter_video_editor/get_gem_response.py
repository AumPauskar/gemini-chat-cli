from dotenv import load_dotenv
import os
import requests

# getting api key from .env file
load_dotenv()
GEM_API = os.getenv("GEM_API")

# pre_prompt
PRE_PROMPT = """Consider the following request and give a response in the form of a one line FFMPEG command.
Example: Request: Reduce the video size by 50%
Answer: ffmpeg -i input.mp4 -vf scale=iw/2:-1 -c:v libx264 -c:a copy output.mp4"""

# Define the URL and headers
url = f'https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash-latest:generateContent?key={GEM_API}'
headers = {
    'Content-Type': 'application/json'
}

def get_gem_response(command):
    data = {
        "contents": [
            {
                "parts": [
                    {
                        "text": f"{PRE_PROMPT}{command}"
                    }
                ]
            }
        ]
    }

    # Make the POST request
    response = requests.post(url, headers=headers, json=data)

    response_json = response.json()
    text_output = response_json['candidates'][0]['content']['parts'][0]['text']
    return text_output