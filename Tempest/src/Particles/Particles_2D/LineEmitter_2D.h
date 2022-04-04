


#ifndef LINE_EMITTER_2D_H
#define LINE_EMITTER_2D_H

#include "Emitter_2D.h"

struct LineEmitter_2D : public Emitter_2D
{
	glm::vec2 m_startPoint, m_endPoint;

	void Emit(const int particleAmount) override;

private:
	glm::vec2 RandomPoint();
};

#endif // !LINE_EMITTER_2D_H

