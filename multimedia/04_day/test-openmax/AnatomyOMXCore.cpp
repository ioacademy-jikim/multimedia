/*
**
** Copyright 2013, kod21236@gmail.com
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#include "AnatomyOMXCore.h"

#include <utils/Log.h>

#include <include/SimpleSoftOMXComponent.h>

//#include <media/stagefright/foundation/ADebug.h>
//#include <media/stagefright/foundation/AString.h>

#include <dlfcn.h>

#define LOGD ALOGD
#define LOGV ALOGV

using namespace android;

static const struct {
    const char *mName;
    const char *mLibName;
    const char *mRole;

} kComponentInfo[] = {
	{"OMX.anatomy.atm.decoder", "libanatomy_atmdec.so", "ascii_decoder.atm" },
};


OMX_API OMX_ERRORTYPE OMX_APIENTRY
OMX_Init()
{
	return OMX_ErrorNone;
}

OMX_API OMX_ERRORTYPE OMX_APIENTRY
OMX_Deinit()
{
	return OMX_ErrorNone;
}

OMX_API OMX_ERRORTYPE OMX_APIENTRY
OMX_GetHandle(OMX_OUT OMX_HANDLETYPE*     handle,
              OMX_IN OMX_STRING    componentName,
              OMX_IN OMX_PTR             appData,
              OMX_IN OMX_CALLBACKTYPE* callBacks)
{
    if (strcmp(componentName, kComponentInfo[0].mName) == 0) {
		AString libName;
        libName.append(kComponentInfo[0].mLibName);


        void *libHandle = dlopen(libName.c_str(), RTLD_NOW);


        typedef SoftOMXComponent *(*CreateSoftOMXComponentFunc)(
                const char *, const OMX_CALLBACKTYPE *,
                OMX_PTR, OMX_COMPONENTTYPE **);

        CreateSoftOMXComponentFunc createSoftOMXComponent =
            (CreateSoftOMXComponentFunc)dlsym(
                    libHandle,
                    "_Z22createSoftOMXComponentPKcPK16OMX_CALLBACKTYPE"
                    "PvPP17OMX_COMPONENTTYPE");

        sp<SoftOMXComponent> codec =
            (*createSoftOMXComponent)(componentName, callBacks, appData, (OMX_COMPONENTTYPE **)handle);


        codec->initCheck();
		codec->incStrong(handle);
        codec->setLibHandle(libHandle);

        return OMX_ErrorNone;
    }

    return OMX_ErrorInvalidComponentName;
}

OMX_API OMX_ERRORTYPE OMX_APIENTRY
OMX_FreeHandle(OMX_IN OMX_HANDLETYPE hComp)
{
	 SoftOMXComponent *me =
        (SoftOMXComponent *)
            ((OMX_COMPONENTTYPE *)hComp)->pComponentPrivate;

    me->prepareForDestruction();

    void *libHandle = me->libHandle();

    me = NULL;

    dlclose(libHandle);
    libHandle = NULL;

    return OMX_ErrorNone;
}

OMX_API OMX_ERRORTYPE OMX_APIENTRY
OMX_ComponentNameEnum(OMX_OUT OMX_STRING componentName,
                      OMX_IN  OMX_U32          nameLen,
                      OMX_IN  OMX_U32            index)
{
	if (index >= 1 ) {
        return OMX_ErrorNoMore;
    }

    strncpy(componentName, kComponentInfo[0].mName, nameLen);

    return OMX_ErrorNone;
}

OMX_API OMX_ERRORTYPE OMX_APIENTRY
OMX_SetupTunnel(OMX_IN OMX_HANDLETYPE outputComponent,
                OMX_IN OMX_U32             outputPort,
                OMX_IN OMX_HANDLETYPE  inputComponent,
                OMX_IN OMX_U32              inputPort)
{
	 return OMX_ErrorNotImplemented;
}

OMX_API OMX_ERRORTYPE
OMX_GetContentPipe(OMX_OUT OMX_HANDLETYPE* pipe,
                   OMX_IN OMX_STRING        uri)
{
	 return OMX_ErrorNotImplemented;
}



OMX_API OMX_ERRORTYPE
OMX_GetComponentsOfRole(OMX_IN OMX_STRING      role,
                        OMX_INOUT OMX_U32* numComps,
                        OMX_INOUT OMX_U8** compNames)
{
	return OMX_ErrorNotImplemented;
}

OMX_API OMX_ERRORTYPE
OMX_GetRolesOfComponent(OMX_IN OMX_STRING compName,
                        OMX_INOUT OMX_U32* numRoles,
                        OMX_OUT OMX_U8** roles)
{
	 return OMX_ErrorNotImplemented;
}
