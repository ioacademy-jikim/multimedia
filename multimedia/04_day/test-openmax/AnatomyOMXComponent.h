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

#ifndef ANATOMY_OMX_COMPONENT_H_

#define ANATOMY_OMX_COMPONENT_H_

#include <include/SimpleSoftOMXComponent.h>
#include <utils/KeyedVector.h>

namespace android {

struct AnatomyOMXComponent : public SimpleSoftOMXComponent {
    AnatomyOMXComponent(const char *name,
            const OMX_CALLBACKTYPE *callbacks,
            OMX_PTR appData,
            OMX_COMPONENTTYPE **component);

protected:
    virtual ~AnatomyOMXComponent();
    virtual void onQueueFilled(OMX_U32 portIndex);


private:
	 enum {
        kInputPortIndex   = 0,
        kOutputPortIndex  = 1,
        kNumInputBuffers  = 2,
        kNumOutputBuffers = 5,
    };
    void initPorts();
	void decodeBuffer(OMX_U8* dst, OMX_U8* src, OMX_U32 size, OMX_U32* decoded_size);

    DISALLOW_EVIL_CONSTRUCTORS(AnatomyOMXComponent);
};

}  // namespace android

#endif  // ANATOMY_OMX_COMPONENT_H_

