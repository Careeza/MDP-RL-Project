#include "renderer.hpp"

Button::Button(SDL_Texture* texture, SDL_Rect rect, std::function<void(void *)> onClick) : texture(texture), rect(rect), onClick(onClick) {
}

Button::~Button() {
	SDL_DestroyTexture(texture);
}

void Button::draw(Renderer& renderer) {
	int w, h;

	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	std::cout << w << " " << h << std::endl;
	std::cout << rect.x << " " << rect.y << " " << rect.w << " " << rect.h << std::endl;
	renderer.draw(texture, NULL, &rect);
}

void Button::handle_event(Renderer& renderer, SDL_Event event, void *data) {
	int		x, y;
	bool	is_inside = false;

	if (event.type == SDL_MOUSEBUTTONDOWN) {
		SDL_GetMouseState(&x, &y);
		auto pos = renderer.transform_mouse_pos({x, y});
		x = pos.first;
		y = pos.second;
		is_inside = x >= rect.x && x <= rect.x + rect.w 
			&& y >= rect.y && y <= rect.y + rect.h;
		if (is_inside) {
			onClick(data);
		}
	}
}