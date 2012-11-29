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

#ifndef LOCALEPREFERENCES_H
#define LOCALEPREFERENCES_H

#include "Common.h"

#include <string>
#include <lunaservice.h>

#include "Mutex.h"
#include "CustomEvents.h"

#include <QObject>

class LocalePreferences : public QObject
{
    Q_OBJECT

public:

    static LocalePreferences* instance();

    std::string locale() const;
    std::string localeRegion() const;
    std::string phoneRegion() const;
    std::string timeFormat() const;

Q_SIGNALS:
    void prefsLocaleChanged(void);

private:

    LocalePreferences();
    ~LocalePreferences();

    void registerService();
    void init();

    static bool serverConnectCallback(LSHandle *sh, LSMessage *message, void *ctx);
    static bool getPreferencesCallback(LSHandle *sh, LSMessage *message, void *ctx);

    std::string m_locale;
    std::string m_localeRegion;
    std::string m_phoneRegion;
    std::string m_currentTimeFormat;

    mutable Mutex m_mutex;
    LSHandle* m_lsHandle;
    LSMessageToken m_serverStatusToken;
};


#endif // LOCALEPREFERENCES_H
