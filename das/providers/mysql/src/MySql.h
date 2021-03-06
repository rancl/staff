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

#ifndef _MYSQLPROVIDER_H_
#define _MYSQLPROVIDER_H_

#include <string>
#include <staff/das/common/Provider.h>

namespace staff
{
  namespace das
  {
    //! MySql provider
    class MySqlProvider: public IProvider
    {
    public:
      MySqlProvider();

      virtual ~MySqlProvider();

      //! initialize
      /*! \param rConfig - provider config
        */
      virtual void Init(const xml::Element& rConfig);

      //! deinitialize
      virtual void Deinit();

      //! get provider name
      /*! \return provider name
        */
      virtual const std::string& GetName() const;

      //! get provider description
      /*! \return provider description
        */
      virtual const std::string& GetDescr() const;

      //! create new executor object to execute query
      /*! \return new executor
        */
      virtual PExecutor GetExecutor();

    private:
      class MySqlImpl;
      MySqlImpl* m_pImpl;
      friend class MySqlQueryExecutor;
    };

  }
}

#endif // _MYSQLPROVIDER_H_
