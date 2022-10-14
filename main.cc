/**
 * main.cc
 *
 * This is the entrypoint to the application. Contains mostly
 * platform-dependent scaffolding and delegates to an implementation of the
 * GameBase interface for all game logic. The codepath differs when compiling
 * to WebAssembly with Emscripten.
 */

#include <SDL2/SDL.h>

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
    double elapsed = SDL_GetTicks() - ticks;
    SDL_Delay(std::max(0, static_cast<int>(16.667 - elapsed)));
  }
#endif

  return 0;
}
