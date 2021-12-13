
#include "config_gotvcore.h"
#if !defined( BUILD_KODIP2P_APP ) && !defined( BUILD_KODIQT_APP ) && !defined( BUILD_GOTV_APP ) 
echo you must define BUILD_KODI_APP 1 in GoTvAppConfig.h to build KodiP2P
echo you must define BUILD_GOTV_APP 0 in GoTvAppConfig.h to build KodiP2P
#endif // !BUILD_KODI_APP

