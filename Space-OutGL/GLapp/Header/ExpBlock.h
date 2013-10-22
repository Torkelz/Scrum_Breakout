#ifndef EXPBLOCK_H
#define EXPBLOCH_H

#include "ABlock.h"

class ExpBlock : public ABlock
{
public:
	ExpBlock(vec3* p_pPos, vec3* p_pColor, unsigned int p_blockType, std::string p_objectName, vec2 p_blockID);
	~ExpBlock();

	void init(mat4* p_pRotMat);
	void update();

private:

};

#endif 
