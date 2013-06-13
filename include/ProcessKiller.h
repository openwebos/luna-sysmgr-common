/* @@@LICENSE
*
*      Copyright (c) 2010-2013 LG Electronics, Inc.
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


#ifndef PROCESSKILLER_H
#define PROCESSKILLER_H

#include <QObject>
#include <QCoreApplication>
#include "Settings.h"

class ProcessKiller : public QObject
{
    Q_OBJECT
public:
    ProcessKiller() {}
    Q_INVOKABLE void localeChanged()
    {
        // Don't shutdown for locale changes in minimal mode
        if (Settings::LunaSettings()->uiType != Settings::UI_MINIMAL)
            QCoreApplication::instance()->exit(0);
    }
};

#endif // PROCESSKILLER_H
