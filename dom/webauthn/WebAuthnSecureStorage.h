/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_WebAuthnSecureStorage_h
#define mozilla_dom_WebAuthnSecureStorage_h

// #include "nsTHashMap.h"
#include <map>
#include "nsString.h"

namespace mozilla { 
namespace dom {

class WebAuthnSecureStorage {
 public:
  static WebAuthnSecureStorage* GetInstance();
  nsresult SetSecureOptions(nsAutoCString host, nsCString options);
 private:
  WebAuthnSecureStorage();
  ~WebAuthnSecureStorage();
  static WebAuthnSecureStorage* gInstance;
  std::map<nsAutoCString, nsAutoCString> storage;
  // nsTHashMap<nsAutoCString, nsAutoCString>* storage;
};

} } // namespace mozilla::dom

#endif  // mozilla_dom_WebAuthnManager_h
