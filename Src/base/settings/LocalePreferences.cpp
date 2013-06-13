/* @@@LICENSE
*
*      Copyright (c) 2008-2013 LG Electronics, Inc.
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

#include "Common.h"

#include <sstream>

#include "LocalePreferences.h"
#include "Settings.h"

#include <sqlite3.h>
#include <cjson/json.h>

#include "HostBase.h"
#include "Logging.h"
#include "MutexLocker.h"
#include "JSONUtils.h"
#include <QLocale>
#include "Localization.h"

static const char* s_defaultLocale = "en_us";
static const char* s_defaultLocaleRegion = "us";
static const char* s_defaultPhoneRegion = "us";
static const char* s_prefsDbPath = "/var/luna/preferences/systemprefs.db";
static const char* s_defaultTimezone = "Etc/UTC";
static const char* s_defaultClock = "locale";

static const char* s_logChannel = "LocalePreferences";

static const char *kLocaleInfo_Root = "localeInfo";
static const char *kLocaleInfo_Locales = "locales";
static const char *kLocaleInfo_UILocale = "UI";
static const char *kLocaleInfo_Timezone = "timezone";
static const char *kLocaleInfo_Clock = "clock";

LocalePreferences* LocalePreferences::instance()
{
    static LocalePreferences* s_prefs = 0;
    if (!s_prefs)
        s_prefs = new LocalePreferences();

    return s_prefs;
}

LocalePreferences::LocalePreferences()
    : m_currentTimeFormat("HH12")
    , m_lsHandle(0)
{
    init();
    registerService();
}

LocalePreferences::~LocalePreferences()
{
}

std::string LocalePreferences::locale() const
{
    MutexLocker locker(&m_mutex);
    return m_locale;
}

std::string LocalePreferences::localeRegion() const
{
    MutexLocker locker(&m_mutex);
    return m_localeRegion;
}

std::string LocalePreferences::phoneRegion() const
{
    MutexLocker locker(&m_mutex);
    return m_phoneRegion;
}

std::string LocalePreferences::timeFormat() const
{
    MutexLocker locker(&m_mutex);
    return m_currentTimeFormat;
}

QString LocalePreferences::uiLocale() const
{
    MutexLocker locker(&m_mutex);
    return m_locales.locales.value(kLocaleInfo_UILocale);
}

QString LocalePreferences::locale(const QString &name) const
{
    MutexLocker locker(&m_mutex);
    return m_locales.locales.value(name);
}

QString LocalePreferences::timezone() const
{
    MutexLocker locker(&m_mutex);
    return m_locales.timezone;
}

QString LocalePreferences::clock() const
{
    MutexLocker locker(&m_mutex);
    return m_locales.clock;
}

LocalePreferences::LocaleInfo LocalePreferences::locales() const
{
    MutexLocker locker(&m_mutex);
    return m_locales;
}

void LocalePreferences::init()
{
    m_locale = s_defaultLocale;
    m_localeRegion = s_defaultLocaleRegion;
    m_phoneRegion = s_defaultPhoneRegion;

    m_locales.locales.insert(kLocaleInfo_UILocale, s_defaultLocale);
    m_locales.timezone = s_defaultTimezone;
    m_locales.clock = s_defaultClock;

    // We open the LocalePreferences database and read the locale setting.
    // avoid waiting for the system-service to come up
    // and we waiting synchronously to get the locale value

    sqlite3* prefsDb = 0;
    sqlite3_stmt* statement = 0;
    const char* tail = 0;
    json_object* label = 0;
    json_object* json = 0;
    json_object* subobj = 0;
    std::string localeCountryCode;

    int ret = sqlite3_open(s_prefsDbPath, &prefsDb);
    if (ret) {
        luna_critical(s_logChannel, "Failed to open LocalePreferences db");
        goto Done;
    }
    // immediately read locale
    ret = sqlite3_prepare(prefsDb, "SELECT * FROM Preferences WHERE KEY='locale'",
                          -1, &statement, &tail);
    if (ret) {
        luna_critical(s_logChannel, "Failed to prepare query");
        goto Done;
    }

    ret = sqlite3_step(statement);
    if (ret == SQLITE_ROW) {

        std::string languageCode, countryCode;

        const char* val = (const char*) sqlite3_column_text(statement, 1);
        if (!val)
            goto Done;

        label = 0;
        json = json_tokener_parse(val);
        if (!json || is_error(json))
            goto Done;

        label = json_object_object_get(json, "languageCode");
        if (!label || is_error(label))
            goto Done;
        languageCode = json_object_get_string(label);

        label = json_object_object_get(json, "countryCode");
        if (!label || is_error(label))
            goto Done;
        countryCode = json_object_get_string(label);

        localeCountryCode = countryCode;

        m_locale = languageCode + "_" + countryCode;

        subobj = json_object_object_get(json, "phoneRegion");

        if (subobj && !is_error(subobj)){
            label = json_object_object_get(subobj, "countryCode");

            if (label && !is_error(label)){
                m_phoneRegion = json_object_get_string(label);
            }
        }

        json_object_put(json);
        json = 0;
    }

    sqlite3_finalize(statement);

    // immediately read regon
    ret = sqlite3_prepare(prefsDb, "SELECT * FROM Preferences WHERE KEY='region'",
                          -1, &statement, &tail);
    if (ret) {
        luna_critical(s_logChannel, "Failed to prepare query");
        goto Done;
    }

    ret = sqlite3_step(statement);
    if (ret == SQLITE_ROW) {

        const char* val = (const char*) sqlite3_column_text(statement, 1);
        if (!val)
            goto Done;

        label = 0;
        json = json_tokener_parse(val);
        if (!json || is_error(json))
            goto Done;

        label = json_object_object_get(json, "countryCode");
        if (!label || is_error(label))
            goto Done;
        m_localeRegion = json_object_get_string(label);

        json_object_put(json);
        json = 0;
    }

    sqlite3_finalize(statement);

    // immediately read localeInfo
    ret = sqlite3_prepare(prefsDb,
                          "SELECT * FROM Preferences WHERE KEY='localeInfo'",
                          -1,
                          &statement,
                          &tail);

    if (ret != SQLITE_OK) {
        luna_critical(s_logChannel, "Failed to prepare query");
        goto Done;
    }

    ret = sqlite3_step(statement);

    if (ret == SQLITE_ROW) {
        const char *val = (const char *) sqlite3_column_text(statement, 1);

        if (!val) {
            goto Done;
        }

        label = 0;
        json = json_tokener_parse(val);

        if (!json || is_error(json)) {
            goto Done;
        }

        label = json_object_object_get(json, kLocaleInfo_Locales);

        if (!label || is_error(label)) {
            goto Done;
        }

        json_object_object_foreach(label, keyName, valueName) {
            m_locales.locales.insert(keyName, json_object_get_string(valueName));
        }

        label = json_object_object_get(json, kLocaleInfo_Timezone);

        if (!label || is_error(label)) {
            goto Done;
        }

        m_locales.timezone = QString(json_object_get_string(label));

        label = json_object_object_get(json, kLocaleInfo_Clock);

        if (!label || is_error(label)) {
            goto Done;
        }

        m_locales.clock = QString(json_object_get_string(label));

        json_object_put(json);
        json = 0;
    }

Done:

    QLocale myLocale(m_locales.locales.value(kLocaleInfo_UILocale));
    g_message ("%s: setting locale country %d language %d", __PRETTY_FUNCTION__,
            myLocale.country(), myLocale.language());
    QLocale::setDefault(myLocale);

    // locale region defaults to locale country code
    if (m_localeRegion.empty())
        m_localeRegion = localeCountryCode;

    if (m_phoneRegion.empty())
        m_phoneRegion = m_localeRegion;

    if (json && !is_error(json))
        json_object_put(json);

    if (statement)
        sqlite3_finalize(statement);

    if (prefsDb)
        sqlite3_close(prefsDb);
}

void LocalePreferences::registerService()
{
    bool ret;
    LSError error;
    LSErrorInit(&error);

    ret = LSRegister(NULL, &m_lsHandle, &error);
    if (!ret) {
        g_critical("Failed to register handler: %s", error.message);
        LSErrorFree(&error);
        exit(1);
    }

    ret = LSGmainAttach(m_lsHandle, HostBase::instance()->mainLoop(), &error);
    if (!ret) {
        g_critical("Failed to attach service to main loop: %s", error.message);
        LSErrorFree(&error);
        exit(1);
    }

    ret = LSCall(m_lsHandle, "palm://com.palm.lunabus/signal/registerServerStatus",
                 "{\"serviceName\":\"com.palm.systemservice\"}",
                 serverConnectCallback, this, &m_serverStatusToken, &error);
    if (!ret) {
        g_critical("Failed in calling palm://com.palm.lunabus/signal/registerServerStatus: %s",
                      error.message);
        LSErrorFree(&error);
        exit(1);
    }

}

bool LocalePreferences::serverConnectCallback(LSHandle *sh, LSMessage *message, void *ctx)
{
    if (!message)
        return true;

    const char* payload = LSMessageGetPayload(message);
    if (!payload)
        return true;

    json_object* label = 0;
    json_object* json = 0;
    bool connected = false;
    LocalePreferences * prefObjPtr;

    label = 0;
    json = json_tokener_parse(payload);
    if (!json || is_error(json))
        goto Done;

    label = json_object_object_get(json, "connected");
    if (!label || is_error(label))
        goto Done;
    connected = json_object_get_boolean(label);

    prefObjPtr = (LocalePreferences *)ctx;

    if (connected) {

        // We are connected to the systemservice. call and get the preference values

        bool ret = false;
        LSError error;
        LSErrorInit(&error);

        ret = LSCall(LocalePreferences::instance()->m_lsHandle,
                     "palm://com.palm.systemservice/getPreferences", "{\"subscribe\":true, \"keys\": [ \"locale\" ]}",
                     getPreferencesCallback, prefObjPtr, NULL, &error);
        if (!ret) {
            luna_critical(s_logChannel, "Failed in calling palm://com.palm.systemservice/getPreferences: %s",
                          error.message);
            LSErrorFree(&error);
        }

        ret = LSCall(LocalePreferences::instance()->m_lsHandle,
                     "palm://com.palm.systemservice/getPreferences",
                     "{\"subscribe\":true, \"keys\": [ \"region\", \
                                                       \"timeFormat\" ]}",
                     getPreferencesCallback, prefObjPtr, NULL, &error);
        if (!ret) {
            g_critical("%s: Failed in calling palm://com.palm.systemservice/getPreferences: %s",
                       __PRETTY_FUNCTION__, error.message);
            LSErrorFree(&error);
        }

        ret = LSCall(LocalePreferences::instance()->m_lsHandle,
                     "palm://com.palm.systemservice/getPreferences",
                     "{\"subscribe\":true, \"keys\": [ \"localeInfo\" ]}",
                     getLocaleInfoCallback,
                     prefObjPtr,
                     NULL,
                     &error);

        if (!ret) {
            g_critical("%s: Failed in calling palm://com.palm.systemservice/getPreferences: %s",
                       __PRETTY_FUNCTION__, error.message);
            LSErrorFree(&error);
        }
    }

Done:

    if (json && !is_error(json))
        json_object_put(json);

    return true;
}

bool LocalePreferences::getPreferencesCallback(LSHandle *sh, LSMessage *message, void *ctx)
{
    if (!message)
        return true;

    const char* payload = LSMessageGetPayload(message);
    if (!payload)
        return true;

    json_object* label = 0;
    json_object* root_label = 0;
    json_object* json = 0;
    json_object* value = 0;
    json_object* subobject = 0;

    const char* languageCode = 0;
    const char* countryCode = 0;
    const char* phoneRegion = 0;
    std::string newLocale;
    std::string newLocaleRegion;
    std::string newPhoneRegion;

    const char* imeType = 0;

    LocalePreferences * prefObjPtr = (LocalePreferences *)ctx;
    label = 0;
    json = json_tokener_parse(payload);
    if (!json || is_error(json))
        goto Done;

    g_message("LocalePreferences::getPreferencesCallback(): toString -> [%s]\n", payload);


    value = json_object_object_get(json, "locale");
    if ((value) && (!is_error(value))) {

        label = json_object_object_get(value, "languageCode");
        if ((label) && (!is_error(label))) {
            languageCode = json_object_get_string(label);
        }

        label = json_object_object_get(value, "countryCode");
        if ((label) && (!is_error(label))) {
            countryCode = json_object_get_string(label);
        }

        subobject = json_object_object_get(value, "phoneRegion");
        if ((subobject) && (!is_error(subobject))) {
            label = json_object_object_get(subobject, "countryCode");
            if ((label) && (!is_error(label))) {
                newPhoneRegion = json_object_get_string(label);
            }
        }

        newLocale = languageCode;
        newLocale += "_";
        newLocale += countryCode;

        if ((newLocale != prefObjPtr->m_locale && newLocale != "_") ||
            (!newPhoneRegion.empty() && newPhoneRegion != prefObjPtr->m_phoneRegion )) {

            prefObjPtr->m_mutex.lock();
            if (newLocale != "_") {
                prefObjPtr->m_locale = newLocale;
            }
            if (!newPhoneRegion.empty()){
                prefObjPtr->m_phoneRegion =  newPhoneRegion;
            }
            prefObjPtr->m_mutex.unlock();

            g_warning("locale changed: %s (%s).",
                      newLocale.empty() ? "" : newLocale.c_str(),
                      prefObjPtr->m_locale.empty() ? "" : prefObjPtr->m_locale.c_str());

            // Locale has changed.
            Q_EMIT prefObjPtr->prefsLocaleChanged();

            // reload localization strings to get updated translations of non-cached strings
            Localization::instance()->loadLocalizedStrings();
        }
    }

    value = json_object_object_get(json, "region");
    if (value && !is_error(value)) {

        label = json_object_object_get(value, "countryCode");
        if (label && !is_error(label))
            newLocaleRegion = json_object_get_string(label);

        if (!newLocaleRegion.empty() && newLocaleRegion != prefObjPtr->m_localeRegion) {

            // Don't shutdown sysmgr for locale changes in minimal mode
            if (Settings::LunaSettings()->uiType != Settings::UI_MINIMAL) {

                g_warning("region changed: %s (%s). shutting down sysmgr",
                                          newLocaleRegion.empty() ? "" : newLocaleRegion.c_str(),
                                          prefObjPtr->m_localeRegion.empty() ? "" : prefObjPtr->m_localeRegion.c_str());

                prefObjPtr->m_mutex.lock();
                prefObjPtr->m_localeRegion = newLocaleRegion;
                prefObjPtr->m_mutex.unlock();

                // Region has changed.
                Q_EMIT prefObjPtr->prefsLocaleChanged();
            }
        }
    }
    label = json_object_object_get(json, "timeFormat");
    if (label && !is_error(label)) {
        if (prefObjPtr) {
            MutexLocker locker(&prefObjPtr->m_mutex);
            prefObjPtr->m_currentTimeFormat = json_object_get_string(label);
            Q_EMIT prefObjPtr->signalTimeFormatChanged(prefObjPtr->m_currentTimeFormat.c_str());
        }
    }
Done:

    if (json && !is_error(json))
        json_object_put(json);

    return true;
}

bool LocalePreferences::getLocaleInfoCallback(LSHandle *sh,
                                              LSMessage *message,
                                              void *ctx)
{
    if (!message) {
        return true;
    }

    const char *payload = LSMessageGetPayload(message);

    if (!payload) {
        return true;
    }

    g_message("LocalePreferences::getLocaleInfoCallback(): [%s]\n", payload);

    LocalePreferences *lp = (LocalePreferences *)ctx;
    json_object *rootObj = 0;
    json_object *localeInfoObj = 0;
    json_object *localesObj = 0;
    json_object *subObj = 0;
    QHash<QString, QString> newLocales;
    QString newValue;
    bool changed = false;

    rootObj = json_tokener_parse(payload);

    if (!rootObj || is_error(rootObj)) {
        goto Done;
    }

    localeInfoObj = json_object_object_get(rootObj, kLocaleInfo_Root);

    if (!localeInfoObj || is_error(localeInfoObj)) {
        goto Done;
    }

    localesObj = json_object_object_get(localeInfoObj, kLocaleInfo_Locales);

    if (!localesObj || is_error(localesObj)) {
        goto Done;
    }

    json_object_object_foreach(localesObj, keyName, valueName) {
        newLocales.insert(keyName, json_object_get_string(valueName));
    }

    lp->m_mutex.lock();

    if (!newLocales.isEmpty() && newLocales != lp->m_locales.locales) {
        if (newLocales.value(kLocaleInfo_UILocale) !=
            lp->m_locales.locales.value(kLocaleInfo_UILocale)) {
            QLocale::setDefault(QLocale(newLocales.value(kLocaleInfo_UILocale)));
        }

        lp->m_locales.locales = newLocales;
        changed = true;
    }

    lp->m_mutex.unlock();

    subObj = json_object_object_get(localeInfoObj, kLocaleInfo_Timezone);

    if (!subObj || is_error(subObj)) {
        goto Done;
    }

    newValue = QString(json_object_get_string(subObj));
    lp->m_mutex.lock();

    if (!newValue.isEmpty() && newValue != lp->m_locales.timezone) {
        lp->m_locales.timezone = newValue;
        changed = true;
    }

    lp->m_mutex.unlock();
    subObj = json_object_object_get(localeInfoObj, kLocaleInfo_Clock);

    if (!subObj || is_error(subObj)) {
        goto Done;
    }

    newValue = QString(json_object_get_string(subObj));
    lp->m_mutex.lock();

    if (!newValue.isEmpty() && newValue != lp->m_locales.clock) {
        lp->m_locales.clock = newValue;
        changed = true;
    }

    lp->m_mutex.unlock();

    json_object_put(rootObj);
    rootObj = 0;

Done:
    if (rootObj && !is_error(rootObj)) {
        json_object_put(rootObj);
    }

    if (changed) {
        Q_EMIT lp->localeInfoChanged();
    }

    return true;
}
