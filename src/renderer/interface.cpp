#include "renderer.hpp"

void	test_function(void *data) {
	std::cout << "Button clicked" << std::endl;
}

BlackJackInterface::BlackJackInterface(Renderer& _renderer) : renderer(_renderer) {
	background_texture = renderer.load_texture("resources/background.png");
	back_card_texture = renderer.load_texture("resources/back-of-cards.png");
	for (std::string suit : {"clubs", "diamonds", "hearts", "spades"}) {
		for (std::string value : {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"}) {
			std::string path = "resources/" + suit + "/" + value + ".png";
			SDL_Texture* texture = renderer.load_texture(path);
			card_textures.push_back(texture);
		}
	}
	dealer_turn = false;
	// SDL_Texture *texture = renderer.load_texture("resources/chip.png");
	// add_button(texture, {1920/2-100, 1080/2-100, 200, 200}, test_function);
}

BlackJackInterface::~BlackJackInterface() {
	for (SDL_Texture* texture : card_textures) {
		SDL_DestroyTexture(texture);
	}
	for (Button *button : buttons) {
		delete button;
	}
	SDL_DestroyTexture(background_texture);
	SDL_DestroyTexture(back_card_texture);
}

void BlackJackInterface::add_card(int card, bool player) {
	Card_Renderer	new_card(card_textures[card], back_card_texture);
	if (player) {
		player_cards.push_back(new_card);
		size_t number_of_cards = player_cards.size();
		int x = 1920/2 - 113 - 75 * (number_of_cards - 1);
		int y = 716;
		for (size_t i = 0; i < number_of_cards; i++) {
			player_cards[i].place_at(x + 150 * i, y);
		}
	} else {
		dealer_cards.push_back(new_card);
		size_t number_of_cards = dealer_cards.size();
		int x = 1920/2 - 113 - 75 * (number_of_cards - 1);
		int y = 50;
		for (size_t i = 0; i < number_of_cards; i++) {
			dealer_cards[i].place_at(x + 150 * i, y);
		}
	}
}

void BlackJackInterface::add_button(SDL_Texture* texture, SDL_Rect rect, std::function<void(void *)> onClick) {
	Button	*button = new Button(texture, rect, onClick);
	buttons.push_back(button);
}

void BlackJackInterface::draw_interface() {
	renderer.clear();
	renderer.draw(background_texture, NULL, NULL);
	for (Card_Renderer card : player_cards) {
		card.draw(renderer);
	}
	for (size_t i = 0; i < dealer_cards.size(); i++) {
		if (i == 0) {
			dealer_cards[i].flip(dealer_turn);
			dealer_cards[i].draw(renderer);
		} else {
			dealer_cards[i].draw(renderer);
		}
	}
	for (Button* button : buttons) {
		button->draw(renderer);
	}
	//draw player score at the right of player card or bust
	if (player_score > 21) {
		renderer.draw_text("BUST", 1600, 716);
	} else {
		renderer.draw_text(std::to_string(player_score), 1600, 716);
	}
	//draw dealer score at the right of dealer card or bust
	if (dealer_turn) {
		if (dealer_score > 21) {
			renderer.draw_text("BUST", 1600, 50);
		} else {
			renderer.draw_text(std::to_string(dealer_score), 1600, 50);
		}
	} else {
		renderer.draw_text("?", 1600, 50);
	}

	renderer.present();
}

Event_Type	BlackJackInterface::handle_event(SDL_Event& event, void *data) {
	Event_Type	event_type = NONE;

	switch (event.type) {
		case SDL_QUIT:
			std::cout << "quit" << std::endl;
			event_type = CLOSE_REQUESTED;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					event_type = CLOSE_REQUESTED;
					break;
				case SDLK_SPACE:
					event_type = NEED_REFRESH;
					break;
				case SDLK_a:
					event_type = PLAYER_HIT;
					break;
				case SDLK_s:
					event_type = PLAYER_STAND;
					break;
				case SDLK_r:
					event_type = GAME_RESTART;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
	for (Button* button : buttons) {
		button->handle_event(renderer, event, data);
	}
	if (event_type == CLOSE_REQUESTED) {
		std::cout << "close requested" << std::endl;
	}
	return event_type;
}

void	BlackJackInterface::set_dealer_turn(bool _dealer_turn) {
	dealer_turn = _dealer_turn;
}

void	BlackJackInterface::update_player_score(int score) {
	player_score = score;
}

void	BlackJackInterface::update_dealer_score(int score) {
	dealer_score = score;
}

void	BlackJackInterface::reset() {
	player_cards.clear();
	dealer_cards.clear();
	player_score = 0;
	dealer_score = 0;
	dealer_turn = false;
}