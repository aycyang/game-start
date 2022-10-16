#include "game.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include <Log/log.h>

Game::Game() : is_mouse_down_(false), is_space_down_(false) {
  {
    SDL_Surface* surface = IMG_Load("assets/SDL_logo.png");
    if (!surface) {
      Log("IMG_Load") << IMG_GetError();
      Crash();
    }
    SDL_GetClipRect(surface, &rect_);
    texture_ = SDL_CreateTextureFromSurface(renderer_, surface);
    SDL_FreeSurface(surface);
  }

  sfx_placeholder_ = Mix_LoadWAV("assets/bloop.wav");
  if (!sfx_placeholder_) {
    Log("Mix_LoadWAV") << Mix_GetError();
    Crash();
  }

  font_ = TTF_OpenFont("assets/Inconsolata.ttf", 48);
  if (!font_) {
    Log("TTF_OpenFont") << TTF_GetError();
  }

  {
    SDL_Surface* surface = TTF_RenderText_Shaded(
        font_, "Click here!", {255, 255, 255, 255}, {0, 0, 0, 255});
    SDL_GetClipRect(surface, &text_rect_);
    text_texture_ = SDL_CreateTextureFromSurface(renderer_, surface);
    SDL_FreeSurface(surface);
  }
}

Game::~Game() {
  TTF_CloseFont(font_);

  Mix_FreeChunk(sfx_placeholder_);
}

void Game::Update(double dt) {
  rect_.x = screen_width_ / 2 - rect_.w / 2;
  rect_.y = screen_height_ / 2 - rect_.h / 2;
  if (is_mouse_down_) {
    rect_.y += 12;
  }

  text_rect_.x = screen_width_ / 2 - text_rect_.w / 2;
  text_rect_.y = screen_height_ / 2 - text_rect_.h / 2 - rect_.h / 2 - 64;
  if (is_space_down_) {
    text_rect_.y -= 12;
  }
}

void Game::Render() {
  SDL_SetRenderDrawColor(renderer_, 0x77, 0x77, 0x77, 0xff);
  SDL_RenderClear(renderer_);

  SDL_RenderCopy(renderer_, texture_, nullptr, &rect_);
  SDL_RenderCopy(renderer_, text_texture_, nullptr, &text_rect_);
  SDL_RenderPresent(renderer_);
}

void Game::HandleEvent(SDL_Event& event) {
  if (event.type == SDL_QUIT) {
    QuitSoon();
  }
  if (event.type == SDL_WINDOWEVENT) {
    switch (event.window.event) {
      case SDL_WINDOWEVENT_FOCUS_GAINED:
        // TODO unpause
        break;
      case SDL_WINDOWEVENT_FOCUS_LOST:
        // TODO pause
        break;
    }
  }
  if (event.type == SDL_MOUSEBUTTONDOWN) {
    Mix_PlayChannel(-1, sfx_placeholder_, 0);
    is_mouse_down_ = true;
  }
  if (event.type == SDL_MOUSEBUTTONUP) {
    is_mouse_down_ = false;
  }
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_SPACE) {
      is_space_down_ = true;
    }
  }
  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_SPACE) {
      is_space_down_ = false;
    }
  }
}
