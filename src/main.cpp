#include "ai.hpp"
#include "game.hpp"
#include "renderer.hpp"
#include <iostream>

// void	loop() {
// 	Event_Type	event_type = NONE;
// 	bool		close_requested = false;
// 	bool		need_refresh = true;
// 	SDL_Event	event;
// 	BlackJackInterface interface;

// 	interface.add_card(0, true);
// 	interface.add_card(1, true);
// 	interface.add_card(3, false);
// 	interface.add_card(4, false);

// 	while (!close_requested) {
// 		while (SDL_PollEvent(&event)) {
// 			event_type = interface.handle_event(event, NULL);
// 			switch (event_type)
// 			{
// 			case CLOSE_REQUESTED:
// 				close_requested = true;
// 				break;
// 			case NEED_REFRESH:
// 				need_refresh = true;
// 				break;
// 			default:
// 				break;
// 			}
// 			if (need_refresh) {
// 				interface.draw_interface();
// 				need_refresh = false;
// 			}
// 		}
// 	}
// }

int main() {
	// std::cout << "Hello, World!" << std::endl;
	Game	game;

	// seed the random number generator
	srand(time(NULL));
	game.game_loop();
	// loop();
	return 0;
}