#include "ParticleHemiSphereEmitter.h"
#include "..\SpCommon\Math.h"
#include "..\SpCommon\Debug.h"

USING_NAMESPACE_ENGINE;

void ParticleHemiSphereEmitter::Emit(const ParticleDesc& desc, Particle& particle)
{
	particle.m_Angle = desc.m_StartAngle.get();
	particle.m_Life = desc.m_Life.get();
	particle.m_StartSize = particle.m_Size = desc.m_Size.get();
	particle.m_Velocity = CMath::RandomNormalizedVec3() * desc.m_Speed.get();
	particle.m_Velocity.y = CMath::Abs(particle.m_Velocity.y);
	particle.m_Acceleration = CMath::RandomNormalizedVec3() * desc.m_AccelSpeed.get();
	particle.m_Acceleration.y = CMath::Abs(particle.m_Acceleration.y);
	particle.m_Position = Vector3::zero;
	particle.m_ElapsedTime = 0.0f;
	particle.m_RandomSeed = CMath::Random(0.0f, 1000.0f);
}