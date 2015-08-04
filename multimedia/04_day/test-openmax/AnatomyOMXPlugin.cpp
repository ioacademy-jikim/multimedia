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
#define LOG_TAG "AnatomyOMXPlugin"
#include <utils/Log.h>

#include "AnatomyOMXPlugin.h"
#include <include/SoftOMXComponent.h>

#include <media/stagefright/foundation/ADebug.h>
#include <media/stagefright/foundation/AString.h>

#include <dlfcn.h>

#define LOGV ALOGV
#define LOGE ALOGE


namespace android {

static const struct {
    const char *mName;
    const char *mLibNameSuffix;
    const char *mRole;

} kComponents[] = {
    { "OMX.anatomy.atm.decoder", "atmdec", "ascii_decoder.atm" },
};

static const size_t kNumComponents =
    sizeof(kComponents) / sizeof(kComponents[0]);

AnatomyOMXPlugin::AnatomyOMXPlugin() {
}

OMX_ERRORTYPE AnatomyOMXPlugin::makeComponentInstance(
        const char *name,
        const OMX_CALLBACKTYPE *callbacks,
        OMX_PTR appData,
        OMX_COMPONENTTYPE **component) {
    LOGV("makeComponentInstance '%s'", name);

    for (size_t i = 0; i < kNumComponents; ++i) {
        if (strcmp(name, kComponents[i].mName)) {
            continue;
        }

        AString libName = "libanatomy_";
        libName.append(kComponents[i].mLibNameSuffix);
        libName.append(".so");

        void *libHandle = dlopen(libName.c_str(), RTLD_NOW);

        if (libHandle == NULL) {
            LOGE("unable to dlopen %s", libName.c_str());

            return OMX_ErrorComponentNotFound;
        }

        typedef SoftOMXComponent *(*CreateSoftOMXComponentFunc)(
                const char *, const OMX_CALLBACKTYPE *,
                OMX_PTR, OMX_COMPONENTTYPE **);

        CreateSoftOMXComponentFunc createSoftOMXComponent =
            (CreateSoftOMXComponentFunc)dlsym(
                    libHandle,
                    "_Z22createSoftOMXComponentPKcPK16OMX_CALLBACKTYPE"
                    "PvPP17OMX_COMPONENTTYPE");

        if (createSoftOMXComponent == NULL) {
            dlclose(libHandle);
            libHandle = NULL;

            return OMX_ErrorComponentNotFound;
        }

        sp<SoftOMXComponent> codec =
            (*createSoftOMXComponent)(name, callbacks, appData, component);

        if (codec == NULL) {
            dlclose(libHandle);
            libHandle = NULL;

            return OMX_ErrorInsufficientResources;
        }

        OMX_ERRORTYPE err = codec->initCheck();
        if (err != OMX_ErrorNone) {
            dlclose(libHandle);
            libHandle = NULL;

            return err;
        }

        codec->incStrong(this);
        codec->setLibHandle(libHandle);

        return OMX_ErrorNone;
    }

    return OMX_ErrorInvalidComponentName;
}

OMX_ERRORTYPE AnatomyOMXPlugin::destroyComponentInstance(
        OMX_COMPONENTTYPE *component) {
    SoftOMXComponent *me =
        (SoftOMXComponent *)
            ((OMX_COMPONENTTYPE *)component)->pComponentPrivate;

    me->prepareForDestruction();

    void *libHandle = me->libHandle();

    CHECK_EQ(me->getStrongCount(), 1);
    me->decStrong(this);
    me = NULL;

    dlclose(libHandle);
    libHandle = NULL;

    return OMX_ErrorNone;
}

OMX_ERRORTYPE AnatomyOMXPlugin::enumerateComponents(
        OMX_STRING name,
        size_t size,
        OMX_U32 index) {
    if (index >= kNumComponents) {
        return OMX_ErrorNoMore;
    }

    strcpy(name, kComponents[index].mName);

    return OMX_ErrorNone;
}

OMX_ERRORTYPE AnatomyOMXPlugin::getRolesOfComponent(
        const char *name,
        Vector<String8> *roles) {
    for (size_t i = 0; i < kNumComponents; ++i) {
        if (strcmp(name, kComponents[i].mName)) {
            continue;
        }

        roles->clear();
        roles->push(String8(kComponents[i].mRole));

        return OMX_ErrorNone;
    }

    return OMX_ErrorInvalidComponentName;
}

}  // namespace android

