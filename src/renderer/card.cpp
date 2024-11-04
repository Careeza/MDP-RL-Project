#include "renderer.hpp"

Card_Renderer::Card_Renderer(SDL_Texture *texture_front, SDL_Texture *texture_back) : texture_front(texture_front), texture_back(texture_back) {
	SDL_QueryTexture(texture_front, NULL, NULL, &rect.w, &rect.h);
	face_up = true;
}

Card_Renderer::~Card_Renderer() {
}

void Card_Renderer::draw(Renderer& renderer) {
	if (face_up)
		renderer.draw(texture_front, NULL, &rect);
	else
		renderer.draw(texture_back, NULL, &rect);
}

void Card_Renderer::place_at(int x, int y, bool face_up) {
	rect.x = x;
	rect.y = y;
	this->face_up = face_up;
}

void Card_Renderer::flip(bool _face_up) {
	face_up = _face_up;
}

bool Card_Renderer::is_face_up() {
	return face_up;
}