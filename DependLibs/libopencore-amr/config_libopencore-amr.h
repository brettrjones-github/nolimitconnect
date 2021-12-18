#pragma once
#include <AppDependLibrariesConfig.h>
#include <libopencore-amr/oscl/oscl_base_macros.h>
#include <libopencore-amr/oscl/oscl_base.h>
#include <libopencore-amr/oscl/oscl_mem.h>

#define OSCL_ARRAY_DELETE oscl_free
#define OSCL_ARRAY_NEW( typeOfObj, cntObjects ) (typeOfObj *)oscl_malloc( sizeof(typeOfObj) * cntObjects )