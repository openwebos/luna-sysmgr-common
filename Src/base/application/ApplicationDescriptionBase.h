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

#ifndef APPLICATIONDESCRIPTIONBASE_H
#define APPLICATIONDESCRIPTIONBASE_H

#include <string>
#include "Common.h"

struct json_object;

class ApplicationDescriptionBase
{
public:
    enum Type {
        Type_Web = 0,
        Type_Native,
        Type_PDK,
        Type_SysmgrBuiltin,
        Type_Qt
    };

    ApplicationDescriptionBase();
    virtual ~ApplicationDescriptionBase() {}

    static ApplicationDescriptionBase* fromJsonString(const char* jsonStr, ApplicationDescriptionBase* base = NULL);

    const std::string& id()         const { return m_id; }
    const std::string& title()         const { return m_title; }
    const std::string& entryPoint() const { return m_entryPoint; }
    bool               isHeadLess() const { return m_isHeadLess; }

    std::string requestedWindowOrientation() { return m_requestedWindowOrientation; }

    virtual void getAppDescriptionString(std::string &descString) const;
protected:
    // Gives an json_object filled by information within this class
    virtual json_object* getAppDescription() const;
private:

    std::string             m_id;
    std::string             m_title;    //copy of default launchpoint's title
    std::string             m_entryPoint;
    bool                    m_isHeadLess;
    std::string             m_requestedWindowOrientation;
};

#endif // APPLICATIONDESCRIPTIONBASE_H
