/*!
 * \file fastuidraw_memory.hpp
 * \brief file fastuidraw_memory.hpp
 *
 * Adapted from: WRATHNew.hpp and WRATHmemory.hpp of WRATH:
 *
 * Copyright 2013 by Nomovok Ltd.
 * Contact: info@nomovok.com
 * This Source Code Form is subject to the
 * terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with
 * this file, You can obtain one at
 * http://mozilla.org/MPL/2.0/.
 *
 * \author Kevin Rogovin <kevin.rogovin@nomovok.com>
 * \author Kevin Rogovin <kevin.rogovin@intel.com>
 *
 */

#pragma once

/*!\addtogroup Utility
  @{
 */

#include <stdlib.h>
#include <boost/checked_delete.hpp>
#include <fastuidraw/util/fastuidraw_memory_private.hpp>

/*!\def FASTUIDRAWnew
  When DEBUG is defined, allocations with FASTUIDRAWnew are tracked and
  at program exit a list of those objects not deleted by FASTUIDRAWdelete
  are printed with the file and line number of the allocation. When DEBUG
  is not defined, FASTUIDRAWnew maps to new.
 */

/*!\def FASTUIDRAWdelete
  Use FASTUIDRAWdelete for objects allocated with FASTUIDRAWnew.
  When DEBUG is not defined, maps to boost::checked_delete(p).
  \param ptr address of object to delete, value must be a return
             value of FASTUIDRAWnew
 */

/*!\def FASTUIDRAWdelete_array
  Use FASTUIDRAWdelete_array for arrays of objects allocated with FASTUIDRAWnew.
  When DEBUG is not defined, maps to boost::checked_array_delete().
  \param ptr address of array of objects to delete, value must be a return
             value of FASTUIDRAWnew
 */

/*!\def FASTUIDRAWmalloc
  When DEBUG is defined, allocations with FASTUIDRAWmalloc are tracked and
  at program exit a list of those objects not deleted by FASTUIDRAWfree
  are printed with the file and line number of the allocation. When DEBUG
  is not defined, maps to ::malloc.
 */

/*!\def FASTUIDRAWcalloc
  When DEBUG is defined, allocations with FASTUIDRAWcalloc are tracked and
  at program exit a list of those objects not deleted by FASTUIDRAWfree
  are printed with the file and line number of the allocation. When DEBUG
  is not defined, maps to ::malloc.
  \param nmemb number of elements to allocate
  \param size size of each element in bytes
 */

/*!\def FASTUIDRAWrealloc
  When DEBUG is defined, allocations with FASTUIDRAWrealloc are tracked and
  at program exit a list of those objects not deleted by FASTUIDRAWfree
  are printed with the file and line number of the allocation. When DEBUG
  is not defined, maps to ::realloc.
  \param ptr pointer at whcih to rellocate
  \param size new size
 */

/*!\def FASTUIDRAWfree
  Use FASTUIDRAWfree for objects allocated with FASTUIDRAWmallo,
  FASTUIDRAWrealloc and FASTUIDRAWcalloc. When DEBUG is not defined,
  maps to ::free(p).
  \param ptr address of object to free
 */
#ifdef DEBUG
#define FASTUIDRAWnew \
  ::new(__FILE__, __LINE__)
#define FASTUIDRAWdelete(ptr) \
  do {                                                                  \
    fastuidraw::memory::object_deletion_message(ptr, __FILE__, __LINE__); \
    boost::checked_delete(ptr); } while(0)
#define FASTUIDRAWdelete_array(ptr) \
  do {                                                                  \
    fastuidraw::memory::object_deletion_message(ptr, __FILE__, __LINE__); \
    boost::checked_array_delete(ptr); } while(0)
#define FASTUIDRAWmalloc(size) \
  fastuidraw::memory::malloc_implement(size, __FILE__, __LINE__)
#define FASTUIDRAWcalloc(nmemb, size) \
  fastuidraw::memory::calloc_implement(nmemb, size, __FILE__, __LINE__)
#define FASTUIDRAWrealloc(ptr, size) \
  fastuidraw::memory::realloc_implement(ptr, size, __FILE__, __LINE__)
#define FASTUIDRAWfree(ptr) \
  fastuidraw::memory::free_implement(ptr, __FILE__, __LINE__)

#else

#define FASTUIDRAWnew \
  new
#define FASTUIDRAWdelete(ptr) \
  do { boost::checked_delete(ptr); } while(0)
#define FASTUIDRAWdelete_array(ptr) \
  do { boost::checked_array_delete(ptr); } while(0)
#define FASTUIDRAWmalloc(size) \
  ::malloc(size)
#define FASTUIDRAWcalloc(nmemb, size) \
  ::calloc(nmemb, size);
#define FASTUIDRAWrealloc(ptr, size) \
  ::realloc(ptr, size);
#define FASTUIDRAWfree(ptr) \
  ::free(ptr)

#endif

/*! @} */
