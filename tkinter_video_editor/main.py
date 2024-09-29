import tkinter as tk
from tkinter import filedialog, messagebox
from get_gem_response import get_gem_response as askgem
import os

# Function to handle file selection
def select_file():
    global file_path
    file_path = filedialog.askopenfilename()
    if file_path:
        messagebox.showinfo("Selected File", f"File selected: {file_path}")
    else:
        file_path = None

# Function to handle the submit button
def submit_text():
    global user_text
    user_text = text_box.get("1.0", tk.END).strip()  # Get text from text box
    user_text = askgem(f"{user_text} input file is stored at {file_path}")
    if user_text:
        messagebox.showinfo("Submitted Text", f"Text submitted: {user_text}")
        edit_video()
    else:
        messagebox.showwarning("Warning", "The text box is empty!")

def edit_video():
    # ffmpeg command to edit the video
    ffmpeg_command = user_text
    # Run the command
    os.system(ffmpeg_command)

# Create the main window
root = tk.Tk()
root.title("File Selector and Text Entry")
root.geometry("600x200")

# Create a frame to organize widgets
frame = tk.Frame(root)
frame.pack(padx=10, pady=10, fill=tk.BOTH, expand=True)

# Button to select a file (left)
select_button = tk.Button(frame, text="Select File", command=select_file)
select_button.pack(side=tk.LEFT, padx=5, pady=5)

# Text box in the center
text_box = tk.Text(frame, height=10, width=30)
text_box.pack(side=tk.LEFT, padx=5, pady=5, fill=tk.BOTH, expand=True)

# Submit button (right)
submit_button = tk.Button(frame, text="Submit", command=submit_text)
submit_button.pack(side=tk.RIGHT, padx=5, pady=5)

# Start the Tkinter main loop
root.mainloop()
