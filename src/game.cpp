#include "game.hpp"
#include "renderer.hpp"
#include <iostream>
#include <SDL.h>

int		calculate_score(std::vector<int> hand) {
	int	score = 0;
	int	ace_count = 0;

	for (int card : hand) {
		if (card % 13 == 0) {
			ace_count++;
			score += 11;
		} else if (card % 13 >= 10) {
			score += 10;
		} else {
			score += card % 13 + 1;
		}
	}
	while (score > 21 and ace_count > 0) {
		score -= 10;
		ace_count--;
	}
	return (score);
}


Game::Game() {
	renderer = new Renderer();
	game_interface = new BlackJackInterface(*renderer);
	bet_interface = new BlackJackInterface(*renderer);
	game_over = false;
	is_dealer_turn = false;
}

Game::~Game() {
	delete renderer;
	delete game_interface;
	delete bet_interface;
}

int	Game::draw_card() {
	return (rand() % 52);
}

void	Game::player_draw_card() {
	player_hand.push_back(draw_card());
	player_score = calculate_score(player_hand);
	game_interface->add_card(player_hand.back(), true);
	game_interface->update_player_score(player_score);
}

void	Game::dealer_draw_card() {
	dealer_hand.push_back(draw_card());
	dealer_score = calculate_score(dealer_hand);
	game_interface->add_card(dealer_hand.back(), false);
	game_interface->update_dealer_score(dealer_score);

}

void	Game::start_bet() {
}

void	Game::start_game() {
	player_draw_card();
	player_draw_card();
	dealer_draw_card();
	dealer_draw_card();
}

// resolve the game

void	Game::player_turn() {
}

bool	Game::dealer_turn() {
	if (player_score > 21) {
		return true;
	}
	if (dealer_score < 17 and dealer_score <= player_score and dealer_score < 21) {
		dealer_draw_card();
		return false;
	}
	return true;
}

void	Game::restart_game() {
	player_hand.clear();
	dealer_hand.clear();
	player_score = 0;
	dealer_score = 0;
	game_over = false;
	is_dealer_turn = false;
	game_interface->reset();
	start_game();
}

void	init_bet_interface() {}

void	init_game_interface() {}

void	Game::game_loop() {
	Event_Type			event_type = NONE;
	bool				close_requested = false;
	bool				need_refresh = true;
	SDL_Event			event;
	BlackJackInterface	*interface;

	interface = game_interface;
	start_game();
	while (!close_requested) {
		while (SDL_PollEvent(&event)) {
			event_type = interface->handle_event(event, NULL);
			switch (event_type) {
			case CLOSE_REQUESTED:
				close_requested = true;
				break;
			case PLAYER_HIT:
				if (is_dealer_turn or game_over) {
					std::cout << "player tried to hit" << std::endl;
					break;
				}
				std::cout << "player hit" << std::endl;
				player_draw_card();
				need_refresh = true;
				break;
			case PLAYER_STAND:
				if (is_dealer_turn or game_over) {
					break;
				}
				is_dealer_turn = true;
				need_refresh = true;
				break;
			case GAME_RESTART:
				std::cout << "ask to restart" << std::endl;
				if (game_over) {
					std::cout << "game restart" << std::endl;
					restart_game();
					need_refresh = true;
				}
				break;
			default:
				break;
			}

		}
		if (!game_over and player_score > 21) {
			is_dealer_turn = true;
		}
		if (!game_over and is_dealer_turn) {
			game_over = dealer_turn();
			need_refresh = true;
		}
		if (need_refresh) {
			interface->set_dealer_turn(is_dealer_turn);
			interface->draw_interface();
			need_refresh = false;
		}
	}
	std::cout << "game loop ended" << std::endl;
}
