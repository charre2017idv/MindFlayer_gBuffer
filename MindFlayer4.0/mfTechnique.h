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
#include "mfPass.h"
#pragma once
/**
 * Forward Class Declaration 
 */
class mfDepthStencilView;
class mfTransform;
/**
 * @brief : 
 */
class mfTechnique
{
public:
  mfTechnique();
  ~mfTechnique();
  /**
   * Members 
   */
private:
  vector<mfPass> m_Passes;
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
  Init(mfBasePassDesc _PassDesc, int _Index);
  /**
    * @brief  : 
    * @param  : 
    * @bug    : No known bugs.
    */
  void 
  Update(mfDepthStencilView & _DepthStencilView, mfTransform & _Transform, const void * _Resource, float _Time);
  /**
    * @brief  : 
    * @param  : 
    * @bug    : No known bugs.
    */
  void 
  Render(mfDepthStencilView & _DepthStencilView);
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
  void CreatePasses();
  /**
    * @brief  : 
    * @param  : 
    * @bug    : 
    */
  mfPass & GetPass(mfPassTypeID _PassType);

};

