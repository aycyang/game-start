#ifndef GAME_BASE_H_
#define GAME_BASE_H_

#include <SDL2/SDL.h>

#include <iostream>
#include <string>

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
  void Log(std::string);
  void Log(std::string, std::string);
  int screen_width_;
  int screen_height_;
  SDL_Window* window_;
  SDL_Renderer* renderer_;
 private:
  bool should_quit_;
};

#endif  // GAME_BASE_H_
