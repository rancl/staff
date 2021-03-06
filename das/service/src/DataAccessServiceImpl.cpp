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

// This file generated by staff_codegen
// For more information please visit: http://code.google.com/p/staff/
// Service Implementation

#include <staff/common/Exception.h>
#include <staff/utils/Log.h>
#include <staff/common/DataObject.h>
#include <staff/das/common/DataSource.h>
#include <staff/das/common/DataSourceFactory.h>
#include <staff/das/common/ProviderFactory.h>
#include <staff/das/common/ScriptExecuter.h>
#include "DataAccessServiceImpl.h"

namespace staff
{
namespace das
{

  DataAccessServiceImpl::DataAccessServiceImpl():
    m_pDataSource(NULL)
  {
    try
    {
      // initialize datasources and create virtual components and services
      DataSourceFactory::Inst();
    }
    STAFF_CATCH_ALL
  }

  DataAccessServiceImpl::~DataAccessServiceImpl()
  {
  }

  StringList DataAccessServiceImpl::GetProviders()
  {
    StringList tResult;
    ProviderFactory::Inst().GetProviders(tResult);
    return tResult;  // result
  }

  StringList DataAccessServiceImpl::GetDataSources()
  {
    StringList lsResult;
    DataSourceFactory::Inst().GetDataSources(lsResult);
    return lsResult;
  }

  void DataAccessServiceImpl::SetDataSource(const std::string& sDataSource)
  {
    m_pDataSource = &DataSourceFactory::Inst().GetDataSource(sDataSource);

    const ProvidersInfoList& rlsProviders = m_pDataSource->GetProviders();

    if (!rlsProviders.empty())
    {
      ProviderFactory& rProviderFactory = ProviderFactory::Inst();
      m_stProviders.sDefaultId = rlsProviders.front().sId;

      for (ProvidersInfoList::const_iterator itProvider = rlsProviders.begin();
           itProvider != rlsProviders.end(); ++itProvider)
      {
        PProvider tpProvider = rProviderFactory.Allocate(itProvider->sName);
        tpProvider->Init(itProvider->tConfig);
        m_stProviders.mProviders[itProvider->sId] = tpProvider;
      }
    }
  }

  void DataAccessServiceImpl::FreeDataSource()
  {
    m_pDataSource = NULL;
    m_stProviders.sDefaultId.clear();
    m_stProviders.mProviders.clear();
  }

  DataObject& operator<<(DataObject& rdoTypes, const DataType& rType)
  {
    DataObject tdoType = rdoTypes.CreateChild("Type");

    tdoType.CreateChild("Name").SetText(rType.sName);
    tdoType.CreateChild("Descr").SetText(rType.sDescr);
    tdoType.CreateChild("Extern").SetText(rType.bExtern ? "True" : "False");

    switch (rType.eType)
    {
    case DataType::Generic:
    case DataType::DataObject:
      {
        tdoType.CreateChild("Type").SetText("generic");
        tdoType.CreateChild("DataType").SetText(rType.sType);
        tdoType.CreateChild("Nillable").SetValue(rType.bNillable);
        break;
      }

    case DataType::Struct:
      {
        tdoType.CreateChild("Type").SetText("struct");
        DataObject tdoMembers = tdoType.CreateChild("Members");

        for (DataTypesList::const_iterator itChildType = rType.lsChilds.begin();
            itChildType != rType.lsChilds.end(); ++itChildType)
        {
          const DataType& rChildType = *itChildType;
          if (rChildType.eType != DataType::Generic)
          {
            // chagne format
            DataObject tdoType = tdoMembers.CreateChild("Type");

            tdoType.CreateChild("Name").SetText(rChildType.sName);
            tdoType.CreateChild("Descr").SetText(rChildType.sDescr);
            tdoType.CreateChild("Extern").SetText(rChildType.bExtern ? "True" : "False");
            tdoType.CreateChild("Type").SetText("struct");
            tdoType.CreateChild("DataType").SetText(rChildType.sType);
          }
          else
          {
            tdoMembers << rChildType;
          }
        }

        break;
      }

    case DataType::List:
      {
        tdoType.CreateChild("Type").SetText("list");
        tdoType.CreateChild("ItemType").SetText(rType.sType);
        break;
      }

//    case Type::DataObject:
//      {
//        tdoType.CreateChild("Type").SetText("dataobject");
//        break;
//      }

    default:
      {
        LogWarning() << "Can't write type " << rType.eType;
      }
    }

    return rdoTypes;
  }

  DataObject& operator<<(DataObject& rdoOperations, const Operation& rOperation)
  {
    DataObject tdoOperation = rdoOperations.CreateChild("Operation");

    tdoOperation.CreateChild("Name").SetText(rOperation.sName);
    tdoOperation.CreateChild("Descr").SetText(rOperation.sDescr);

    // Options
    staff::DataObject tdoOptions = tdoOperation.CreateChild("Options");

    for (StringMap::const_iterator itOption = rOperation.mOptions.begin();
         itOption != rOperation.mOptions.end(); ++itOption)
    {
      tdoOptions.CreateChild(itOption->first, itOption->second);
    }

    DataObject tdoParams = tdoOperation.CreateChild("Params");

    for (DataTypesList::const_iterator itParam = rOperation.lsParams.begin();
          itParam != rOperation.lsParams.end(); ++itParam)
    {
      const DataType& rParam = *itParam;

      DataObject tdoParam = tdoParams.CreateChild("Param");

      tdoParam.CreateChild("Name").SetText(rParam.sName);
      tdoParam.CreateChild("Type").SetText(rParam.sType);
    }

    std::string sReturnType;

    if (rOperation.stReturn.eType == DataType::Void)
    {
      sReturnType = "void";
    }
    else
    {
      if (rOperation.stReturn.eType == DataType::List ||
          rOperation.stReturn.eType == DataType::Struct)
      {
        sReturnType = rOperation.stReturn.sName;
      }
      else
      {
        sReturnType = rOperation.stReturn.sType;
      }
    }

    tdoOperation.CreateChild("Return").SetText(sReturnType);

    return rdoOperations;
  }

  //  get project!
  DataObject DataAccessServiceImpl::GetInterface() const
  {
    STAFF_ASSERT(m_pDataSource, "Data Source is not set");

    staff::DataObject tdoInterface("Interface");
    tdoInterface.CreateChild("Name").SetText(m_pDataSource->GetName());
    tdoInterface.CreateChild("Descr").SetText(m_pDataSource->GetDescr());
    tdoInterface.CreateChild("Namespace").SetText(m_pDataSource->GetNamespace());

    // includes
    staff::DataObject tdoIncludes = tdoInterface.CreateChild("Includes");
    const IncludesList& rmIncludes = m_pDataSource->GetIncludes();

    for (IncludesList::const_iterator itInclude = rmIncludes.begin();
      itInclude != rmIncludes.end(); ++itInclude)
    {
      staff::DataObject tdoInclude = tdoIncludes.CreateChild("Include");

      std::string sName = itInclude->sFileName;
      std::string::size_type nPos = sName.find_last_of('.');
      if (nPos != std::string::npos)
      {
        sName.erase(nPos);
      }

      tdoInclude.CreateChild("Name").SetText(sName);
      tdoInclude.CreateChild("FileName").SetText(itInclude->sFileName);
      tdoInclude.CreateChild("Namespace").SetText(m_pDataSource->GetNamespace());

      staff::DataObject tdoTypes = tdoInclude.CreateChild("Types");

      const DataTypesList& rlsTypes = itInclude->lsTypes;
      for (DataTypesList::const_iterator itType = rlsTypes.begin(); itType != rlsTypes.end(); ++itType)
      {
        tdoTypes << *itType;
      }
    }

    // Options
    staff::DataObject tdoOptions = tdoInterface.CreateChild("Options");

    const StringMap& rmOptions = m_pDataSource->GetOptions();
    for (StringMap::const_iterator itOption = rmOptions.begin(); itOption != rmOptions.end(); ++itOption)
    {
      tdoOptions.CreateChild(itOption->first, itOption->second);
    }

    // types
    staff::DataObject tdoTypes = tdoInterface.CreateChild("Types");

    const DataTypesList& rlsTypes = m_pDataSource->GetTypes();
    for (DataTypesList::const_iterator itType = rlsTypes.begin(); itType != rlsTypes.end(); ++itType)
    {
      tdoTypes << *itType;
    }

    // operations
    staff::DataObject tdoOperations = tdoInterface.CreateChild("Operations");

    const OperationsList& rmOperations = m_pDataSource->GetOperations();
    for (OperationsList::const_iterator itOperation = rmOperations .begin();
        itOperation != rmOperations .end(); ++itOperation)
    {
      tdoOperations << *itOperation;
    }

    return tdoInterface;
  }

  DataObject DataAccessServiceImpl::Invoke(const DataObject& rdoOperation)
  {
    STAFF_ASSERT(m_pDataSource, "Not initialized");

    ScriptExecuter tScriptExecuter(*m_pDataSource, m_stProviders);
    DataObject tdoResult(rdoOperation.GetLocalName() + "Result");
    tScriptExecuter.Process(rdoOperation, tdoResult);
    return tdoResult;
  }

}
}

