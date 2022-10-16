#include "game_base.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include <Log/log.h>

#include <exception>
#include <iostream>

GameBase::GameBase()
    : screen_width_(640),
      screen_height_(480),
      window_(nullptr),
      renderer_(nullptr),
      should_quit_(false) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
    Log("SDL_Init") << SDL_GetError();
    Crash();
  }

  if (SDL_CreateWindowAndRenderer(screen_width_, screen_height_, 0, &window_,
                                  &renderer_)) {
    Log("SDL_CreateWindowAndRenderer") << SDL_GetError();
    Crash();
  }

  if (TTF_Init() == -1) {
    Log("TTF_Init") << TTF_GetError();
  }

  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
    Log("Mix_OpenAudio") << Mix_GetError();
    Crash();
  }
}

GameBase::~GameBase() {
  Log("GameBase dtor");
  TTF_Quit();

  Mix_CloseAudio();

  SDL_DestroyRenderer(renderer_);
  SDL_DestroyWindow(window_);

  SDL_Quit();
}

bool GameBase::ShouldContinue() {
  return !should_quit_;
}

void GameBase::QuitSoon() {
  should_quit_ = true;
}

void GameBase::Crash() {
  throw std::exception();
}
