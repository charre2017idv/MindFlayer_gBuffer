#include "mfImGui.h"
#include "mdDevice.h"
#include "mfDeviceContext.h"
#include "mfWindow.h"
#include "mfRenderTarget.h"

mfImGui::mfImGui()
{
}


mfImGui::~mfImGui()
{
}

void mfImGui::Init(mfDevice _Device, mfDeviceContext _DevContext, mfWindow _window)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
#ifdef mfDIRECTX
  ImGui_ImplWin32_Init(_window.getInterface().ID);
  ImGui_ImplDX11_Init(_Device.getInterface().ID, _DevContext.getInterface().ID);
#elif defined mfOPENGL
  mfOutputLOG("mfImGui", "Init()", "Imgui has been Initializated.");
#endif // mfDIRECTX
  ImGui::StyleColorsLight();
}

void mfImGui::Update()
{
#ifdef mfDIRECTX
  ImGui_ImplDX11_NewFrame();
  ImGui_ImplWin32_NewFrame();
#elif defined mfOPENGL
  mfOutputLOG("mfImGui", "Update()", "Update has been Initializated.");
#endif // mfDIRECTX

  ImGui::NewFrame();
}

void mfImGui::Render()
{
  ImGui::Render();

  // Render Draw Data
#ifdef mfDIRECTX
  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#elif defined mfOPENGL
  mfOutputLOG("mfImGui", "Render()", "Render has been Initializated.");
#endif // mfDIRECTX
}

void mfImGui::AestheticWindow(vector<mfRenderTarget>& _RenderTargets)
{
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;

  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  window_flags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

  ImGui::SetNextWindowSize(ImVec2(872, 184), ImGuiCond_Always);
  ImGui::SetWindowPos(ImVec2(0, 0));
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 1.0f, 1.0f, 0.8f)); // Set window background to red
  ImGui::Begin("gBuffer Pass", NULL, window_flags);
  ImGui::PopStyleVar(2);
  for (int i = 0; i < _RenderTargets.size(); i++)
  {
    ImGui::SameLine();
    ImGui::Image(_RenderTargets[i].getInterface().ResourceViewID, ImVec2(200, 128));
  }
  ImGui::PopStyleColor();
  ImGui::End();
}
