//--------------------------------------------------------------
// Copyright 2026, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef PUGI_XML_DLL_INTERFACE_H
#define PUGI_XML_DLL_INTERFACE_H

#ifdef PUGI_XML_USE_DLL
#ifdef PUGI_XML_LIBRARY_EXPORTS
#define PUGI_XML_LIBRARY_API __declspec(dllexport)
#define PUGIXML_API __declspec(dllexport)
#else
#define PUGI_XML_LIBRARY_API __declspec(dllimport)
#define PUGIXML_CLASS __declspec(dllimport)
#endif
#else
#define PUGI_XML_LIBRARY_API
#endif

#endif

// --- End of File ---
