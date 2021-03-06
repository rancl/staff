// This file generated by staff_codegen
// For more information please visit: http://code.google.com/p/staff/

#ifndef _foo_h_
#define _foo_h_

#include <string>
#include <staff/common/Abstract.h>
#include <staff/common/Optional.h>
#include <staff/common/WsdlTypes.h>
#include <staff/common/IService.h>
#include "foo_types.h"
#include "foo_other_types.h"

// *interface.elementFormDefault: qualified
// *interface.targetNamespace: http://fooUrl/




namespace ns_staff
{

  //!  service ns_staff.fooService
  // *serviceUri: http://fooUrl/fooService.php
  // *soapVersion: 1.1
  // *targetNamespace: http://fooMethod
  class fooService: public staff::IService
  {
  public:
    //! Creates Account
    // *requestElement: fooMethodRequest
    // *responseElement: fooMethodResponse
    // *soapAction: http://fooUrl/fooMethod
    virtual void fooMethod(const staff::Optional< ::ns_staff::foo_struct >& account) = 0;

  };

}

#endif // _BssPrincipalManagement_h_

