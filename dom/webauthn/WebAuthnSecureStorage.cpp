#include "mozilla/dom/WebAuthnSecureStorage.h"
#include "mozilla/ipc/BackgroundParent.h"
#include <cstdio>
#include <iostream>
#include <thread>
#include <map>
#include <unistd.h>
// #include "nsCOMPtr.h"

// nsTHashMap<nsAutoCString, nsAutoCString>* WebAuthnSecureStorage::storage = nullptr;
namespace mozilla::dom {

WebAuthnSecureStorage::WebAuthnSecureStorage() {
  storage = std::map<nsAutoCString, nsAutoCString>();
  printf("WebAuthnSecureStorage::WebAuthnSecureStorage -- created\n");
}

StaticRefPtr<WebAuthnSecureStorage> WebAuthnSecureStorage::gInstance;

WebAuthnSecureStorage* WebAuthnSecureStorage::GetInstance() {
  // mozilla::ipc::AssertIsInMainProcess();
  // AssertIsOnMainThread();
  printf("WebAuthnSecureStorage::GetInstance -- thread: ");
  std::cout << std::this_thread::get_id() << " process: " << getpid() << std::endl;
  printf("WebAuthnSecureStorage::GetInstance -- gInstance: %p\n", gInstance.get());
  if (!gInstance) {
    auto secureStorage = MakeRefPtr<WebAuthnSecureStorage>();
    gInstance = secureStorage.get();
  }
  return gInstance;
}

int WebAuthnSecureStorage::AddRef() { return 1; }

int WebAuthnSecureStorage::Release() { return 0; }

nsresult WebAuthnSecureStorage::SetSecureOptions(nsAutoCString host, nsCString options) {
  storage.insert(std::make_pair(host, options));
  printf("WebAuthnSecureStorage::SetSecureOptions -- set %s to %s\n", host.get(), storage.at(host).get());
  return NS_OK;
}

nsresult WebAuthnSecureStorage::GetSecureOptions(nsCString host, nsCString* options) {
  printf("WebAuthnSecureStorage::GetSecureOptions -- entered function\n");
  nsAutoCString host_t = nsAutoCString(host.get());
  options = &storage.at(host_t);
  printf("WebAuthnSecureStorage::GetSecureOptions -- retrieved %s as %s\n", host_t.get(), options->get());
  return NS_OK;
}

}