/* @@@LICENSE
*
*      Copyright (c) 2008-2012 Hewlett-Packard Development Company, L.P.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* LICENSE@@@ */

#ifndef WINDOWPROPERTIES_H
#define WINDOWPROPERTIES_H

struct WindowProperties {
    enum {
        isSetNothing                 = 0,
        isSetBlockScreenTimeout      = 1 << 0,
        isSetSubtleLightbar          = 1 << 1,
        isSetFullScreen              = 1 << 2,
        isSetFastAccelerometer       = 1 << 3,
        isSetOverlayNotifications    = 1 << 4,
        isSetSuppressBannerMessages  = 1 << 5,
        isSetHasPauseUi              = 1 << 6,
        isSetSuppressGestures        = 1 << 7,
        isSetDashboardManualDragMode = 1 << 9,
        isSetStatusBarColor          = 1 << 10,
        isSetRotationLockMaximized   = 1 << 11,
        isSetAllowResizeOnPositiveSpaceChange = 1 << 12,
        isSetEnableCompassEvents     = 1 << 13,
        isSetGyro                    = 1 << 14,
        isSetActiveTouchpanel        = 1 << 15,
        isSetAlsDisabled             = 1 << 16,
        isSetLast
    };

    enum {
        OverlayNotificationsBottom = 0,
        OverlayNotificationsLeft,
        OverlayNotificationsRight,
        OverlayNotificationsTop,
        OverlayNotificationsLast
    };

    unsigned int  flags;

    bool    isBlockScreenTimeout;
    bool    isSubtleLightbar;
    bool    fullScreen;
    bool    activeTouchpanel;
    bool    alsDisabled;
    unsigned int overlayNotificationsPosition;
    bool    suppressBannerMessages;
    bool    hasPauseUi;
    bool    suppressGestures;
    bool    dashboardManualDrag;
    unsigned int dockBrightness;
    unsigned int statusBarColor;
    bool    rotationLockMaximized;
    bool    allowResizeOnPositiveSpaceChange;
    bool    gyroEnabled;
    bool    compassEnabled;

    WindowProperties()
        : flags(0)
        , isBlockScreenTimeout(false)
        , isSubtleLightbar(false)
        , fullScreen(false)
        , activeTouchpanel(false)
        , alsDisabled (false)
        , overlayNotificationsPosition(OverlayNotificationsBottom)
        , suppressBannerMessages(false)
        , hasPauseUi(false)
        , suppressGestures(false)
        , dockBrightness(100)
        , dashboardManualDrag(false)
        , statusBarColor(0x00000000)
        , rotationLockMaximized(false)
        , allowResizeOnPositiveSpaceChange(true)
        , gyroEnabled(false)
        , compassEnabled(false)
    {
    }

    void merge(const WindowProperties& props);

    // convenience functions
    void setBlockScreenTimeout (bool enable) { flags |= isSetBlockScreenTimeout; isBlockScreenTimeout = enable; }
    void setSubtleLightbar (bool enable) { flags |= isSetSubtleLightbar; isSubtleLightbar = enable; }
    void setActiveTouchpanel(bool enable) { flags |= isSetActiveTouchpanel; activeTouchpanel = enable; }
    void setAlsDisabled(bool disable) { flags |= isSetAlsDisabled; alsDisabled = disable; }
    void setFullScreen(bool enable) { flags |= isSetFullScreen; fullScreen = enable; }
    void setOverlayNotificationsPosition(unsigned int position);
    void setSuppressBannerMessages(bool enable) { flags |= isSetSuppressBannerMessages; suppressBannerMessages = enable; }
    void setHasPauseUi(bool val) { flags |= isSetHasPauseUi; hasPauseUi = val; }
    void setSuppressGestures(bool val) { flags |= isSetSuppressGestures; suppressGestures = val; }
    void setDashboardManualDragMode(bool isManual) { flags |= isSetDashboardManualDragMode; dashboardManualDrag = isManual; }
    void setStatusBarColor(unsigned int color) { flags |= isSetStatusBarColor; statusBarColor = color; }
    void setRotationLockMaximized(bool enable) { flags |= isSetRotationLockMaximized; rotationLockMaximized = enable;}
    void setAllowResizeOnPositiveSpaceChange(bool allow) { flags |= isSetAllowResizeOnPositiveSpaceChange; allowResizeOnPositiveSpaceChange = allow; }
    void setAllowGyroEvents(bool allow) { flags |= isSetGyro; gyroEnabled = allow; }
    void setCompassEvents(bool enable) { flags |= isSetEnableCompassEvents; compassEnabled = enable; }
};



#endif // WINDOWPROPERTIES_H
