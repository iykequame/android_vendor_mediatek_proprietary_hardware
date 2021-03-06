#define LOG_TAG "AudioMTKStreamInClient"

#include "AudioType.h"
#include "AudioMTKStreamInClient.h"

#define MTK_LOG_ENABLE 1
#include <cutils/log.h>

#define AUDIO_MTKSTREAMIN_BUFFER_SIZE (0x8000)
#define AUDIO_MTKSTREAMIN_BUFFER_MAXE (0x10000)

namespace android {

AudioMTKStreamInClient::AudioMTKStreamInClient(uint32 BuffeSize, uint32 clientid) {
    memset((void *)&mRingBuf, 0, sizeof(RingBuf));
    if (BuffeSize == 0  || BuffeSize >  AUDIO_MTKSTREAMIN_BUFFER_MAXE) {
        BuffeSize = AUDIO_MTKSTREAMIN_BUFFER_SIZE;
    }
    mRingBuf.pBufBase = new char[BuffeSize];
    if (mRingBuf.pBufBase  == NULL) {
        ALOGW("mRingBuf.pBufBase allocate fail");
    }
    mRingBuf.bufLen = BuffeSize;
    mRingBuf.pRead = mRingBuf.pBufBase ;
    mRingBuf.pWrite = mRingBuf.pBufBase ;
    mClientId = clientid;
    mEnable = false;

    //================OLD  blisrc ( for platform before mt6582) =====
    mBliHandlerBuffer = NULL;
    mBliOutputLinearBuffer = new char[BuffeSize]; // tmp buffer for blisrc out
    ASSERT(mBliOutputLinearBuffer != NULL);
    //=============================================================//

    mBliSrc = NULL;

    mMemDataType = 0;//by Changqing

    mEnableBesRecord = true;
    mStreamIn = NULL;
    mAttributeClient = NULL;
    mSourceType = 0;
    memset(&mInputStartTime, 0, sizeof(timespec));

    ALOGD("AudioMTKStreamInClient constructor pBufBase = 0x%x  mClientId = %d",
          mRingBuf.pBufBase, mClientId);
}

AudioMTKStreamInClient::~AudioMTKStreamInClient() {
    // old blisrc ( for platform before mt6582)
    if (mBliOutputLinearBuffer) {
        delete[] mBliOutputLinearBuffer;
    }
    //////////////////////////////////////////

    if (mBliSrc) {
        mBliSrc->close();
        delete mBliSrc;
        mBliSrc = NULL;
    }

    if (mRingBuf.pBufBase) {
        delete[] mRingBuf.pBufBase;
    }
    mRingBuf.bufLen = 0;
    mRingBuf.pRead = NULL;
    mRingBuf.pWrite = NULL;
}


}




