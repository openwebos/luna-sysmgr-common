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

#include <cjson/json.h>
#include <stdio.h>

#include "Common.h"
#include "ApplicationDescriptionBase.h"
#include "Utils.h"

ApplicationDescriptionBase::ApplicationDescriptionBase()
{
    m_isHeadLess = false;
    m_requestedWindowOrientation = "";
}

bool ApplicationDescriptionBase::fromJsonObject(const struct json_object* root)
{
    bool success = true;

    success &= extractFromJson(root, "id", m_id);
    success &= extractFromJson(root, "main", m_entryPoint);
    success &= extractFromJson(root, "noWindow", m_isHeadLess);
    success &= extractFromJson(root, "requestedWindowOrientation", m_requestedWindowOrientation);
    success &= extractFromJson(root, "title", m_title);
    success &= extractFromJson(root, "icon", m_icon);

    if(!success)
        fprintf(stderr,"ApplicationDescriptionBase::fromJsonString : error decodeing app description JSON string.\n" );

    return success;
}

void ApplicationDescriptionBase::getAppDescriptionString(std::string &descString) const
{
    json_object* json = getAppDescription();
    descString = json_object_to_json_string(json);
    json_object_put(json);
}

json_object* ApplicationDescriptionBase::getAppDescription() const
{
    // Compose json string from the app description object  -------------------------------
    // This will only include the integer and string fields of App Description

    json_object* json = json_object_new_object();

    json_object_object_add(json, (char*) "id",   json_object_new_string((char*) m_id.c_str()));
    json_object_object_add(json, (char*) "main",   json_object_new_string((char*) m_entryPoint.c_str()));
    json_object_object_add(json, (char*) "noWindow",   json_object_new_boolean(m_isHeadLess));
    json_object_object_add(json, (char*) "requestedWindowOrientation", json_object_new_string((char*) m_requestedWindowOrientation.c_str()));
    json_object_object_add(json, (char*) "title", json_object_new_string((char *) m_title.c_str()));
    json_object_object_add(json, (char*) "icon", json_object_new_string((char *) m_icon.c_str()));

    return json;
}


