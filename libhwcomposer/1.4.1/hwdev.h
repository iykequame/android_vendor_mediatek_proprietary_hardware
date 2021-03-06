#ifndef HWC_HWDEV_H_
#define HWC_HWDEV_H_

#include <ui/Rect.h>
#include <utils/Singleton.h>
#include <utils/threads.h>

#include <linux/disp_session.h>

#include "idevice.h"
#include "display.h"

#define DISP_NO_PRESENT_FENCE  ((int)(~0U>>1))

using namespace android;

class MTKM4UDrv;

// ---------------------------------------------------------------------------

class DispDevice : public IDevice, public Singleton<DispDevice>
{
public:
    DispDevice();
    ~DispDevice();

    // initOverlay() initializes overlay related hw setting
    void initOverlay();

    // getMaxOverlayInputNum() gets overlay supported input amount
    int getMaxOverlayInputNum();

    // createOverlaySession() creates overlay composition session
    status_t createOverlaySession(
        int dpy, int mode);

    // destroyOverlaySession() destroys overlay composition session
    void destroyOverlaySession(int dpy);

    // truggerOverlaySession() used to trigger overlay engine to do composition
    status_t triggerOverlaySession(int dpy, int present_fence_idx, DISP_DC_TYPE type);

    // disableOverlaySession() usd to disable overlay session to do composition
    void disableOverlaySession(int dpy,  OverlayPortParam* const* params, int num);

    // setOverlaySessionMode() sets the overlay session mode
    status_t setOverlaySessionMode(int dpy, int mode);

    // getOverlaySessionMode() gets the overlay session mode
    int getOverlaySessionMode(int dpy);

    // getOverlaySessionInfo() gets specific display device information
    status_t getOverlaySessionInfo(int dpy, disp_session_info* info);

    // getAvailableOverlayInput gets available amount of overlay input
    // for different session
    int getAvailableOverlayInput(int dpy);

    // prepareOverlayInput() gets timeline index and fence fd of overlay input layer
    void prepareOverlayInput(int dpy, OverlayPrepareParam* param);

    // enableOverlayInput() enables single overlay input layer
    void enableOverlayInput(int dpy, OverlayPortParam* param, int id);

    // updateOverlayInputs() updates multiple overlay input layers
    void updateOverlayInputs(int dpy, OverlayPortParam* const* params, int num);

    // prepareOverlayOutput() gets timeline index and fence fd for overlay output buffer
    void prepareOverlayOutput(int dpy, OverlayPrepareParam* param);

    // enableOverlayOutput() enables overlay output buffer
    void enableOverlayOutput(int dpy, OverlayPortParam* param);

    // prepareOverlayPresentFence() gets present timeline index and fence
    void prepareOverlayPresentFence(int dpy, OverlayPrepareParam* param);

    // waitVSync() is used to wait vsync signal for specific display device
    status_t waitVSync(int dpy, nsecs_t *ts);

    // setPowerMode() is used to switch power setting for display
    void setPowerMode(int dpy, int mode);

    // toString returns the string literal of session mode
    static char const* toString(DISP_MODE mode);

    // query hw capabilities through ioctl and store in m_caps_info
    status_t setCapsInfo();

    // return m_caps_info to who uses this DispDevice
    inline DISP_CAP_OUTPUT_MODE getCapabilitiesMode() { return m_caps_info.output_mode; }
    inline DISP_CAP_OUTPUT_PASS getCapabilitiesPass() { return m_caps_info.output_pass; }

private:
    enum
    {
        DISP_INVALID_SESSION = -1,
    };

    int m_dev_fd;

    int m_ovl_input_num;

    disp_session_input_config m_input_config[DisplayManager::MAX_DISPLAYS];

    DISP_MODE m_session_mode[DisplayManager::MAX_DISPLAYS];

    disp_caps_info m_caps_info;

    buffer_handle_t m_decouple_buf[3];
};

// --------------------------------------------------------------------------

class MMUDevice : public Singleton<MMUDevice>
{
public:
    MMUDevice();
    ~MMUDevice();

    // enable client
    int enable(int client);

    // map mva
    int map(int client, unsigned int vaddr, int size, unsigned int* mva);

    // unmap mva
    int unmap(int client, unsigned int vaddr, int size, unsigned int mva);

    // flush memory
    int flush(int client, unsigned int vaddr, int size);

    // config to enable or disable port
    int config(int port, bool enabled);

private:
    MTKM4UDrv* m_dev;
};

// --------------------------------------------------------------------------

class GrallocDevice : public Singleton<GrallocDevice>
{
public:
    GrallocDevice();
    ~GrallocDevice();

    struct AllocParam
    {
        AllocParam()
            : width(0), height(0), format(0)
            , usage(0), handle(NULL), stride(0)
        { }

        unsigned int width;
        unsigned int height;
        int format;
        int usage;

        buffer_handle_t handle;
        int stride;
    };

    // allocate memory by gralloc driver
    status_t alloc(AllocParam& param);

    // free a previously allocated buffer
    status_t free(buffer_handle_t handle);

    // dump information of allocated buffers
    void dump() const;

private:
    alloc_device_t* m_dev;
};

#endif // HWC_HWDEV_H_
