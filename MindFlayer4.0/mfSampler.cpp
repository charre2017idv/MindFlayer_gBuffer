#include "mfSampler.h"



mfSampler::mfSampler()
{
#ifdef mfDIRECTX
  m_Sampler.ID = NULL;
#endif // mfDIRECTX

}


mfSampler::~mfSampler()
{
}

void mfSampler::Init(mfSamplerDesc _Desc)
{
  m_descriptor = _Desc;
#ifdef mfDIRECTX

  mfGraphic_API::getSingleton().CreateSamplerState(*this);
#elif defined mfOPENGL
  mfOutputLOG("mfSampler", "Init()", "Sampler has been Initializated.");
#endif // mfDIRECTX
}

void mfSampler::Update()
{
#ifdef mfDIRECTX
  mfGraphic_API::getSingleton().PSSetSamplers(0, 1, *this);
#elif defined mfOPENGL
  mfOutputLOG("mfSampler", "Render()", "Render has been Initializated.");
#endif // mfDIRECTX
}

void mfSampler::Destroy()
{
#ifdef mfDIRECTX
  SAFE_RELEASE(m_Sampler.ID);
#endif // mfDIRECTX
}

mfSamplerID & mfSampler::getInterface()
{
  return m_Sampler;
}

mfSamplerDesc & mfSampler::getDescriptor()
{
  return m_descriptor;
}
