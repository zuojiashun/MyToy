#ifndef _CMODEL_FILE_
#define _CMODEL_FILE_

#include "Math.h"
#include "SourceFile.h"
#include "SkeletonAnimation.h"

class ENGINE_API CModelFile : public ISourceFile
{
public:
	Model* m_model = NULL;
};

#endif