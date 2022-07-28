/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_WebAuthnSecureStorage_h
#define mozilla_dom_WebAuthnSecureStorage_h

// #include "nsTHashMap.h"

namespace mozilla { 
namespace dom {

class WebAuthnSecureStorage {
 public:
  static WebAuthnSecureStorage* GetInstance();
  // static void Release(){};
  // nsresult SetSecureOptions(nsCString host, nsCString options);
 private:
  WebAuthnSecureStorage();
  ~WebAuthnSecureStorage();
  static WebAuthnSecureStorage* gSecureStorage;
  // nsTHashMap<nsAutoCString, nsAutoCString>* storage;
};

} } // namespace mozilla::dom

#endif  // mozilla_dom_WebAuthnManager_h
