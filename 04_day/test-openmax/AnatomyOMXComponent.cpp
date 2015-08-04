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
#define LOG_TAG "AnatomyOMXComponent"
#include <utils/Log.h>

#include "AnatomyOMXComponent.h"

#include <media/stagefright/foundation/ADebug.h>
#include <media/stagefright/MediaDefs.h>
#include <media/stagefright/MediaErrors.h>
#include <media/IOMX.h>

#define LOGD ALOGD

namespace android {


template<class T>
static void InitOMXParams(T *params) {
    params->nSize = sizeof(T);
    params->nVersion.s.nVersionMajor = 1;
    params->nVersion.s.nVersionMinor = 0;
    params->nVersion.s.nRevision = 0;
    params->nVersion.s.nStep = 0;
}

AnatomyOMXComponent::AnatomyOMXComponent(
        const char *name,
        const OMX_CALLBACKTYPE *callbacks,
        OMX_PTR appData,
        OMX_COMPONENTTYPE **component)
    : SimpleSoftOMXComponent(name, callbacks, appData, component) {
    LOGD("anatomy component created");
    initPorts();
}

AnatomyOMXComponent::~AnatomyOMXComponent() {
    List<BufferInfo *> &outQueue = getPortQueue(kOutputPortIndex);
    List<BufferInfo *> &inQueue = getPortQueue(kInputPortIndex);
    CHECK(outQueue.empty());
    CHECK(inQueue.empty());
}

void AnatomyOMXComponent::initPorts() {
    OMX_PARAM_PORTDEFINITIONTYPE def;
    InitOMXParams(&def);

    def.nPortIndex = kInputPortIndex;
    def.eDir = OMX_DirInput;
    def.nBufferCountMin = kNumInputBuffers;
    def.nBufferCountActual = def.nBufferCountMin;
    def.nBufferSize = 2048;
    def.bEnabled = OMX_TRUE;
    def.bPopulated = OMX_FALSE;
    def.eDomain = OMX_PortDomainOther;
    def.bBuffersContiguous = OMX_FALSE;
    def.nBufferAlignment = 1;//TODO

    def.format.other.eFormat = OMX_OTHER_FormatVendorStartUnused; //TODO

    addPort(def);

    def.nPortIndex = kOutputPortIndex;
    def.eDir = OMX_DirOutput;
    def.nBufferCountMin = kNumOutputBuffers;
    def.nBufferCountActual = def.nBufferCountMin;
	def.nBufferSize = 2048;
    def.bEnabled = OMX_TRUE;
    def.bPopulated = OMX_FALSE;
    def.eDomain = OMX_PortDomainOther;
    def.bBuffersContiguous = OMX_FALSE;
    def.nBufferAlignment = 1;

    def.format.other.eFormat = OMX_OTHER_FormatVendorStartUnused; //TODO

    addPort(def);
}

void AnatomyOMXComponent::onQueueFilled(OMX_U32 portIndex) {
    List<BufferInfo *> &inQueue = getPortQueue(kInputPortIndex);
    List<BufferInfo *> &outQueue = getPortQueue(kOutputPortIndex);

	LOGD("onQueueFilled called");
	while (!inQueue.empty() && !outQueue.empty()) {
        BufferInfo *inInfo = *inQueue.begin();
        OMX_BUFFERHEADERTYPE *inHeader = inInfo->mHeader;

        BufferInfo *outInfo = *outQueue.begin();
        OMX_BUFFERHEADERTYPE *outHeader = outInfo->mHeader;


        if (inHeader->nFlags & OMX_BUFFERFLAG_EOS) {
            inQueue.erase(inQueue.begin());
            inInfo->mOwnedByUs = false;
            notifyEmptyBufferDone(inHeader);

            outHeader->nFilledLen = 0;
            outHeader->nFlags = OMX_BUFFERFLAG_EOS;

            outQueue.erase(outQueue.begin());
            outInfo->mOwnedByUs = false;
            notifyFillBufferDone(outHeader);
            return;
        }

		OMX_U32 decoded_size;
		decodeBuffer(outHeader->pBuffer, inHeader->pBuffer, inHeader->nSize, &decoded_size);
		usleep(10000);
		outHeader->nSize = decoded_size;

        inQueue.erase(inQueue.begin());
        inInfo->mOwnedByUs = false;
		inInfo = NULL;
        notifyEmptyBufferDone(inHeader);
		inHeader = NULL;

        outQueue.erase(outQueue.begin());
        outInfo->mOwnedByUs = false;
		outInfo = NULL;
        notifyFillBufferDone(outHeader);
		outHeader = NULL;
 	  }

}
void AnatomyOMXComponent::decodeBuffer(OMX_U8* dst, OMX_U8* src, OMX_U32 size, OMX_U32* decoded_size) {
	char ascii;
	char* ascii_length;
	int length = 0;
	ascii = src[0];
	length = atoi((char *)&src[1]);
	LOGD("decode value %c, %d",ascii,length);
	*decoded_size = length;
	for(int i = 0; i < length; i++)
	{
		dst[i] = ascii;
	}
}

}  // namespace android
android::SoftOMXComponent *createSoftOMXComponent(
        const char *name, const OMX_CALLBACKTYPE *callbacks,
        OMX_PTR appData, OMX_COMPONENTTYPE **component) {
    return new android::AnatomyOMXComponent(name, callbacks, appData, component);
}

