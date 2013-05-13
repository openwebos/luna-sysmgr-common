# @@@LICENSE
#
#      Copyright (c) 2010-2012 Hewlett-Packard Development Company, L.P.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# LICENSE@@@
# @@@LICENSE
#
#      Copyright (c) 2010 Hewlett-Packard Development Company, L.P.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# LICENSE@@@

TARGET = LunaSysMgrCommon
TEMPLATE = lib
CONFIG += qt

VPATH += Src \
    Src/base \
    Src/base/application \
    Src/base/hosts \
    Src/base/settings \
    Src/core \
    include

include(Src/nyx/nyx.pri)

OBJECTS_DIR = .obj
MOC_DIR = .moc

SOURCES += \
    ApplicationDescriptionBase.cpp \
    AsyncCaller.cpp \
    HostBase.cpp \
    JSONUtils.cpp \
    LocalePreferences.cpp \
    Localization.cpp \
    Logging.cpp \
    Mutex.cpp \
    Settings.cpp \
    SingletonTimer.cpp \
    TaskBase.cpp \
    Timer.cpp \
    Utils.cpp \
    WindowProperties.cpp

HEADERS += \
    ApplicationDescriptionBase.h \
    AsyncCaller.h \
    Common.h \
    CustomEvents.h \
    Event.h \
    HostBase.h \
    InputControl.h \
    JSONUtils.h \
    LedControl.h \
    LocalePreferences.h \
    Localization.h \
    Logging.h \
    Mutex.h \
    MutexLocker.h \
    QtUtils.h \
    Settings.h \
    SignalSlot.h \
    SingletonTimer.h \
    sptr.h \
    TaskBase.h \
    Time.h \
    Timer.h \
    Utils.h \
    WindowProperties.h \
    WindowTypes.h \
    ProcessKiller.h


STAGING_INCDIR = $$(STAGING_INCDIR)
isEmpty(STAGING_INCDIR):STAGING_INCDIR = $$(STAGING_DIR)/include

STAGING_LIBDIR = $$(STAGING_LIBDIR)
isEmpty(STAGING_LIBDIR):STAGING_LIBDIR = $$(STAGING_DIR)/lib

CONFIG += link_pkgconfig
PKGCONFIG = glib-2.0 gthread-2.0 LunaSysMgrIpc

QT = core network gui widgets

# ../staging/include ../staging/include/PmLogLib ../staging/include/sysmgr-ipc

QMAKE_CXXFLAGS += -fno-rtti -fno-exceptions -Wall -fpermissive # -fvisibility=hidden
QMAKE_CXXFLAGS += -DFIX_FOR_QT
QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter -Wno-unused-variable -Wno-reorder -Wno-missing-field-initializers -Wno-extra

linux-g++ {
    include(desktop.pri)
} else:linux-g++-64 {
    include(desktop.pri)
} else:linux-qemux86-g++ {
    include(emulator.pri)
    QMAKE_CXXFLAGS += -fno-strict-aliasing
} else {
    ## First, check to see if this in an emulator build
    include(emulator.pri)
    contains (CONFIG_BUILD, webosemulator) {
        QMAKE_CXXFLAGS += -fno-strict-aliasing
    } else {
        ## Neither a desktop nor an emulator build, so must be a device
        include(device.pri)
    }
}

INCLUDEPATH += $$VPATH

DESTDIR = ./$${BUILD_TYPE}-$${MACHINE_NAME}

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc

