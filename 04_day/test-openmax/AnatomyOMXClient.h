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

#ifndef ANATOMY_OMXCLIENT_H_
#define ANATOMY_OMXCLIENT_H_

#include "ATMParser.h"
#include "StdoutRenderer.h"
#include "OMX_Types.h"
#include "OMX_Core.h"
#include "OMX_Component.h"

#include <utils/threads.h>
#include <utils/Vector.h>

namespace anatomy {

struct AnatomyOMXClient {
public:
	AnatomyOMXClient(){}
	virtual ~AnatomyOMXClient(){}
	void prepare();
	void start() { excute(); waitEOF();}
	void stop();

private:
	void waitEOF() { mEOSCondition.wait(mLock); }
	void changeState(OMX_STATETYPE state);
	void allocateBuffers();
	void freeBuffers();
	void init();
	void deinit();
	void excute();
	void waitIdleState() { mIdleCondition.wait(mLock); }
	void waitExcutingState() { mExcutingCondition.wait(mLock);}
	void waitLoadedState() { mLoadedCondition.wait(mLock);}

	void render(void* buffer,int length){mRenderer.render(buffer, length);}
	void read(void* buffer,int* length){mParser.read(buffer, length);}

	void signalEOF() { mEOSCondition.signal(); }
	void signalIdleState() { mIdleCondition.signal(); }
	void signalExcutingState() { mExcutingCondition.signal(); }
	void signalLoadedState() { mLoadedCondition.signal(); }

	void addOMXBufferInfo(OMX_U32 portIndex, OMX_BUFFERHEADERTYPE* header);
	void removeOMXBufferInfo(OMX_U32 portIndex, OMX_BUFFERHEADERTYPE* header);

	void setStatus(int status){ mStatus = status; }
	int getStatus(){ return mStatus; }

	static OMX_CALLBACKTYPE kCallbacks;

	android::Condition mEOSCondition;
	android::Condition mExcutingCondition;
	android::Condition mIdleCondition;
	android::Condition mLoadedCondition;
	android::Mutex mLock;
	ATMParser mParser;
	StdoutRenderer mRenderer;
	OMX_HANDLETYPE mComponentHandle;
	int mStatus;

    enum {
        kPortIndexInput  = 0,
        kPortIndexOutput = 1
    };

	 struct OMXClientBufferInfo {
        OMX_U32 mPortIndex;
        OMX_BUFFERHEADERTYPE* mBufferHeader;
    };

    android::Vector<OMXClientBufferInfo> mBufferInfo;

	static OMX_ERRORTYPE OnEvent(
	    OMX_IN OMX_HANDLETYPE hComponent,
	    OMX_IN OMX_PTR pAppData,
	    OMX_IN OMX_EVENTTYPE eEvent,
	    OMX_IN OMX_U32 nData1,
	    OMX_IN OMX_U32 nData2,
	    OMX_IN OMX_PTR pEventData);

	static OMX_ERRORTYPE OnEmptyBufferDone(
	    OMX_IN OMX_HANDLETYPE hComponent,
	    OMX_IN OMX_PTR pAppData,
	    OMX_IN OMX_BUFFERHEADERTYPE *pBuffer);


	static OMX_ERRORTYPE OnFillBufferDone(
	    OMX_IN OMX_HANDLETYPE hComponent,
	    OMX_IN OMX_PTR pAppData,
	    OMX_IN OMX_BUFFERHEADERTYPE *pBuffer);
};

}  // namespace anatomy

#endif  // ANATOMY_OMXCLIENT_H_

