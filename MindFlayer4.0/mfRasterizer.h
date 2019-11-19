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
#pragma once
/**
 * @brief : 
 */
struct mfRasterizerID
{
  ID3D11RasterizerState* ID;
};
/**
 * @brief : 
 */
struct mfRasterizerDesc
{
  mf_CULL_MODE CullMode;
  mf_FILL_MODE FillMode;

};

/**
 * @brief : 
 */
class mfRasterizer
{
public:
  mfRasterizer();
  ~mfRasterizer();
  /**
   * Members 
   */
private:
  /**
   * @brief : 
   */
  mfRasterizerID m_Rasterizer;
  /**
   * @brief : 
   */
  mfRasterizerDesc m_descriptor;
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
  Init(mfRasterizerDesc _Desc);
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
  mfRasterizerID & getInterface();
  /**
    * @brief  : 
    * @param  : 
    * @bug    : 
    */
  mfRasterizerDesc & getDescriptor();
};

