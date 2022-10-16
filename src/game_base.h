#ifndef GAME_BASE_H_
#define GAME_BASE_H_

#include <SDL2/SDL.h>

/**
 * Interface used by the bootstrapping code (in main.cc) and implemented by the
 * game logic (in game.h). See protected section for stuff you can use in the
 * implementation. This is the contract relied upon by the bootstrapping code
 * so I don't recommend changing this.
 */
class GameBase {
 public:
  GameBase();
  ~GameBase();
  virtual void Update(double){};
  virtual void Render(){};
  virtual void HandleEvent(SDL_Event&){};
  bool ShouldContinue();

 protected:
  void QuitSoon();
  void Crash();
  int screen_width_;
  int screen_height_;
  SDL_Window* window_;
  SDL_Renderer* renderer_;
 private:
  bool should_quit_;
};

#endif  // GAME_BASE_H_
