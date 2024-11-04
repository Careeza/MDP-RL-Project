#pragma once

# include <iostream>
# include <vector>
# include <SDL.h>
# include <SDL_image.h>
# include <SDL_ttf.h>
# include <functional>
# include <string>
# include <array>

# define CANVA_WIDTH 1920
# define CANVA_HEIGHT 1080

typedef std::pair<int, int> coord;

class Renderer {
	private:
		SDL_Window*		window;
		SDL_Renderer*	renderer;
		SDL_Texture*	canva;
		TTF_Font		*font;
		SDL_Rect		dst;
		int				window_width;
		int				window_height;

	public:
		Renderer();
		~Renderer();

		void			clear();
		void			present();
		void			draw(SDL_Texture* texture, SDL_Rect* src, SDL_Rect* dst);
		void			draw_text(const std::string& text, int x, int y);
		coord			transform_mouse_pos(coord mouse_pos);
		bool 			take_screenshot();
		SDL_Texture*	load_texture(const std::string path);
};

class Button {
	private:
		SDL_Texture*				texture;
		SDL_Rect					rect;
		std::function<void(void *)>	onClick;

	public:
		Button(SDL_Texture* texture, SDL_Rect rect, std::function<void(void *)> onClick);
		~Button();

		void	draw(Renderer& renderer);
		void	handle_event(Renderer& renderer, SDL_Event event, void *data);
};

class Card_Renderer {
	private:
		SDL_Texture*	texture_front;
		SDL_Texture*	texture_back;
		SDL_Rect		rect;
		bool			face_up;

	public:
		Card_Renderer(SDL_Texture *texture_front, SDL_Texture *texture_back);
		~Card_Renderer();

		void	draw(Renderer& renderer);
		void	place_at(int x, int y, bool face_up=true);
		void	flip(bool face_up=true);
		bool	is_face_up();
};

enum Event_Type {
	CLOSE_REQUESTED,
	NEED_REFRESH,
	PLAYER_HIT,
	PLAYER_STAND,
	GAME_RESTART,
	NONE,
};

class BlackJackInterface {
	private:
		std::vector<Card_Renderer>	player_cards;
		std::vector<Card_Renderer>	dealer_cards;
		std::vector<SDL_Texture*>	card_textures;
		SDL_Texture*				background_texture;
		SDL_Texture*				back_card_texture;
		Renderer					&renderer;
		std::vector<Button*>		buttons;
		bool						dealer_turn;
		int							player_score;
		int							dealer_score;

	public:
		BlackJackInterface(Renderer& renderer);
		~BlackJackInterface();

		Event_Type	handle_event(SDL_Event& event, void *data);
		void		add_card(int card, bool player);
		void		draw_interface();
		void		add_button(SDL_Texture* texture, SDL_Rect rect, std::function<void(void *)> onClick);
		void		set_dealer_turn(bool dealer_turn);
		void		update_player_score(int score);
		void		update_dealer_score(int score);
		void		reset();
};