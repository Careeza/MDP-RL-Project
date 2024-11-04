#include "renderer.hpp"

#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

bool Renderer::take_screenshot() {
	int width, height;
	SDL_QueryTexture(canva, NULL, NULL, &width, &height);

	SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);
	if (!surface) {
		std::cerr << "Could not create surface: " << SDL_GetError() << std::endl;
		return false;
	}

	SDL_SetRenderTarget(renderer, canva);
	if (SDL_RenderReadPixels(renderer, NULL, surface->format->format, surface->pixels, surface->pitch) != 0) {
		std::cerr << "Could not read pixels: " << SDL_GetError() << std::endl;
		SDL_FreeSurface(surface);
		return false;
	}

	std::time_t t = std::time(nullptr);
	std::tm* now = std::localtime(&t);
	std::ostringstream filename;
	filename << "screenshot_" << std::setfill('0')
			<< std::setw(2) << now->tm_mday
			<< std::setw(2) << now->tm_hour
			<< std::setw(2) << now->tm_min
			<< std::setw(2) << now->tm_sec << ".png";

	if (IMG_SavePNG(surface, filename.str().c_str()) != 0) {
		std::cerr << "Could not save PNG: " << IMG_GetError() << std::endl;
		SDL_FreeSurface(surface);
		return false;
	}

	SDL_FreeSurface(surface);
	return true;
}


Renderer::Renderer() {
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	window = SDL_CreateWindow("blackJack", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_ALLOW_HIGHDPI);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	canva = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, CANVA_WIDTH, CANVA_HEIGHT);
	font = TTF_OpenFont("resources/arial.ttf", 72);

	SDL_GetRendererOutputSize(renderer, &window_width, &window_height);
	std::cout << "window size: " << window_width << "x" << window_height << std::endl;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	dst = {0, 0, window_width, window_height};
	if (window_width * CANVA_HEIGHT > window_height * CANVA_WIDTH) {
		dst.w = window_height * CANVA_WIDTH / CANVA_HEIGHT;
		dst.x = (window_width - dst.w) / 2;
	} else {
		dst.h = window_width * CANVA_HEIGHT / CANVA_WIDTH;
		dst.y = (window_height - dst.h) / 2;
	}
}

Renderer::~Renderer() {
	SDL_DestroyTexture(canva);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
	std::cout << "Renderer destroyed" << std::endl;
}

void Renderer::clear() {
	SDL_SetRenderTarget(renderer, canva);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
}

void Renderer::present() {
	SDL_Rect src = {0, 0, CANVA_WIDTH, CANVA_HEIGHT};

	SDL_SetRenderTarget(renderer, NULL);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, canva, &src, &dst);
	SDL_RenderPresent(renderer);
}

void Renderer::draw(SDL_Texture* texture, SDL_Rect* src, SDL_Rect* dst) {
	SDL_SetRenderTarget(renderer, canva);
	SDL_RenderCopy(renderer, texture, src, dst);
}

SDL_Texture	*Renderer::load_texture(const std::string path) {
	SDL_Surface* surface = IMG_Load(path.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	return texture;
}

coord Renderer::transform_mouse_pos(coord mouse_pos) {
	return {(mouse_pos.first - dst.x) * CANVA_WIDTH / window_width, (mouse_pos.second - dst.y) * CANVA_HEIGHT / window_height};
}

void	Renderer::draw_text(const std::string& text, int x, int y) {
	SDL_Color color = { 0, 0, 0, 255 };

	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
	if (!surface) {
		std::cerr << "Failed to create text surface: " << TTF_GetError() << std::endl;
		TTF_CloseFont(font);
		return;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture) {
		std::cerr << "Failed to create text texture: " << SDL_GetError() << std::endl;
	}

	SDL_FreeSurface(surface);
	int textWidth, textHeight;
	SDL_QueryTexture(texture, NULL, NULL, &textWidth, &textHeight);
	SDL_Rect destRect = { x, y, textWidth, textHeight };
	SDL_RenderCopy(renderer, texture, NULL, &destRect);
	SDL_DestroyTexture(texture);
}
