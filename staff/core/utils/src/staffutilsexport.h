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

#ifndef _STAFFUTILSEXPORT_H_
#define _STAFFUTILSEXPORT_H_

#ifdef WIN32
  #ifdef _MSC_VER
    #pragma warning(disable: 4786 4251 4521)
  #endif

  #define STAFF_DLL_EXPORT __declspec(dllexport)
  #define STAFF_DLL_IMPORT __declspec(dllimport) 

  #ifdef STAFF_UTILS_DLL_EXPORTS
    #define STAFF_UTILS_EXPORT STAFF_DLL_EXPORT
  #else
    #define STAFF_UTILS_EXPORT STAFF_DLL_IMPORT
  #endif
#else
  #define STAFF_UTILS_EXPORT 
#endif

#ifndef STAFF_DEPRECATED
  #ifdef _MSC_VER
    #if _MSC_FULL_VER >= 140050320
      #define STAFF_DEPRECATED(Replacement) __declspec(deprecated("You are using deprecated API. Consider using " #Replacement " instead."))
    #else
      #define STAFF_DEPRECATED(Replacement) __declspec(deprecated)
    #endif
  #elif __GNUC__ >= 3
    #define STAFF_DEPRECATED(Replacement) __attribute__ ((deprecated))
  #else
    #define STAFF_DEPRECATED(Replacement)
  #endif
#endif

#endif // _STAFFUTILSEXPORT_H_

