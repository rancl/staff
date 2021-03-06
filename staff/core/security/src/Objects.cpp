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

#ifdef _MSC_VER
#pragma warning (disable : 4267)
#endif

#include <staff/common/Exception.h>
#include <staff/utils/Log.h>
#include <staff/sqlite3/sqlite3.h>
#include "DbConn.h"
#include "Objects.h"

namespace staff
{
  namespace security
  {
    Objects& Objects::Inst()
    {
      static Objects tInst;
      return tInst;
    }

    void Objects::GetById(int nId, Object& rstObject)
    {
      sqlite3* pDb = DbConn::GetDb();
      sqlite3_stmt* pVm = NULL;

      int nResult = sqlite3_prepare_v2(pDb, "SELECT id, name, description, parentid FROM objects WHERE id=?", -1, &pVm, NULL);
      STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

      try
      {
        nResult = sqlite3_bind_int(pVm, 1, nId);
        STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

        // get data
        STAFF_ASSERT(sqlite3_step(pVm) == SQLITE_ROW, "Object with id is not found: " + std::string(sqlite3_errmsg(pDb)));

        rstObject.nId = sqlite3_column_int(pVm, 0);
        const char* szTmp = reinterpret_cast<const char*>(sqlite3_column_text(pVm, 1));
        STAFF_ASSERT(szTmp, "Failed to get object name");
        rstObject.sName = szTmp;
        szTmp = reinterpret_cast<const char*>(sqlite3_column_text(pVm, 2));
        rstObject.sDescription = szTmp != NULL ? szTmp : "";
        rstObject.nParentId = sqlite3_column_int(pVm, 3);
      }
      catch(...)
      {
        sqlite3_finalize(pVm);
        throw;
      }

      STAFF_ASSERT(sqlite3_finalize(pVm) == SQLITE_OK, sqlite3_errmsg(pDb));
    }

    void Objects::GetByPathName(const std::string& sName, Object& rstObject)
    {
      sqlite3* pDb = DbConn::GetDb();
      sqlite3_stmt* pVm = NULL;

      std::string sRequest = "0";
      std::string::size_type nPos = sName.find_first_of('.');
      std::string::size_type nBegin = 0;

      for (; nPos != std::string::npos; nBegin = nPos + 1, nPos = sName.find_first_of('.', nBegin))
      {
        sRequest = "SELECT id FROM objects WHERE name = '" + sName.substr(nBegin, nPos - nBegin)
                   + "' AND parentid = (" + sRequest + ")";
      }

      sRequest = "SELECT id, name, description, parentid FROM objects WHERE name = '"
                 + sName.substr(nBegin, nPos - nBegin) + "' AND parentid = (" + sRequest + ")";


      int nResult = sqlite3_prepare_v2(pDb, sRequest.c_str(), sRequest.size(), &pVm, NULL);
      STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

      try
      {
        // get data
        STAFF_ASSERT(sqlite3_step(pVm) == SQLITE_ROW, "Object with id is not found: " + std::string(sqlite3_errmsg(pDb)));

        rstObject.nId = sqlite3_column_int(pVm, 0);
        const char* szTmp = reinterpret_cast<const char*>(sqlite3_column_text(pVm, 1));
        STAFF_ASSERT(szTmp, "Failed to get object name");
        rstObject.sName = szTmp;
        szTmp = reinterpret_cast<const char*>(sqlite3_column_text(pVm, 2));
        rstObject.sDescription = szTmp != NULL ? szTmp : "";
        rstObject.nParentId = sqlite3_column_int(pVm, 3);
      }
      catch(...)
      {
        sqlite3_finalize(pVm);
        throw;
      }

      STAFF_ASSERT(sqlite3_finalize(pVm) == SQLITE_OK, sqlite3_errmsg(pDb));
    }

    bool Objects::GetIdByPathName(const std::string& sName, int& nId)
    {
      sqlite3* pDb = DbConn::GetDb();
      sqlite3_stmt* pVm = NULL;

      std::string sRequest = "0";
      std::string::size_type nPos = sName.find_first_of('.');
      std::string::size_type nBegin = 0;
      bool bFound = false;

      for (; nPos != std::string::npos; nBegin = nPos + 1, nPos = sName.find_first_of('.', nBegin))
      {
        sRequest = "SELECT id FROM objects WHERE name = '" + sName.substr(nBegin, nPos - nBegin)
                   + "' AND parentid = (" + sRequest + ")";
      }

      sRequest = "SELECT id FROM objects WHERE name = '"
                 + sName.substr(nBegin, nPos - nBegin) + "' AND parentid = (" + sRequest + ")";


      int nResult = sqlite3_prepare_v2(pDb, sRequest.c_str(), sRequest.size(), &pVm, NULL);
      STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

      try
      {
        // get data
        if (sqlite3_step(pVm) == SQLITE_ROW)
        {
          nId = sqlite3_column_int(pVm, 0);
          bFound = true;
        }
      }
      catch(...)
      {
        sqlite3_finalize(pVm);
        throw;
      }

      STAFF_ASSERT(sqlite3_finalize(pVm) == SQLITE_OK, sqlite3_errmsg(pDb));

      return bFound;
    }

    void Objects::Add(const std::string& sName, const std::string& sDescription, int nParentId, int& nId)
    {
      sqlite3* pDb = DbConn::GetDb();
      sqlite3_stmt* pVm = NULL;

      int nResult = sqlite3_prepare_v2(pDb, "INSERT INTO objects(name, description, parentid) VALUES(?, ?, ?)", -1, &pVm, NULL);
      STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

      try
      {
        nResult = sqlite3_bind_text(pVm, 1, sName.c_str(), sName.size(), SQLITE_STATIC);
        STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

        nResult = sqlite3_bind_text(pVm, 2, sDescription.c_str(), sDescription.size(), SQLITE_STATIC);
        STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

        nResult = sqlite3_bind_int(pVm, 3, nParentId);
        STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

        // get data
        STAFF_ASSERT(sqlite3_step(pVm) == SQLITE_DONE, "Failed to add object: " + std::string(sqlite3_errmsg(pDb)));

        // get inserted object id
        nId = static_cast<int>(sqlite3_last_insert_rowid(pDb));
      }
      catch(...)
      {
        sqlite3_finalize(pVm);
        throw;
      }

      STAFF_ASSERT(sqlite3_finalize(pVm) == SQLITE_OK, sqlite3_errmsg(pDb));
    }

    void Objects::Remove(int nId)
    {
      sqlite3* pDb = DbConn::GetDb();
      sqlite3_stmt* pVm = NULL;

      int nResult = sqlite3_prepare_v2(pDb, "DELETE FROM objects WHERE id=?", -1, &pVm, NULL);
      STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

      try
      {
        nResult = sqlite3_bind_int(pVm, 1, nId);
        STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

        // get data
        STAFF_ASSERT(sqlite3_step(pVm) == SQLITE_DONE, "Object with id is not found: " + std::string(sqlite3_errmsg(pDb)));

        // get inserted object id
        nId = static_cast<int>(sqlite3_last_insert_rowid(pDb));
      }
      catch(...)
      {
        sqlite3_finalize(pVm);
        throw;
      }

      STAFF_ASSERT(sqlite3_finalize(pVm) == SQLITE_OK, sqlite3_errmsg(pDb));
    }

    void Objects::SetDescription(int nId, const std::string& sDescription)
    {
      sqlite3* pDb = DbConn::GetDb();
      sqlite3_stmt* pVm = NULL;

      int nResult = sqlite3_prepare_v2(pDb, "UPDATE objects SET description = ? WHERE id=?", -1, &pVm, NULL);
      STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

      try
      {
        nResult = sqlite3_bind_text(pVm, 1, sDescription.c_str(), sDescription.size(), SQLITE_STATIC);
        STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

        nResult = sqlite3_bind_int(pVm, 2, nId);
        STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

        // get data
        STAFF_ASSERT(sqlite3_step(pVm) == SQLITE_DONE, "Failed to set object description: " + std::string(sqlite3_errmsg(pDb)));
      }
      catch(...)
      {
        sqlite3_finalize(pVm);
        throw;
      }

      STAFF_ASSERT(sqlite3_finalize(pVm) == SQLITE_OK, sqlite3_errmsg(pDb));
    }

    void Objects::GetChilds(int nId, ObjectsList& rlsChilds)
    {
      sqlite3* pDb = DbConn::GetDb();
      sqlite3_stmt* pVm = NULL;

      int nResult = sqlite3_prepare_v2(pDb, "SELECT id, name, description, parentid FROM objects WHERE parentid=?", -1, &pVm, NULL);
      STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

      rlsChilds.clear();

      try
      {
        nResult = sqlite3_bind_int(pVm, 1, nId);
        STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

        // get data
        while (sqlite3_step(pVm) == SQLITE_ROW)
        {
          Object stObject;
          stObject.nId = sqlite3_column_int(pVm, 0);
          const char* szTmp = reinterpret_cast<const char*>(sqlite3_column_text(pVm, 1));
          STAFF_ASSERT(szTmp, "Failed to get object name");
          stObject.sName = szTmp;
          szTmp = reinterpret_cast<const char*>(sqlite3_column_text(pVm, 2));
          stObject.sDescription = szTmp != NULL ? szTmp : "";
          stObject.nParentId = sqlite3_column_int(pVm, 3);
          rlsChilds.push_back(stObject);
        }
      }
      catch(...)
      {
        sqlite3_finalize(pVm);
        throw;
      }

      STAFF_ASSERT(sqlite3_finalize(pVm) == SQLITE_OK, sqlite3_errmsg(pDb));
    }

    bool Objects::GetChildId(int nId, const std::string& sChildName, int& nChildId)
    {
      sqlite3* pDb = DbConn::GetDb();
      sqlite3_stmt* pVm = NULL;
      bool bResult = false;

      int nResult = sqlite3_prepare_v2(pDb, "SELECT id FROM objects WHERE parentid=? AND name=?", -1, &pVm, NULL);
      STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

      try
      {
        nResult = sqlite3_bind_int(pVm, 1, nId);
        STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

        nResult = sqlite3_bind_text(pVm, 2, sChildName.c_str(), sChildName.size(), SQLITE_STATIC);
        STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

        // get data
        if (sqlite3_step(pVm) == SQLITE_ROW)
        {
          bResult = true;
          nChildId = sqlite3_column_int(pVm, 0);
        }
      }
      catch(...)
      {
        sqlite3_finalize(pVm);
        throw;
      }

      STAFF_ASSERT(sqlite3_finalize(pVm) == SQLITE_OK, sqlite3_errmsg(pDb));

      return bResult;
    }

    void Objects::GetParent(int nId, Object& rstParent)
    {
      sqlite3* pDb = DbConn::GetDb();
      sqlite3_stmt* pVm = NULL;

      int nResult = sqlite3_prepare_v2(pDb, "SELECT id, name, description, parentid FROM objects WHERE id="
                                              "(SELECT parentid FROM objects WHERE id=?)", -1, &pVm, NULL);
      STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

      try
      {
        nResult = sqlite3_bind_int(pVm, 1, nId);
        STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

        // get data
        STAFF_ASSERT(sqlite3_step(pVm) == SQLITE_ROW, "Object with id is not found: " + std::string(sqlite3_errmsg(pDb)));

        rstParent.nId = sqlite3_column_int(pVm, 0);
        const char* szTmp = reinterpret_cast<const char*>(sqlite3_column_text(pVm, 1));
        STAFF_ASSERT(szTmp, "Failed to get object name");
        rstParent.sName = szTmp;
        szTmp = reinterpret_cast<const char*>(sqlite3_column_text(pVm, 2));
        rstParent.sDescription = szTmp != NULL ? szTmp : "";
        rstParent.nParentId = sqlite3_column_int(pVm, 3);
      }
      catch(...)
      {
        sqlite3_finalize(pVm);
        throw;
      }

      STAFF_ASSERT(sqlite3_finalize(pVm) == SQLITE_OK, sqlite3_errmsg(pDb));
    }

    void Objects::GetParentId(int nId, int& nParentId)
    {
      sqlite3* pDb = DbConn::GetDb();
      sqlite3_stmt* pVm = NULL;

      int nResult = sqlite3_prepare_v2(pDb, "SELECT parentid FROM objects WHERE id=?", -1, &pVm, NULL);
      STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

      try
      {
        nResult = sqlite3_bind_int(pVm, 1, nId);
        STAFF_ASSERT(nResult == SQLITE_OK, sqlite3_errmsg(pDb));

        // get data
        STAFF_ASSERT(sqlite3_step(pVm) == SQLITE_ROW, "Object with id is not found: " + std::string(sqlite3_errmsg(pDb)));

        nParentId = sqlite3_column_int(pVm, 0);
      }
      catch(...)
      {
        sqlite3_finalize(pVm);
        throw;
      }

      STAFF_ASSERT(sqlite3_finalize(pVm) == SQLITE_OK, sqlite3_errmsg(pDb));
    }

    Objects::Objects()
    {
    }

    Objects::~Objects()
    {
    }

  }
}
