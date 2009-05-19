// This file generated by staff_codegen
// Service Implementation

#ifndef _WidgetManagerImpl_h_
#define _WidgetManagerImpl_h_

#include "WidgetManager.h"
#include <rise/xml/XMLDocument.h>

namespace widget
{
  class CWidgetManagerImpl: public CWidgetManager
  {
  public:
    CWidgetManagerImpl();
    virtual ~CWidgetManagerImpl();
    
    virtual void Open(const std::string& sProfile);
    virtual void Close();
    virtual void Commit();

    virtual TStringMap GetWidgetClasses();

    virtual TWidgetMap GetActiveWidgets() const;
    virtual void AddWidget(const SWidget& rWidget);
    virtual void DeleteWidget(const std::string& sId);
    virtual void DeleteWidgets(const TStringList& lsIds);
    virtual void AlterWidget(const SWidget& rWidget);

    virtual TStringMap GetAvailableWidgetGroups() const;
    virtual TWidgetGroupMap GetWidgetGroups(const TStringList& lsWidgetGroups) const;

    virtual void AddWidgetGroup(const SWidgetGroup& rWidgetGroup);
    virtual void DeleteWidgetGroup(const std::string& sGroupId);
    virtual void AlterWidgetGroup(const SWidgetGroup& rWidgetGroup);

    virtual TStringList GetActiveWidgetGroups() const;
    virtual void SetActiveWidgetGroups(const TStringList& lsActiveWidgetGroups);


  private:
    struct SWidgetInternal;
    typedef std::map<std::string, SWidgetInternal> TWidgetInternalMap;
    struct SWidgetGroupInternal;
    typedef std::map<std::string, SWidgetGroupInternal> TWidgetGroupInternalMap;

  private:
    virtual void LoadWidgetClasses(const std::string& sProfile);
    const std::string& GetSessionId();
    int GetUserId();

    void LoadWidgets(const rise::xml::CXMLNode& rNodeWidgets, TWidgetInternalMap& mWidgets);
    
  private:
    TStringMap m_mWidgetClasses;
    TWidgetInternalMap m_mActiveWidgets;
    TStringList m_lsActiveWidgetGroups;
    TWidgetGroupInternalMap m_mWidgetGroups;
    rise::xml::CXMLDocument m_tDoc;
    std::string m_sUserDbFileName;
    std::string m_sProfile;
    std::string m_sSessionId;
    static std::string m_sComponentHome;
    int m_nUserId;
  };
}


#endif // _WidgetManagerImpl_h_
