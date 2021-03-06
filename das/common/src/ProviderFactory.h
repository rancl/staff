/*
 *  Copyright 2010 Utkin Dmitry
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

#ifndef PROVIDERFACTORY_H
#define PROVIDERFACTORY_H

#include <string>
#include <list>
#include "staffdascommonexport.h"
#include "Provider.h"

namespace staff
{
namespace das
{
  class STAFF_DAS_COMMON_EXPORT ProviderFactory
  {
  public:
    static ProviderFactory& Inst();

    void GetProviders(StringList& rlsProviders);

    PProvider Allocate(const std::string& sProvider);

  private:
    ProviderFactory();
    ~ProviderFactory();
    ProviderFactory(const ProviderFactory&);
    ProviderFactory& operator=(const ProviderFactory&);

  private:
    class ProviderFactoryImpl;
    ProviderFactoryImpl* m_pImpl;
  };
}
}

#endif // PROVIDERFACTORY_H
