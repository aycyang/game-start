#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#include "game.h"
#include "game_base.h"

bool mainLoop(double dt, GameBase* g) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    g->HandleEvent(event);
  }
  g->Update(dt);
  g->Render();

  return g->ShouldContinue();
}

#ifdef __EMSCRIPTEN__
EM_BOOL emLoop(double time, void* g) {
  // This function-local static variable is initialized exactly once during the
  // entire execution of the program -- when control first passes through its
  // declaration -- i.e. subsequent calls to this function will not reset this
  // value.
  static double prevTime = 0;

  const double dt = time - prevTime;
  prevTime = time;

  const bool shouldContinue = mainLoop(dt, (GameBase*)g);

  return shouldContinue ? EM_TRUE : EM_FALSE;
}
#endif

int main(int argc, char** argv) {
  // When compiling to WebAssembly with Emscripten, we must declare the Game
  // instance static, or else the memory goes out of bounds at some point.
  // Allocating it on the stack or the heap does not work.
  static Game game;

#ifdef __EMSCRIPTEN__
  emscripten_request_animation_frame_loop(emLoop, (void*)&game);
#else
  double dt;
  double prevTicks = 0;
  bool shouldContinue = true;

  while (shouldContinue) {
    double ticks = SDL_GetTicks();
    dt = ticks - prevTicks;
    prevTicks = ticks;
    shouldContinue = mainLoop(dt, &game);
    SDL_Delay(17);
  }
#endif

  return 0;
}
