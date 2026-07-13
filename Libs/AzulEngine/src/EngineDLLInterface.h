#ifndef AZUL_ENGINE_DLL_INTERFACE_H
#define AZUL_ENGINE_DLL_INTERFACE_H

// Export/import switch for the AzulEngine DLL.
// Mirrors the pattern used by Libs/Math and Libs/AnimTime.
//   - AzulEngine.vcxproj defines AZUL_ENGINE_LIBRARY_EXPORTS  -> dllexport
//   - Consumers (Game / Engine app) define only AZUL_ENGINE_USE_DLL -> dllimport
// Tag public engine classes/functions with AZUL_ENGINE_LIBRARY_API.

#ifdef AZUL_ENGINE_USE_DLL
#ifdef AZUL_ENGINE_LIBRARY_EXPORTS
#define AZUL_ENGINE_LIBRARY_API __declspec(dllexport)
#else
#define AZUL_ENGINE_LIBRARY_API __declspec(dllimport)
#endif
#else
#define AZUL_ENGINE_LIBRARY_API
#endif

#endif

// --- End of File ---
