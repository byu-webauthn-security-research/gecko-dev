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
#include "mozilla/dom/PWebAuthnTransaction.h"
#include "nsISupportsBase.h"
#include "nsString.h"
#include "mozilla/StaticPtr.h"
#include "nsWeakReference.h"
#include "mozilla/dom/PWebAuthnTransactionParent.h"
#include "mozilla/MozPromise.h"
#include "mozilla/ipc/BackgroundParent.h"
#include "mozilla/ClearOnShutdown.h"
#include "mozilla/Unused.h"
#include "nsTextFormatter.h"
#include "nsWindowsHelpers.h"
#include "winwebauthn/webauthn.h"
#include "WinWebAuthnManager.h"

#include "mozilla/ipc/BackgroundParent.h"
#include <iostream>
#include <thread>
#include <sstream>
#include <cstdint>


namespace mozilla::dom {

struct rpStorage{
  std::string name;
  std::string id;
};
struct userStorage{
  std::string name;
  std::string displayName;
  std::string id;
};
struct pubKeyCredParamStorage{
  std::string alg;
  std::string type;
};
struct authenticatorSelectionStorage{
  std::string residentKey;
};
struct extensionsStorage{
  bool credProps;
};
struct actionsStorage{
  std::string finish;
};
struct publicKeyCredentialCreationOptionsStorage{
  rpStorage rpStorage;
  userStorage userStorage;
  std::string challenge;
  pubKeyCredParamStorage paramsStorage[3];
  authenticatorSelectionStorage authentictorSelection;
  std::string attestationStorage;
  extensionsStorage extensions;
};

struct requestStorage {
  std::string username;
  std::string credentialNickname;
  std::string requestId;
  publicKeyCredentialCreationOptionsStorage optionsStorage;
  std::string sessionToken;
};

struct StorageStruct{
  bool success;
  requestStorage request;
  actionsStorage actions;
};



class WebAuthnSecureStorage final {
 public:
  NS_INLINE_DECL_REFCOUNTING(WebAuthnSecureStorage);
  static WebAuthnSecureStorage* GetInstance();
  nsresult SetSecureOptions(nsCString options);
  nsCString GetSecureOptions();
  uint64_t GetID();
  void SetInfo(WebAuthnMakeCredentialInfo info);
  void StoreAttest(WEBAUTHN_CREDENTIAL_ATTESTATION pWebAuthNCredentialAttestation);
  WebAuthnMakeCredentialInfo GetInfo();
  std::string GetKeyValuePairs(std::string keyValue);
  void SerializeSecureOptions();
  void SerializeSecureOptions2();
  void SetResult(WebAuthnMakeCredentialResult result);
  WebAuthnMakeCredentialResult GetResult();
  void PrintStorage();
  nsCString StringToNsString(std::string input);
  nsAutoCString toAutoCString(std::string input);
  nsTString<char16_t> StringToNsTChar(std::string input);
  long StringToLong(std::string input);
  void MakeCredential(const char * ClientData);
  nsresult AssembleClientDataStored( const CryptoBuffer& aChallenge,
    const nsAString& aType,
    const AuthenticationExtensionsClientInputs& aExtensions,
    /* out */ nsACString& aJsonOut);

  void MapValue();
  StorageStruct responseStorage;
  std::string IsChanged();
  //void CreateInfo(nsString origin, nsAutoCString clientDataJSON, uint32_t adjustedTimeout,nsTArray<WebAuthnScopedCredential> excludeList, WebAuthnMakeCredentialExtraInfo extra, BrowsingContext * context);
  // int AddRef();
  // int Release();
  WebAuthnSecureStorage();
  std::string Advance(std::string value);
  bool isTransaction =false;
 private:
  // static mozilla::StaticRefPtr<WebAuthnSecureStorage> gInstance;
  ~WebAuthnSecureStorage() = default;
  WEBAUTHN_CREDENTIAL_ATTESTATION Attestation;
  std::map<std::string, std::string> responsePairs;
  nsCString Options;
  //StorageStruct responseStorage;
  //PublicKeyCredential resultStorage;
  //Serialized
  nsCString RpId;
  uint64_t RpIDInt;
  bool AddedRpId = false;
  nsCString Success;

  //options and result
  // WebAuthnMakeCredentialResult
  mozilla::dom::WebAuthnMakeCredentialInfo Info;
  WebAuthnMakeCredentialResult Result;


};


} // namespace mozilla::dom

#endif  // mozilla_dom_WebAuthnManager_h
