/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_WebAuthnSecureStorage_h
#define mozilla_dom_WebAuthnSecureStorage_h

// #include "nsTHashMap.h"
#include <map>
#include "mozilla/RefPtr.h"
#include "nsISupportsBase.h"
#include "nsString.h"
#include "mozilla/StaticPtr.h"
#include "nsWeakReference.h"

namespace mozilla::dom {

class WebAuthnSecureStorage final {
 public:
  NS_INLINE_DECL_REFCOUNTING(WebAuthnSecureStorage);
  static WebAuthnSecureStorage* GetInstance();
  nsresult SetSecureOptions(nsCString options);
  nsCString GetSecureOptions();
  // int AddRef();
  // int Release();
  WebAuthnSecureStorage();
 private:
  // static mozilla::StaticRefPtr<WebAuthnSecureStorage> gInstance;
  ~WebAuthnSecureStorage() = default;
  std::map<nsAutoCString, nsAutoCString> storage;
  nsCString options;
  // nsTHashMap<nsAutoCString, nsAutoCString>* storage;
};

} // namespace mozilla::dom

#endif  // mozilla_dom_WebAuthnManager_h
