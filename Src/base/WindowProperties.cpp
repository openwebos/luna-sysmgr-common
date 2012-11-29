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


#include "WindowProperties.h"

void WindowProperties::merge(const WindowProperties& props)
{
    if (props.flags & isSetBlockScreenTimeout)
        setBlockScreenTimeout(props.isBlockScreenTimeout);

    if (props.flags & isSetSubtleLightbar)
        setSubtleLightbar(props.isSubtleLightbar);

    if (props.flags & isSetFullScreen)
        setFullScreen(props.fullScreen);

    if (props.flags & isSetActiveTouchpanel)
        setActiveTouchpanel(props.activeTouchpanel);

    if (props.flags & isSetAlsDisabled)
        setAlsDisabled(props.alsDisabled);

    if (props.flags & isSetOverlayNotifications)
        setOverlayNotificationsPosition(props.overlayNotificationsPosition);

    if (props.flags & isSetSuppressBannerMessages)
        setSuppressBannerMessages(props.suppressBannerMessages);

    if (props.flags & isSetHasPauseUi)
        setHasPauseUi(props.hasPauseUi);

    if (props.flags & isSetSuppressGestures)
        setSuppressGestures(props.suppressGestures);

    if (props.flags & isSetDashboardManualDragMode)
        setDashboardManualDragMode(props.dashboardManualDrag);

    if (props.flags & isSetStatusBarColor)
        setStatusBarColor(props.statusBarColor);

    if (props.flags & isSetRotationLockMaximized)
        setRotationLockMaximized(props.rotationLockMaximized);

    if (props.flags & isSetAllowResizeOnPositiveSpaceChange)
        setAllowResizeOnPositiveSpaceChange(props.allowResizeOnPositiveSpaceChange);

    if (props.flags & isSetGyro)
        setAllowGyroEvents(props.gyroEnabled);

    if (props.flags & isSetEnableCompassEvents)
        setCompassEvents(props.compassEnabled);
}

void WindowProperties::setOverlayNotificationsPosition(unsigned int position)
{
    flags |= isSetOverlayNotifications;
    if (position > OverlayNotificationsBottom && position < OverlayNotificationsLast)
        overlayNotificationsPosition = position;
    else
        overlayNotificationsPosition = OverlayNotificationsBottom;
}
