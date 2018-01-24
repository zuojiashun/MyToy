#ifndef _FONT_MANAGER_H_
#define _FONT_MANAGER_H_

#include"TrueTypeFont.h"
#include"SpCommon\Singleton.h"

BEGIN_NAMESPACE_ENGINE

#define _LoadFont(fontId, file_name) CFontManager::GetInstance()->LoadFont(fontId, file_name)
#define _GetFont(fontId) CFontManager::GetInstance()->GetFont(fontId)
#define FontManager CFontManager::GetInstance()

class CFontManager : public CSingleton<CFontManager>
{
private:
	map<int, CTrueTypeFont*> fontMap;

public:
	CTrueTypeFont* LoadFont(int fontId, const char* file_name);

	CTrueTypeFont* GetFont(int fontId);
};

END_NAMESPACE_ENGINE

#endif