# Singleton Analysis
The goal of this document is to record my investigation into singletons and document what does and doesn't work.

## Singletons I've Examined
### [PermissionManager.cpp](extensions/permissions/PermissionManager.cpp)
- instance type - static StaticRefPtr<PermissionManager>
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

###