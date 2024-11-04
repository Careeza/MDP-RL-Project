# game_engine.py

import pygame
import sys
import random
from renderer import Renderer

class GameEngine:
	def __init__(self, fullscreen=True, screen_width=1920, screen_height=1080):
		pygame.init()
		self.screen_width, self.screen_height = screen_width, screen_height

		# Screen mode
		self.screen = pygame.display.set_mode((self.screen_width, self.screen_height), pygame.FULLSCREEN) if fullscreen \
			else pygame.display.set_mode((self.screen_width, self.screen_height))

		# Instantiate Renderer with the screen and background
		self.renderer = Renderer(self.screen, "resources/background.png")
		self.running = True

		# Load cards for player and dealer
		self.player_cards = self.renderer.draw_random_cards(2)
		self.dealer_cards = self.renderer.draw_random_cards(2)
		self.dealer_cards[1] = "resources/back-of-cards.png"  # Hide one dealer card

	def handle_events(self):
		for event in pygame.event.get():
			if event.type == pygame.QUIT:
				self.running = False
			elif event.type == pygame.KEYDOWN:
				if event.key == pygame.K_ESCAPE:
					self.running = False

	def run(self):
		# Main game loop
		while self.running:
			self.handle_events()
			self.renderer.render_background()
			self.renderer.render_cards(self.player_cards, self.dealer_cards, self.screen_width, self.screen_height)
			self.renderer.update()

		# Clean up
		pygame.quit()
		sys.exit()
