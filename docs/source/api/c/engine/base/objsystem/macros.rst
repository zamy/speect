
.. index:: 
   single: Generic Object System (C API); Macros

======
Macros
======

Macros for the creation/deletion and casting of objects of
all types as well as some miscellaneous macros to get the base
object, base class and reference count of objects.


Summary
=======

.. doxybridge-autosummary::
   :nosignatures:

   macro S_NEW
   macro S_NEW_FROM_NAME
   macro S_DELETE
   macro S_FORCE_DELETE
   macro S_CAST_SAFE
   macro S_CAST_UNSAFE
   macro S_CAST
   macro S_OBJECT_CALL
   macro S_OBJECT_METH_VALID
   macro S_OBJECT
   macro S_OBJECT_REF
   macro S_OBJECT_CLS
   macro S_OBJECTCLASS
   macro S_FIND_CLASS


New/Delete
==========

.. doxybridge:: S_NEW
   :type: macro

.. doxybridge:: S_NEW_FROM_NAME
   :type: macro

.. doxybridge:: S_DELETE
   :type: macro

.. doxybridge:: S_FORCE_DELETE
   :type: macro


Cast
====

.. doxybridge:: S_CAST_SAFE
   :type: macro

.. doxybridge:: S_CAST_UNSAFE
   :type: macro

.. doxybridge:: S_CAST
   :type: macro

See also :c:macro:`SPCT_DO_SAFE_CAST`


Function Methods
================

.. doxybridge:: S_OBJECT_CALL
   :type: macro

.. doxybridge:: S_OBJECT_METH_VALID
   :type: macro

.. doxybridge:: S_OBJECT
   :type: macro


Reference 
=========

.. doxybridge:: S_OBJECT_REF
   :type: macro

Class
=====

.. doxybridge:: S_OBJECT_CLS
   :type: macro

.. doxybridge:: S_OBJECTCLASS
   :type: macro

.. doxybridge:: S_FIND_CLASS
   :type: macro
