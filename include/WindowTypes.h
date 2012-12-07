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

#ifndef WINDOWTYPES_H
#define WINDOWTYPES_H

#include <QGraphicsObject>

namespace WindowType {
    enum Type {
    Type_Invalid                = QGraphicsObject::UserType + 1,
    Type_StatusBar              = QGraphicsObject::UserType + 2,
    Type_Card                   = QGraphicsObject::UserType + 3,
    Type_ChildCard              = QGraphicsObject::UserType + 4,
    Type_Overlay                = QGraphicsObject::UserType + 5,
    Type_Launcher               = QGraphicsObject::UserType + 6,
    Type_Dashboard              = QGraphicsObject::UserType + 7,
    Type_PopupAlert             = QGraphicsObject::UserType + 8,
    Type_BannerAlert            = QGraphicsObject::UserType + 9,
    Type_Menu                   = QGraphicsObject::UserType + 10,
    Type_PIN                    = QGraphicsObject::UserType + 11,
    Type_Emergency              = QGraphicsObject::UserType + 12,
    Type_QtNativePaintWindow    = QGraphicsObject::UserType + 13,
    Type_DockModeWindow         = QGraphicsObject::UserType + 14,
    Type_DockModeLoadingWindow  = QGraphicsObject::UserType + 15,
    Type_ModalChildWindowCard   = QGraphicsObject::UserType + 16,
    Type_None                   = QGraphicsObject::UserType + 32 // arbitrary, can be as large as 0xFFFEFFFF
    };
}
#endif // WINDOWTYPES_H
