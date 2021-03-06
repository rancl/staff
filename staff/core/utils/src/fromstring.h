/*
 *  Copyright 2011 Utkin Dmitry
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 *  This file is part of the WSF Staff project.
 *  Please, visit http://code.google.com/p/staff for more information.
 */

#include <string>
#include "fromcstring.h"

#ifndef _STAFF_UTILS_FROMSTRING_H_
#define _STAFF_UTILS_FROMSTRING_H_

namespace staff
{

  template<typename Type>
  inline Type& FromString(const std::string& rString, Type& rValue)
  {
    FromCString(rString.c_str(), rValue);
    return rValue;
  }

  template<typename Type>
  inline Type& FromString(const std::string& rString, Type& rValue, bool* pbIsOk)
  {
    bool bIsOk = FromCString(rString.c_str(), rValue);
    if (pbIsOk)
    {
      *pbIsOk = bIsOk;
    }
    return rValue;
  }

  template<typename Type>
  inline Type& FromHexString(const std::string& rString, Type& rValue)
  {
    FromHexCString(rString.c_str(), rValue);
    return rValue;
  }

}

#endif // _STAFF_UTILS_FROMSTRING_H_
