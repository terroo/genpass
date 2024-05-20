#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_opengl3.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>
#include "genpass.hpp"
#include <array>
#include "font.hpp"

#ifdef __EMSCRIPTEN__
#include "imgui/emscripten_mainloop_stub.h"
#endif

int main(){
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window* window = SDL_CreateWindow(
      "C++, Dear ImGui, SDL2, e OpenGL", 
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
      1280, 720, 
      SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE
      );
  SDL_GLContext gl_context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, gl_context);
  SDL_GL_SetSwapInterval(1);

  // ImGui START
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  ImGui::StyleColorsDark();
  ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
  ImGui_ImplOpenGL3_Init("#version 100");

  //ImFont * font = io.Fonts->AddFontFromFileTTF("./assets/Cantarell-Regular.ttf", 23.f);
  ImFont * font = io.Fonts->AddFontFromMemoryTTF((void*)data.data(), data.size(), 23.0f);
  IM_ASSERT(font != nullptr);

  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  // Obter dimensões da janela dinamicamente
  int width {1}, height {1};
  SDL_GetWindowSize(window, &width, &height);

  // Variaveis para o ImGui
  int size {8};
  bool hide {false}, copied {false};
  std::string pass {};

#ifdef __EMSCRIPTEN__
  io.IniFilename = nullptr;
  EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (true)
#endif
    {

      SDL_Event event;
      while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT){
          exit(0);
        }
      }

      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplSDL2_NewFrame();
      ImGui::NewFrame();


      ImGui::SetNextWindowPos(
        ImVec2( 
          (float)width / 2 - (float)width / 4,
          (float)height / 2 - (float)height / 4
        ), ImGuiCond_Always
      );
      ImGui::SetNextWindowSize(
          ImVec2(
            (float)width / 2, 
            (float)height / 2
          ), ImGuiCond_Always

      );

      ImGui::Begin("Gerador de Senhas :: ImGui", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

      float w_window = ImGui::GetWindowWidth();
      float w_item   = 400.f;
      float pos_x    = (w_window - w_item ) * 0.5f;

      float h1_size = ImGui::CalcTextSize("Gerador de Senhas").x;
      float h1_x    = (w_window - h1_size ) * 0.5f;
      ImGui::SetCursorPosX(h1_x);
      ImGui::Text("Gerador de Senhas");

      ImGui::SetCursorPosX(pos_x);
      ImGui::SetNextItemWidth(400.f);
      if(!hide){
        ImGui::InputText("##Gerar", pass.data(), 32, ImGuiInputTextFlags_ReadOnly);
      }else{
        ImGui::InputText("##Gerar", pass.data(), 32, ImGuiInputTextFlags_Password | ImGuiInputTextFlags_ReadOnly);
      }

      ImGui::SetCursorPosX(pos_x);
      ImGui::SetNextItemWidth(400.f);
      ImGui::SliderInt(" ", &size, 8, 32);

      ImGui::SetCursorPosX(pos_x);
      ImGui::Checkbox("Hide", &hide);

      ImGui::SameLine();
      ImGui::Text("(Esconder a senha)");
      
      ImGui::SetCursorPosX(pos_x);
      if( ImGui::Button("Gerar", ImVec2(400, 30)) ){
        auto g = std::make_unique<GenPass>();
        pass = g->gen_pass(size);
        size = pass.size();
        copied = false;
      }

      ImGui::SetCursorPosX(pos_x);
      if(!copied){
        if( ImGui::Button("Copiar", ImVec2(400, 30)) ){
          copied = true;
        }
      }else{
        ImGui::TextColored(ImVec4(1.f, 0.0f, 0.f, 1.0f), "Recurso não disponível nessa branch.");
      }


      ImGui::SetCursorPosX(pos_x);
      float footer_size = ImGui::CalcTextSize("www.terminalroot.com.br").x;
      float footer_x    = (w_window - footer_size ) * 0.5f;
      ImGui::SetCursorPosX(footer_x);
      ImGui::Text("www.terminalroot.com.br");
      
      ImGui::End();

      ImGui::Render();
      glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
      glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
      glClear(GL_COLOR_BUFFER_BIT);
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      SDL_GL_SwapWindow(window);
    }
#ifdef __EMSCRIPTEN__
  EMSCRIPTEN_MAINLOOP_END;
#endif

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
