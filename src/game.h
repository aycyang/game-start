#ifndef GAME_H_
#define GAME_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "game_base.h"

/**
 * Put game logic here, primarily in Update(), Render(), and HandleEvent().
 * Feel free to add new methods for common procedures and new fields for
 * storing game state.
 */
class Game : public GameBase {
 public:
  Game();
  ~Game();
  void Update(double) override;
  void Render() override;
  void HandleEvent(SDL_Event&) override;

 private:
  bool should_quit_;
  bool is_mouse_down_;
  bool is_space_down_;
  SDL_Rect rect_;
  SDL_Texture* texture_;
  Mix_Chunk* sfx_placeholder_;
  TTF_Font* font_;
  SDL_Rect text_rect_;
  SDL_Texture* text_texture_;
};

#endif  // GAME_H_
