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

//#define LOG_NDEBUG 0
#define LOG_TAG "AnatomyOMXClient"

#include "AnatomyOMXClient.h"
#include "AnatomyOMXCore.h"

#include <media/stagefright/foundation/ADebug.h>
#include <utils/Log.h>

#define LOGW ALOGW
#define LOGD ALOGD


namespace anatomy {


template<class T>
static void InitOMXParams(T *params) {
    params->nSize = sizeof(T);
    params->nVersion.s.nVersionMajor = 1;
    params->nVersion.s.nVersionMinor = 0;
    params->nVersion.s.nRevision = 0;
    params->nVersion.s.nStep = 0;
}

void AnatomyOMXClient::prepare() {
	init();

	changeState(OMX_StateIdle);
	allocateBuffers();
	sleep(1);

	changeState(OMX_StateExecuting);
	sleep(1);
}

void AnatomyOMXClient::stop() {
	changeState(OMX_StateIdle);
	sleep(1);
	changeState(OMX_StateLoaded);
	freeBuffers();
	sleep(1);

	deinit();
}

void AnatomyOMXClient::init() {
	OMX_HANDLETYPE handle = NULL;
	char componentName[256];
	OMX_Init();
	OMX_ComponentNameEnum(componentName, 256, 0);
	OMX_GetHandle(&handle, componentName, this, &AnatomyOMXClient::kCallbacks);
	CHECK(handle != NULL);

	mComponentHandle = handle;
}
void AnatomyOMXClient::changeState(OMX_STATETYPE state) {
	OMX_SendCommand(mComponentHandle, OMX_CommandStateSet, state, NULL);
}
void AnatomyOMXClient::allocateBuffers() {
    OMX_PARAM_PORTDEFINITIONTYPE def;
    InitOMXParams(&def);
    def.nPortIndex = kPortIndexInput;

    OMX_GetParameter(mComponentHandle, OMX_IndexParamPortDefinition, &def);

	OMX_BUFFERHEADERTYPE *pInputHeader;
	for(uint i = 0; i < def.nBufferCountActual; i++ )
	{
		OMX_AllocateBuffer(mComponentHandle, &pInputHeader, 0, this, def.nBufferSize);
		addOMXBufferInfo(kPortIndexInput, pInputHeader);
	}

    InitOMXParams(&def);
    def.nPortIndex = kPortIndexOutput;

    OMX_GetParameter(mComponentHandle, OMX_IndexParamPortDefinition, &def);
	OMX_BUFFERHEADERTYPE *pOutputHeader;
	for(uint i = 0; i < def.nBufferCountActual; i++ )
	{
			OMX_AllocateBuffer(mComponentHandle, &pOutputHeader, 1, this, def.nBufferSize);
			addOMXBufferInfo(kPortIndexOutput, pOutputHeader);
	}
}

void AnatomyOMXClient::excute() {
	int nSize = 0;
	OMX_BUFFERHEADERTYPE *pInputHeader = NULL;
	OMX_BUFFERHEADERTYPE *pOutputHeader = NULL;

	for(size_t i = 0; i < mBufferInfo.size(); i++) {
		if( mBufferInfo[i].mPortIndex == kPortIndexInput ) {
			pInputHeader = mBufferInfo[i].mBufferHeader;
			read((char*)(pInputHeader->pBuffer), &nSize);
			OMX_EmptyThisBuffer(mComponentHandle, pInputHeader);
			removeOMXBufferInfo(kPortIndexInput, mBufferInfo[i].mBufferHeader);
		}
		if( mBufferInfo[i].mPortIndex == kPortIndexOutput ) {
			pOutputHeader = mBufferInfo[i].mBufferHeader;
			OMX_FillThisBuffer(mComponentHandle, pOutputHeader);
			removeOMXBufferInfo(kPortIndexOutput, mBufferInfo[i].mBufferHeader);
		}
	}
}
void AnatomyOMXClient::deinit() {
	OMX_FreeHandle(mComponentHandle);
	OMX_Deinit();
}

void AnatomyOMXClient::addOMXBufferInfo(
		OMX_U32 portIndex, OMX_BUFFERHEADERTYPE* header) {
    OMXClientBufferInfo info;
    info.mPortIndex = portIndex;
    info.mBufferHeader = header;
    mBufferInfo.push(info);
}

void AnatomyOMXClient::removeOMXBufferInfo(
        OMX_U32 portIndex, OMX_BUFFERHEADERTYPE* header) {
    bool found = false;
    for (size_t i = 0; i < mBufferInfo.size(); i++) {
        if (mBufferInfo[i].mPortIndex == portIndex
            && mBufferInfo[i].mBufferHeader == header) {
            found = true;
            mBufferInfo.removeItemsAt(i);
            break;
        }
    }

    if (!found) {
        LOGW("Attempt to remove an active buffer we know nothing about...");
    }
}

void AnatomyOMXClient::freeBuffers() {
    LOGD("=============================================");
    for (size_t i = mBufferInfo.size(); i--;) {
		int port_index= mBufferInfo[i].mPortIndex;
		OMX_BUFFERHEADERTYPE *header = mBufferInfo[i].mBufferHeader;
		LOGD("mBufferInfo[%d] mPortIndex : %d, mBufferHeader %p",i, port_index, header);
    	removeOMXBufferInfo(port_index, header);
    	OMX_FreeBuffer(mComponentHandle, port_index, header);
    }
    LOGD("=============================================");
}


// static
OMX_CALLBACKTYPE AnatomyOMXClient::kCallbacks = {
    &OnEvent, &OnEmptyBufferDone, &OnFillBufferDone
};

OMX_ERRORTYPE AnatomyOMXClient::OnFillBufferDone(
        OMX_IN OMX_HANDLETYPE hComponent,
        OMX_IN OMX_PTR pAppData,
        OMX_IN OMX_BUFFERHEADERTYPE *pBuffer) {

	AnatomyOMXClient* omxclient = reinterpret_cast<AnatomyOMXClient*>(pAppData);
	CHECK( NULL != omxclient);
	CHECK( NULL != pBuffer );

	omxclient->addOMXBufferInfo(kPortIndexOutput, pBuffer);

	if( pBuffer->nFlags & OMX_BUFFERFLAG_EOS || omxclient->getStatus() == 1 )
	{
		omxclient->signalEOF();
		omxclient->setStatus(1);
		return OMX_ErrorNone;
	}

	if( pBuffer->nSize > 0 )
	{
		omxclient->render(pBuffer->pBuffer, pBuffer->nSize);
	}

	OMX_FillThisBuffer(hComponent, pBuffer);
	omxclient->removeOMXBufferInfo(kPortIndexOutput, pBuffer);
	return OMX_ErrorNone;
}

OMX_ERRORTYPE AnatomyOMXClient::OnEmptyBufferDone(
	    OMX_IN OMX_HANDLETYPE hComponent,
	    OMX_IN OMX_PTR pAppData,
	    OMX_IN OMX_BUFFERHEADERTYPE *pBuffer) {

	AnatomyOMXClient* omxclient = reinterpret_cast<AnatomyOMXClient*>(pAppData);
	int nSize = 0;
	omxclient->addOMXBufferInfo(kPortIndexInput, pBuffer);
	CHECK(NULL != omxclient);
	CHECK( NULL != pBuffer );

	if( pBuffer->nFlags & OMX_BUFFERFLAG_EOS )
	{
		return OMX_ErrorNone;
	}

	omxclient->read(pBuffer->pBuffer, &nSize);

	if( nSize <= 0 ) {
		pBuffer->nSize = 0;
		pBuffer->nFlags |= OMX_BUFFERFLAG_EOS;
	}
	else {
		pBuffer->nSize = nSize;
	}

	OMX_EmptyThisBuffer(hComponent, pBuffer);
	omxclient->removeOMXBufferInfo(kPortIndexInput, pBuffer);
	return OMX_ErrorNone;
}

OMX_ERRORTYPE AnatomyOMXClient::OnEvent(
	    OMX_IN OMX_HANDLETYPE hComponent,
	    OMX_IN OMX_PTR pAppData,
	    OMX_IN OMX_EVENTTYPE eEvent,
	    OMX_IN OMX_U32 nData1,
	    OMX_IN OMX_U32 nData2,
	    OMX_IN OMX_PTR pEventData) {
	AnatomyOMXClient* omxclient = reinterpret_cast<AnatomyOMXClient*>(pAppData);
	switch(eEvent)
	{
		case OMX_EventCmdComplete:
			if (OMX_CommandStateSet == nData1)
            {
           		switch (nData2)
                {
                    case OMX_StateLoaded:
						omxclient->signalLoadedState();
                        break;
                    case OMX_StateIdle:
						omxclient->signalIdleState();
                        break;
                    case OMX_StateExecuting:
						omxclient->signalExcutingState();
                        break;
           		}
            }
			break;
		case OMX_EventError:
		default:
			break;
	}
	return OMX_ErrorNone;
}
}

