/**
 * @LC	   : //2019
 * @file   : 
 * @Author : Roberto Charreton (idv17c.rcharreton@uartesdigitales.edu.mx)
 * @date   : //2019
 * @brief  : 
 * @bug	   : No known bugs.
 */

/**
 * Headers 
 */
#include "mfDefines.h"
#include "mfGraphic_API.h"
#include "mfPass.h"
#include "mfLightPass.h"
#include "mfDepthStencil.h"
#include "mfDepthStencilView.h"
#include "mfSwapchain.h"
#include "mfViewport.h"
#include "mfLoadModel.h"
#include "mfOpenFile.h"
#include "mfWindow.h"
#include "mfCamera.h"
#include "mfEffect.h"
#include "mfRenderTarget.h"
#include "mfImGui.h"
#include "mfTransform.h"
#pragma once
/**
 * Forward Class Declarations 
 */
class mfConstBuffer;
class mfCamera;
class mfSampler;
class mfRasterizer;

/**
 * @brief :
 */
struct mfLightShaderAttributes
{
  XMFLOAT4 Position;                          // 124
  XMFLOAT4 lightDir;                          // Light direction in world space 96
  XMFLOAT4 lightDirColor;                     // Light direction color in linear space 128
  XMFLOAT4 ambientColor;                      // Ambient color linear space 128
  XMFLOAT4 specularColor;                     // Specular color in linear space 128
  float specPower;                            // Mult specular factor   32
  float kDiffuse;                             // Diffuse mult constant  32
  float kAmbient;                             // Ambient mult constant  32  
  float kSpecular;                            // Specular mult constant 32
};

/**
 * @brief : 
 */
class mfRenderManager
{
public:
  mfRenderManager();
  ~mfRenderManager();
  /**
   * Members
   */
private:
  /**
   * @brief : 
   */
  mfWindow m_Window;
  /**
   * @brief : 
   */
  mfSwapchain m_Swapchain;
  mfRenderTarget m_RenderTarget;
  /**
   * @brief : 
   */
  mfDepthStencil m_DepthStencil;
  /**
   * @brief : 
   */
  mfDepthStencilView m_DepthStencilView;
  /**
   * @brief : 
   */
  mfViewport m_Viewport;
  /**
   * @brief : 
   */
  mfLoadModel m_LoadModel;
  mfLoadModel m_loadSAQ;
  /**
   * @brief : 
   */
  mfOpenFile OpenModelFile;
  /**
   * @brief : 
   */
  mfOpenFile OpenTextureFile;

  /**
   * @brief : 
   */
  mfEffect m_gBuffer_Effect;
  /**
    * @brief  : 
    * @param  : 
    * @bug    : 
    */
  mfLightShaderAttributes m_Shader_Light_Attributes;
  /**
   * @brief : 
   */
  mf_DRIVER_TYPE m_driverType = mf_DRIVER_TYPE_NULL;
  /**
   * @brief : 
   */
  D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_0;

  /**
   * @brief :
   */
  CB_CameraBuffer m_CameraVP;
  /**
   * @brief :
   */
  mfCamera m_Camera;
  /**
   * @brief : 
   */
  vector<mfRenderTarget> m_gBufferRT;
  /**
   * @brief : 
   */
  mfImGui m_ImGui;
  /**
   * @brief : 
   */
  mfTransform m_Transform;

  XMFLOAT3 m_Translate;
  XMFLOAT3 m_Rotation;
  XMFLOAT3 m_Scale;
  float Translate[3] = { 0,0,0 };
  float Rotation[3] = { 0,0,0 };
  float Scale[3] = { 50,50,50 };
  /**
   * @brief : 
   */
  float m_Time = 0.0f;
  float Position[4] = { 200, 0,0,0 };
  float lightDir[4] = {-115, 10, 1, 1};
  float lightDirColor[4]= { 1.0f,1.0f,1.0f,1.0f };
  float ambientColor[4] = { 0.0f,0.0f,0.0f,1.0f };
  float specularColor[4] = { 1, 0, 0, 1 };
  float specPower = 15.0f;
  float kDiffuse = 1.0f;
  float kAmbient = 0.14f;
  float kSpecular = 1.0f;

  bool isRotating = false;
  /**
   * Methods 
   */
public:
  /**
    * @brief  : 
    * @param  : 
    * @bug    : No known bugs.
    */
  HRESULT 
  Init();
  /**
    * @brief  : 
    * @param  : 
    * @bug    : No known bugs.
    */
  void 
  Update();
  /**
    * @brief  : 
    * @param  : 
    * @bug    : No known bugs.
    */
  void 
  Render();
  /**
    * @brief  : 
    * @param  : 
    * @bug    : No known bugs.
    */
  void 
  Destroy();
  /**
    * @brief  : 
    * @param  : 
    * @bug    : 
    */
  void InitWindow(WNDPROC _wndProc, HINSTANCE _HInstance, int _CmdShow);
  /**
    * @brief  : 
    * @param  : 
    * @bug    : 
    */
  void Initialize_gBuffer();
  /**
    * @brief  : 
    * @param  : 
    * @bug    : 
    */
  void Initialize_Light_Pass();
  /**
    * @brief  : 
    * @param  : 
    * @bug    : 
    */
  void Initialize_CopyBB_Pass(vector<mfRenderTarget> & _tmpRenderTarget);
  /**
    * @brief  : 
    * @param  : 
    * @bug    : 
    */
  void RTSelector_Window(vector<mfRenderTarget>& _RenderTargets);
  void Transform_Window();
};

