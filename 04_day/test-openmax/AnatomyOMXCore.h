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

#ifndef ANATOMYOMXCORE_H_
#define ANATOMYOMXCORE_H_

#include "OMX_Types.h"
#include "OMX_Core.h"
#include "OMX_Component.h"

OMX_API OMX_ERRORTYPE OMX_APIENTRY
OMX_Init();

OMX_API OMX_ERRORTYPE OMX_APIENTRY
OMX_Deinit();

OMX_API OMX_ERRORTYPE OMX_APIENTRY
OMX_GetHandle(OMX_OUT OMX_HANDLETYPE*     handle,
              OMX_IN OMX_STRING    componentName,
              OMX_IN OMX_PTR             appData,
              OMX_IN OMX_CALLBACKTYPE* callBacks);

OMX_API OMX_ERRORTYPE OMX_APIENTRY
OMX_FreeHandle(OMX_IN OMX_HANDLETYPE hComp);

OMX_API OMX_ERRORTYPE OMX_APIENTRY
OMX_SetupTunnel(OMX_IN OMX_HANDLETYPE outputComponent,
                OMX_IN OMX_U32             outputPort,
                OMX_IN OMX_HANDLETYPE  inputComponent,
                OMX_IN OMX_U32              inputPort);

OMX_API OMX_ERRORTYPE
OMX_GetContentPipe(OMX_OUT OMX_HANDLETYPE* pipe,
                   OMX_IN OMX_STRING        uri);

OMX_API OMX_ERRORTYPE OMX_APIENTRY
OMX_ComponentNameEnum(OMX_OUT OMX_STRING componentName,
                      OMX_IN  OMX_U32          nameLen,
                      OMX_IN  OMX_U32            index);

OMX_API OMX_ERRORTYPE
OMX_GetComponentsOfRole(OMX_IN OMX_STRING      role,
                        OMX_INOUT OMX_U32* numComps,
                        OMX_INOUT OMX_U8** compNames);

OMX_API OMX_ERRORTYPE
OMX_GetRolesOfComponent(OMX_IN OMX_STRING compName,
                        OMX_INOUT OMX_U32* numRoles,
                        OMX_OUT OMX_U8** roles);

#endif /* ANATOMYOMXCORE_H_ */
