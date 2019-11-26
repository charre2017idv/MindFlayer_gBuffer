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
class mfSwapchain;

/**
 * @brief : 
 */
struct mfLightPassDesc
{
  ID3D11Texture2D* RenderTargetsTex_Ref[4];
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
class mfLightPass
{
public:
  mfLightPass();
  ~mfLightPass();
  /**
   * Members 
   */
protected:
  /**
   * @brief :
   */
  mfLightPassDesc m_descriptor;
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
private:
  /**
   * @brief : 
   */
  mfLightPassDesc  m_lightDescriptor;
  /**
   * @brief : 
   */
  mfLightShaderAttributes m_LightAttributes;
  /**
   * @brief : 
   */
  ID3D11ShaderResourceView* m_gBufferTex[4];
  /**
   * @brief : 
   */
  mfConstBuffer m_ShaderAttribtuesBuffer;
  /**
   * @brief : 
   */
  mfRenderTarget m_BackBufferRT;
  /**
   * @brief :
   */
  float whiteClearcolor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; // red, green, blue, alpha 

public:
  ID3D11Texture2D* m_backbuffer;
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
  Init(mfLightPassDesc _Desc, mfSwapchain & _Swapchain);
  /**
    * @brief  : 
    * @param  : 
    * @bug    : No known bugs.
    */
  void 
  Update(mfDepthStencilView & _DepthStencilView, Vector4 _Eye);
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
  mfLightPassDesc & getLightDescriptor();
};

