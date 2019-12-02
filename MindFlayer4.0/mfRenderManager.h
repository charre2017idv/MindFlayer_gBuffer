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
//   /**
//    * @brief : 
//    */
//   mfPass m_gBuffer_Pass;
//   /**
//    * @brief : 
//    */
//   mfLightPass m_Light_Pass;
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
  mfConstBuffer m_ViewBuffer;
  /**
   * @brief :
   */
  mfConstBuffer m_ProjBuffer;
  /**
   * @brief :
   */
  CBNeverChanges View;
  /**
   * @brief :
   */
  CBChangeOnResize Projection;
  /**
   * @brief :
   */
  mfCamera m_Camera;
  /**
   * @brief : 
   */
  float m_Time = 0.0f;
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
};

