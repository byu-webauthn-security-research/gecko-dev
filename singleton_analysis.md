# Singleton Analysis
The goal of this document is to record my investigation into singletons and document what does and doesn't work.

## Singletons I've Examined
### [PermissionManager.cpp](extensions/permissions/PermissionManager.cpp)
- instance type - static StaticRefPtr<PermissionManager>
  - Declared in the .cpp file
- used by the following classes: 
  - dom/ipc/ContentChild.cpp
  - dom/ipc/ContentParent.cpp
  - dom/ipc/ProcessIsolation.cpp
  - dom/serviceworkers/ServiceWorkerManager.cpp
  - dom/workers/remoteworkers/RemoteWorkerChild.cpp
  - extensions/permissions/PermissionDelegateHandler.cpp
  - extensions/permissions/PermissionManager.cpp
  - extensions/permissions/test/gtest/PermissionManagerTest.cpp
  - netwerk/cookie/CookieJarSettings.cpp
  - toolkit/components/antitracking/AntiTrackingUtils.cpp
  - toolkit/components/antitracking/ContentBlockingAllowList.cpp
  - toolkit/components/antitracking/ContentBlockingTelemetryService.cpp
  - toolkit/components/antitracking/ContentBlockingUserInteraction.cpp
  - toolkit/components/antitracking/StorageAccess.cpp
  - toolkit/components/antitracking/StorageAccessAPIHelper.cpp
- The module usage would imply that mimicing this setup would be beneficial as it is used in both the dom and netwerk modules.
- No AddRefPtr and Release methods defined, probably getting them from an inherited class

### [AndroidWebAuthnTokenManager](dom/webauthn/AndroidWebAuthnTokenManager.cpp)
- instance type StaticRefPtr<AndroidWebAuthnTokenManager> (note absense of static keyword)
  - Declared in the .cpp file
- Asserts it is on the background thread when creating the instance object


## Things that I've tried

This section is just a sanity check for myself, no guarantee that all combinations have been tried

- Declaring static ptr in cpp file: when compiling the name had a conflict with another class which would suggest a global scope but still no carry over between processes.
- NS_INLINE_DECL_REFCOUNTING is a macro that should add the AddRef and Release functions, no different than just using manual defs but that may have been because I didn't get to a necessary point