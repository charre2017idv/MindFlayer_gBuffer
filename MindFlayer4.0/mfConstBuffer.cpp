/**
 * @LC	   : 14/10/2019
 * @file   : mfconstBuffer.cpp
 * @Author : Roberto Charreton (idv17c.rcharreton@uartesdigitales.edu.mx)
 * @date   : 14/10/2019
 * @brief  : Class in charge of setting const buffer.
             This class inherits from mfbuffer.
 * @bug	   : No known bugs.
 */
#include "mfConstBuffer.h"



mfConstBuffer::mfConstBuffer()
{
}


mfConstBuffer::~mfConstBuffer()
{
}

void mfConstBuffer::Init(mfBufferDesc _Desc)
{
  mfBuffer::Init(_Desc);
  HRESULT hr = mfGraphic_API::getSingleton().CreateBuffer(*this);
  if (FAILED(hr))
  {
    mfOutputLOG("mfConstBuffer", "init()", "Const Buffer creation fail.")
  }
}

void mfConstBuffer::Update(const void * _Resource)
{
  // Check if the current buffer have CPU access
  if (m_descriptor.CPUAccessFlags == mf_CPU_ACCESS_WRITE)
  {
    mfGraphic_API::getSingleton().UpdateSubresource(*this, _Resource);
  }
}

void mfConstBuffer::Render(unsigned int _startSlot, unsigned int _numBuffers, bool _isChangeEveryFrame)
{
  mfGraphic_API::getSingleton().VSSetConstantBuffers
  (
    _startSlot, 
    _numBuffers,
    *this
  );
  if (_isChangeEveryFrame)
  {
    mfGraphic_API::getSingleton().PSSetConstantBuffers
    (
      _startSlot, 
      _numBuffers, 
      *this
    );
  }
}



