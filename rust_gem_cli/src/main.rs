use reqwest::Client;
use serde_json::json;
use dotenv::dotenv;
use std::env;
use std::io::{self, Write};

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    dotenv().ok(); // Load environment variables from .env file

    let api_key = env::var("GEM_API").expect("GEMINI_API_KEY must be set");

    // Prompt the user for input
    print!("Enter your prompt: ");
    io::stdout().flush()?; // Ensure the prompt is displayed before reading input

    let mut user_prompt = String::new();
    io::stdin().read_line(&mut user_prompt)?;
    let user_prompt = user_prompt.trim(); // Remove any trailing newline characters

    let client = Client::new();
    let url = format!("https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash:generateContent?key={}", api_key);

    let request_body = json!({
        "contents": [{
            "parts": [
                {"text": user_prompt}
            ]
        }]
    });

    let response = client.post(&url)
        .header("Content-Type", "application/json")
        .json(&request_body)
        .send()
        .await?;

    let response_text = response.text().await?;
    println!("Response: {}", response_text);

    Ok(())
}