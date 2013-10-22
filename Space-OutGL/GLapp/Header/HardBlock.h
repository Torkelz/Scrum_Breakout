#ifndef HARDBLOCK_H
#define HARDBLOCK_H

#include "ABlock.h"

class HardBlock : public ABlock
{
public:
	HardBlock(vec3* p_pPos, vec3* p_pColor, unsigned int p_blockType, std::string p_objectName, vec2 p_blockID);
	~HardBlock();

	void init(mat4* p_pRotMat);
	void update();

private:

};

#endif 
