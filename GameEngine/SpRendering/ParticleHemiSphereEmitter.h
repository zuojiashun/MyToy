#pragma once

#include"ParticleEmitter.h"

BEGIN_NAMESPACE_ENGINE

class ParticleHemiSphereEmitter : public ParticleEmitter
{
public:
	virtual void Emit(const ParticleDesc& desc, Particle& particle) override;
};

END_NAMESPACE_ENGINE