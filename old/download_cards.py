import os
import requests

# Base URL for the Deck of Cards API images
BASE_URL = "https://deckofcardsapi.com/static/img/"

# Directory to save the card images
BASE_DIR = "resources"

# Suits and values for a standard 52-card deck
SUITS = {
    "Hearts": "H",
    "Diamonds": "D",
    "Clubs": "C",
    "Spades": "S"
}
VALUES = {
    "A": "A", "2": "2", "3": "3", "4": "4", "5": "5",
    "6": "6", "7": "7", "8": "8", "9": "9", "10": "0",
    "J": "J", "Q": "Q", "K": "K"
}

def download_card_image(suit_name, suit_code, value_name, value_code):
    """Download and save the card image for a given suit and value."""
    # Construct the URL for the card
    url = f"{BASE_URL}{value_code}{suit_code}.png"
    
    # Path to save the image (e.g., resources/Hearts/6.png)
    save_dir = os.path.join(BASE_DIR, suit_name)
    os.makedirs(save_dir, exist_ok=True)
    save_path = os.path.join(save_dir, f"{value_name}.png")
    
    # Download and save the image
    response = requests.get(url)
    if response.status_code == 200:
        with open(save_path, "wb") as file:
            file.write(response.content)
        print(f"Saved {value_name} of {suit_name} to {save_path}")
    else:
        print(f"Failed to download {value_name} of {suit_name}")

def download_all_cards():
    """Download and save all cards in a standard deck."""
    for suit_name, suit_code in SUITS.items():
        for value_name, value_code in VALUES.items():
            download_card_image(suit_name, suit_code, value_name, value_code)

if __name__ == "__main__":
    download_all_cards()
