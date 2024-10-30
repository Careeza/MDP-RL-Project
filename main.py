# main.py

from game_engine import GameEngine

def main():
    # Set fullscreen to True or False based on your preference
    game = GameEngine(fullscreen=True)
    game.run()

if __name__ == "__main__":
    main()
