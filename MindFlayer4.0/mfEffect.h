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
#include "mfTechnique.h"
#pragma once
/**
 * Forward Class Declarations 
 */
class mfDepthStencilView;
class mfPass;
/**
 * @brief : 
 */
class mfEffect
{
public:
  mfEffect();
  ~mfEffect();
  /**
   * Members 
   */
private:
  /**
   * @brief : 
   */
  vector<mfTechnique> m_Techniques;
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
  Init(mfBasePassDesc _PassDesc);
  /**
    * @brief  : 
    * @param  : 
    * @bug    : No known bugs.
    */
  void 
  Update(mfDepthStencilView & _DepthStencilView, const void * _Resource, float _Time);
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


