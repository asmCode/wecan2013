#pragma once

#include "Spark.h"
#include <Math\Vec3.h>
#include <Math\Matrix.h>
#include <stdint.h>

class SparksGenerator
{
public:
	SparksGenerator(void);
	~SparksGenerator(void);

	void Update(float seconds);
	void Draw(float seconds,
		const sm::Matrix &proj,
		const sm::Matrix &view);

	void SetSourcePosition(const sm::Vec3 &position);

	void EnableSparksSource();
	void DisableSparksSource();

private:
	static const uint32_t MaxSparks = 1000;
	static const uint32_t SparksPerSecond = 400;

	bool m_sparksSourceEnabled;

	sm::Vec3 m_sourcePosition;

	uint32_t m_liveSparksCount;
	float m_sparksToStart;

	Spark m_sparks[MaxSparks];
};

