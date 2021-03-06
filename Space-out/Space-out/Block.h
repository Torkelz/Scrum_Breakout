#ifndef BLOCK_H
#define BLOCK_H

#include "ABlock.h"

class Block : public ABlock
{
public:
	Block(vec3* p_pPos, vec3* p_pColor, unsigned int p_blockType, std::string p_objectName, vec2 p_blockID);
	~Block();

	void init(mat4* p_pRotMat);
	void update();

private:

};

#endif 
