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
#include "mfTexture.h"
#include "mfRenderTarget.h"
#include "mfVertexShader.h"
#include "mfInputLayout.h"
#include "mfPixelShader.h"
#include "mfMesh.h"
#include "mfSampler.h"
#include "mfGameObject.h"
#include "mfRasterizer.h"
#include "mfConstBuffer.h"
#include "mfCamera.h"
#pragma once
/**
 * Forward Class Declarations 
 */
class mfLoadModel;
class mfDepthStencilView;
/**
 * @brief :
 */
struct mfBasePassDesc
{
  mfLoadModelID RawData;
  mfBufferDesc ViewBufferDesc;
  mfBufferDesc ProjBufferDesc;
  mfBufferDesc ModelBufferDesc;
  mfSamplerDesc SamplerDesc;
  mfCameraDesc CameraDesc;
  WCHAR * VertexShaderFileName;
  WCHAR * PixelShaderFileName;
  mfRasterizerDesc RasterizerDesc;
  mfInputLayoutDesc InputLayoutDesc;
};
/**
 * @brief : 
 */
struct mfgBufferPassID
{
  /**
   * @brief :
   */
  ID3D11RenderTargetView* RenderTargets[4];
  /**
   * @brief : 
   */
  ID3D11Texture2D* RenderTargetTextures[4];
};

  /**
   * @brief :
   */
class mfPass
{
public:
  mfPass();
  ~mfPass();
  /**
   * Members
   */
private:
  /**
   * @brief : 
   */
  mfgBufferPassID m_gBuffer;
  
  /**
   * @brief : 
   */
  float ClearColor0[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha   
  /**
   * @brief : 
   */
  float ClearColor1[4] = { 1.0f, 0.0f, 0.0f, 1.0f }; // red, green, blue, alpha  
  /**
   * @brief : 
   */
  float ClearColor2[4] = { 0.0f, 1.0f, 0.0f, 1.0f }; // red, green, blue, alpha   
  /**
   * @brief : 
   */
  float ClearColor3[4] = { 0.0f, 0.0f, 1.0f, 1.0f }; // red, green, blue, alpha 
  /**
   * @brief : 
   */
  float whiteClearcolor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; // red, green, blue, alpha 
  /**
   * @brief : 
   */
  ID3D11ShaderResourceView* m_Textures[2];
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
  mfBasePassDesc m_descriptor;
private:
  /**
   * @brief :
   */
  mfGameObject m_GameObject;
  /**
   * @brief :
   */
  mfVertexShader m_VertexShader;
  /**
   * @brief :
   */
  mfInputLayout m_InputLayout;
  /**
   * @brief :
   */
  mfPixelShader m_PixelShader;
  /**
   * @brief :
   */
  mfSampler m_SamplerState;
  /**
   * @brief :
   */
  mfRasterizer m_Rasterizer;
  /**
   * Methods
   */
public:
  /**
    * @brief  :
    * @param  :
    * @bug    : No known bugs.
    */
  void
    Init(mfBasePassDesc _Desc);
  /**
    * @brief  :
    * @param  :
    * @bug    : No known bugs.
    */
  void
    Update(mfDepthStencilView & _DepthStencilView, float _Time);
  /**
    * @brief  : 
    * @param  : 
    * @bug    : 
    */
  void Render();
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
  mfgBufferPassID & getInterface();

  mfCamera & getCamera();
};

