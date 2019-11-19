#include "mfRasterizer.h"



mfRasterizer::mfRasterizer()
{
}


mfRasterizer::~mfRasterizer()
{
}

void mfRasterizer::Init(mfRasterizerDesc _Desc)
{
  m_descriptor = _Desc;
  mfGraphic_API::getSingleton().GetDevice().CreateRasterizer(*this);
}

void mfRasterizer::Update()
{
  mfGraphic_API::getSingleton().GetDeviceContext().RSSetState(*this);
}

void mfRasterizer::Destroy()
{
#ifdef mfDIRECTX
  SAFE_RELEASE(m_Rasterizer.ID);
#endif // mfDIRECTX
}

mfRasterizerID & mfRasterizer::getInterface()
{
  return m_Rasterizer;
}

mfRasterizerDesc & mfRasterizer::getDescriptor()
{
  return m_descriptor;
}
