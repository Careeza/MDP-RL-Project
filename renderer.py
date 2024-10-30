# renderer.py

import pygame
import os
import random

class Renderer:
	def __init__(self, screen, background_path):
		self.screen = screen
		self.background = pygame.image.load(background_path)
		self.screen_width, self.screen_height = self.screen.get_size()
		self.bg_width, self.bg_height = self.background.get_size()

		print(f"Screen size: {self.screen_width}x{self.screen_height}")
		print(f"Background size: {self.bg_width}x{self.bg_height}")

		# Calculate scale factor and center position for the background
		self.scale_factor = min(self.screen_width / self.bg_width, self.screen_height / self.bg_height)
		self.new_bg_width = int(self.bg_width * self.scale_factor)
		self.new_bg_height = int(self.bg_height * self.scale_factor)
		self.background = pygame.transform.scale(self.background, (self.new_bg_width, self.new_bg_height))
		self.bg_x = (self.screen_width - self.new_bg_width) // 2
		self.bg_y = (self.screen_height - self.new_bg_height) // 2

	def render_background(self):
		# Fill screen with black to handle any gaps due to aspect ratio
		self.screen.fill((0, 0, 0))
		# Draw the background image centered
		self.screen.blit(self.background, (self.bg_x, self.bg_y))

	def draw_random_cards(self, num_cards=2):
		"""Randomly selects a specified number of card images."""
		suits = ["Hearts", "Diamonds", "Clubs", "Spades"]
		values = ["A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"]
		
		selected_cards = []
		for _ in range(num_cards):
			suit = random.choice(suits)
			value = random.choice(values)
			card_path = f"resources/{suit}/{value}.png"
			if os.path.exists(card_path):  # Ensure the card file exists
				selected_cards.append(card_path)
		return selected_cards

	def render_cards(self, player_cards, dealer_cards, screen_width, screen_height):
		"""Renders player and dealer cards on the screen."""
		
		# Load and render player cards at the bottom center
		for i, card_path in enumerate(player_cards):
			card_image = pygame.image.load(card_path).convert_alpha()
			card_rect = card_image.get_rect()
			card_rect.centerx = screen_width // 2 + (i * 120) - 60  # Offset to avoid overlap
			card_rect.centery = screen_height - 150
			self.screen.blit(card_image, card_rect)

		# Load and render dealer cards at the top center
		for i, card_path in enumerate(dealer_cards):
			card_image = pygame.image.load(card_path).convert_alpha()
			card_rect = card_image.get_rect()
			card_rect.centerx = screen_width // 2 + (i * 120) - 60  # Offset to avoid overlap
			card_rect.centery = 150
			self.screen.blit(card_image, card_rect)

	def update(self):
		pygame.display.flip()
