#pragma once

#include <vector>
#include "renderer.hpp"

class Game {
	private:
		std::vector<int>	player_hand;
		std::vector<int>	dealer_hand;
		int					player_score;
		int					dealer_score;
		bool				game_over;
		bool				is_dealer_turn;
		Renderer			*renderer;
		BlackJackInterface	*game_interface;
		BlackJackInterface	*bet_interface;

	public:
		Game();
		~Game();
		int		draw_card();
		void	player_draw_card();
		void	dealer_draw_card();

		void	restart_game();
		void	game_loop();
		void	start_bet();
		void	start_game();
		void	player_turn();
		bool	dealer_turn();
};