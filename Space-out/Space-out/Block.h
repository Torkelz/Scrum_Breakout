#ifndef BLOCK_H
#define BLOCK_H

#include "ABlock.h"

class Block : public ABlock
{
public:
	Block(vec3* p_pPos, vec3* p_pColor, std::string p_objectName, int p_blockID);
	~Block();

	void init();
	void update();

private:

};

#endif 
