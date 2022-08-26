#include "mozilla/dom/WebAuthnSecureStorage.h"
#include "mozilla/ipc/BackgroundParent.h"
#include "nsStringFwd.h"
#include <cstdio>
#include <iostream>
#include <thread>
#include <map>
#include <utility>
#include <unistd.h>
// #include "nsCOMPtr.h"

// nsTHashMap<nsAutoCString, nsAutoCString>* WebAuthnSecureStorage::storage = nullptr;
namespace mozilla::dom {

WebAuthnSecureStorage::WebAuthnSecureStorage() {
  // storage = std::map<nsAutoCString, nsAutoCString>();
  printf("WebAuthnSecureStorage::WebAuthnSecureStorage -- created\n");
}

static StaticRefPtr<WebAuthnSecureStorage> gWebAuthnSecureStorage;

WebAuthnSecureStorage* WebAuthnSecureStorage::GetInstance() {
  // mozilla::ipc::AssertIsInMainProcess();
  // AssertIsOnMainThread();
  printf("WebAuthnSecureStorage::GetInstance -- thread: ");
  std::cout << std::this_thread::get_id() << " process: " << getpid() << std::endl;
  printf("WebAuthnSecureStorage::GetInstance -- gInstance: %p\n", gWebAuthnSecureStorage.get());
  if (!gWebAuthnSecureStorage) {
    auto secureStorage = MakeRefPtr<WebAuthnSecureStorage>();
    gWebAuthnSecureStorage = secureStorage.get();
  }
  printf("WebAuthnSecureStorage::GetInstance -- gInstance: %p\n", gWebAuthnSecureStorage.get());
  return gWebAuthnSecureStorage;
}

// int WebAuthnSecureStorage::AddRef() { return 1; }

// int WebAuthnSecureStorage::Release() { return 0; }

nsresult WebAuthnSecureStorage::SetSecureOptions(nsCString options) {
  // storage.insert(std::make_pair(host, options));
  // printf("WebAuthnSecureStorage::SetSecureOptions -- set %s to %s\n", host.get(), storage.at(host).get());
  // printf("WebAuthnSecureStorage::SetSecureOptions -- map: %s\n", storage.cbegin()->first.get());
  this->options = std::move(options);

  return NS_OK;
}

nsCString WebAuthnSecureStorage::GetSecureOptions() {
  printf("WebAuthnSecureStorage::GetSecureOptions -- entered function\n");
  // nsAutoCString host_t = nsAutoCString(host.get());
  // printf("WebAuthnSecureStorage::GetSecureOptions -- map: %s\n", storage.cbegin()->first.get());
  // printf("WebAuthnSecureStorage::GetSecureOptions -- host: %s\n", host_t.get());
  // options = &(storage.at(host_t));
  // printf("WebAuthnSecureStorage::GetSecureOptions -- retrieved %s as %s\n", host_t.get(), options->get());
  return options;
}

}