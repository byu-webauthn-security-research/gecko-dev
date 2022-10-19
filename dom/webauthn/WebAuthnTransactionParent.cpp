/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/WebAuthnTransactionParent.h"
#include <iostream>
#include "WebAuthnSecureStorage.h"
#include "mozilla/dom/U2FTokenManager.h"
#include "mozilla/ipc/PBackgroundParent.h"
#include "mozilla/ipc/BackgroundParent.h"
#include <inttypes.h>

#ifdef OS_WIN
#  include "WinWebAuthnManager.h"
#endif

namespace mozilla::dom {

mozilla::ipc::IPCResult WebAuthnTransactionParent::RecvRequestRegister(
    const uint64_t & aTransactionId,
    const WebAuthnMakeCredentialInfo & aTransactionInfo) {
  ::mozilla::ipc::AssertIsOnBackgroundThread();

#ifdef OS_WIN
  if (WinWebAuthnManager::AreWebAuthNApisAvailable()) { // Daniel's section
    WinWebAuthnManager* mgr = WinWebAuthnManager::Get();
    // Secure Storage Start
    std::cout << "WebAuthnTransactionParent::RecvRequestRegister if (OS WIN) -- Thread: " << std::this_thread::get_id() << " process: " << getpid() << " parent: " << getpid() << std::endl;

    WebAuthnSecureStorage* storage = WebAuthnSecureStorage::GetInstance();
    std::cout << "WebAuthnTransactionParent::RecvRequestRegister -- ID from storage if OS : "<< storage->GetID()<< std::endl;
    std::cout <<"WebAuthnTransactionParent::RecvRequestRegister -- atransaction id: " << aTransactionId <<std::endl;
    //std::cout <<"WebAuthnTransactionParent::RecvRequestRegister -- atransaction info: " << aTransactionInfo.rpID() <<std::endl;
    // Secure Storage END


    //mgr->Register(this, aTransactionId, storage->GetInfo()); //my info
    mgr->Register(this, aTransactionId, aTransactionInfo);
  } else {
    U2FTokenManager* mgr = U2FTokenManager::Get();
    mgr->Register(this, aTransactionId, aTransactionInfo);
  }
#else
  std::cout << "WebAuthnTransactionParent::RecvRequestRegister -- Thread: " << std::this_thread::get_id() << " process: " << getpid() << " parent: " << getpid() << std::endl;
  std::cout << "WebAuthnTransactionParent::RecvRequestRegister -- RpId: " << aTransactionInfo.RpId() << std::endl;


  WebAuthnSecureStorage* storage = WebAuthnSecureStorage::GetInstance();
  nsCString options = storage->GetSecureOptions();
  // std::cout << "WebAuthnTransactionParent::RecvRequestRegister -- Options new line "<< std::endl;
  //std::cout << "WebAuthnTransactionParent::RecvRequestRegister -- Options from func: " << options.get() << std::endl;
  std::cout <<"WebAuthnTransactionParent::RecvRequestRegister -- atransaction info: " << aTransactionInfo <<std::endl;
  std::cout <<"WebAuthnTransactionParent::RecvRequestRegister -- atransaction id: " << aTransactionId <<std::endl;
  // deserialize this so that the id and the info are gotten

  U2FTokenManager* mgr = U2FTokenManager::Get();
  mgr->Register(this, options, storage->Info); // need to change these two parameters, id and info so that it is us injecting the authen token
#endif

  return IPC_OK();
}

mozilla::ipc::IPCResult WebAuthnTransactionParent::RecvRequestSign(
    const uint64_t& aTransactionId,
    const WebAuthnGetAssertionInfo& aTransactionInfo) {
  ::mozilla::ipc::AssertIsOnBackgroundThread();

#ifdef OS_WIN
  if (WinWebAuthnManager::AreWebAuthNApisAvailable()) {
    WinWebAuthnManager* mgr = WinWebAuthnManager::Get();
    std::cout << "WebAuthnTransactionParent::AreWebAuthNApisAvailable if (OS WIN) -- Thread: " <<std::endl;
    WebAuthnSecureStorage* storage = WebAuthnSecureStorage::GetInstance();
    mgr->Sign(this, aTransactionId, aTransactionInfo);
  } else {
    U2FTokenManager* mgr = U2FTokenManager::Get();
    std::cout << "WebAuthnTransactionParent::AreWebAuthNApisAvailable else inner________________ (OS WIN) -- Thread: " <<std::endl;
    WebAuthnSecureStorage* storage = WebAuthnSecureStorage::GetInstance();
    mgr->Sign(this, aTransactionId, aTransactionInfo);
  }
#else
  U2FTokenManager* mgr = U2FTokenManager::Get();
  std::cout << "WebAuthnTransactionParent::AreWebAuthNApisAvailable else (OS WIN) -- Thread: " <<std::endl;
  mgr->Sign(this, aTransactionId, aTransactionInfo);
#endif

  return IPC_OK();
}

mozilla::ipc::IPCResult WebAuthnTransactionParent::RecvRequestCancel(
    const Tainted<uint64_t>& aTransactionId) {
  ::mozilla::ipc::AssertIsOnBackgroundThread();

#ifdef OS_WIN
  if (WinWebAuthnManager::AreWebAuthNApisAvailable()) {
    std::cout << "WebAuthnTransactionParent::RecvRequestCancel" <<std::endl;
    WinWebAuthnManager* mgr = WinWebAuthnManager::Get();
    mgr->Cancel(this, aTransactionId);
  } else {
    U2FTokenManager* mgr = U2FTokenManager::Get();
    std::cout << "WebAuthnTransactionParent::RecvRequestCancel else" <<std::endl;
    mgr->Cancel(this, aTransactionId);
  }
#else
  U2FTokenManager* mgr = U2FTokenManager::Get();
  mgr->Cancel(this, aTransactionId);
#endif

  return IPC_OK();
}

mozilla::ipc::IPCResult WebAuthnTransactionParent::RecvDestroyMe() {
  ::mozilla::ipc::AssertIsOnBackgroundThread();

  // The child was disconnected from the WebAuthnManager instance and will send
  // no further messages. It is kept alive until we delete it explicitly.

  // The child should have cancelled any active transaction. This means
  // we expect no more messages to the child. We'll crash otherwise.

  // The IPC roundtrip is complete. No more messages, hopefully.
  IProtocol* mgr = Manager();
  if (!Send__delete__(this)) {
    return IPC_FAIL_NO_REASON(mgr);
  }

  return IPC_OK();
}

void WebAuthnTransactionParent::ActorDestroy(ActorDestroyReason aWhy) {
  ::mozilla::ipc::AssertIsOnBackgroundThread();

  // Called either by Send__delete__() in RecvDestroyMe() above, or when
  // the channel disconnects. Ensure the token manager forgets about us.

#ifdef OS_WIN
  if (WinWebAuthnManager::AreWebAuthNApisAvailable()) {
    WinWebAuthnManager* mgr = WinWebAuthnManager::Get();
    if (mgr) {
      mgr->MaybeClearTransaction(this);
    }
  } else {
    U2FTokenManager* mgr = U2FTokenManager::Get();
    if (mgr) {
      mgr->MaybeClearTransaction(this);
    }
  }
#else
  U2FTokenManager* mgr = U2FTokenManager::Get();
  if (mgr) {
    mgr->MaybeClearTransaction(this);
  }
#endif
}

WebAuthnTransactionParent::WebAuthnTransactionParent() {
  std::cout << "WebAuthnTransactionParent::WebAuthnTransactionParent -- Thread: " << std::this_thread::get_id() << " process: " << getpid() << " parent: " << getpid() << std::endl;
  WebAuthnSecureStorage* storage = WebAuthnSecureStorage::GetInstance();
  nsCString options = storage->GetSecureOptions();
}


}  // namespace mozilla::dom
