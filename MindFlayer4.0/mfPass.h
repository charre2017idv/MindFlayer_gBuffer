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
#include "mfRenderTarget.h"
#include "mfVertexShader.h"
#include "mfInputLayout.h"
#include "mfPixelShader.h"
#include "mfMesh.h"
#include "mfCamera.h"
#include "mfConstBuffer.h"
#include "mfSampler.h"
#include "mfGameObject.h"
#include "mfRasterizer.h"
#pragma once
class mfLoadModel;
class mfDepthStencilView;
/**
 * @brief : 
 */
struct mfPassDesc
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
  mfPassDesc m_descriptor;
  /**
   * @brief : 
   */
  mfRenderTarget m_RenderTarget;

  ID3D11Texture2D* renderTargetTextureArray[4];
  ID3D11RenderTargetView* renderTargetViewArray[4];
  ID3D11ShaderResourceView* shaderResourceViewArray[4];

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
  mfConstBuffer m_ViewBuffer;
  /**
   * @brief : 
   */
  mfConstBuffer m_ProjBuffer;
  /**
   * @brief : 
   */
  mfCamera m_Camera;
  /**
   * @brief : 
   */
  mfSampler m_SamplerState;
  /**
   * @brief : 
   */
  mfRasterizer m_Rasterizer;
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
  ID3D11ShaderResourceView* m_Textures[4];
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
    Init(mfPassDesc _Desc);
  /**
    * @brief  :
    * @param  :
    * @bug    : No known bugs.
    */
  void
    Update(mfDepthStencilView & _DepthStencilView);
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

};

